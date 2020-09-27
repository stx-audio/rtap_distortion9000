/**
 * @file lfs_brickwall_pd.c
 * @author Lynn, Felix, Stephan
 * Audiocommunication Group, Technical University Berlin <br>
 * @brief Audio Object for distortion <br>
 * <br>
 * RTAP Distortion 9000 allows to select one of 16 distortion types and choose how much distortion will be applied to the signal. It is recommended to use multiple instances with bandpass filters to get the most out of this external.
 * <br>
 */

#include "lfs_dist_dsp.h"
#include <math.h>

const float econst = M_E;

/**
 * @related lfs_dist_dsp_new
 * @brief Creates a new dist_dsp object<br>
 * The function creates a new lfs_dist_dsp object <br>
 * @return a pointer to the newly created lfs_dist_dsp object <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
lfs_dist_dsp *lfs_dist_dsp_new()
{
	lfs_dist_dsp *x = (lfs_dist_dsp *)malloc(sizeof(lfs_dist_dsp));
    x->dryWet = 0.0;
    x->distortionMod = 0;
    x->saturation = 0;
    return x;
}

/**
 * @related lfs_dist_dsp_free
 * @brief destructor of the dsp object<br>
 * @param x A pointer to the lfs_dist_dsp object <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void lfs_dist_dsp_free(lfs_dist_dsp *x)
{
    free(x);
}

/**
 * @related lfs_dist_dsp_setdryWet
 * @brief Sets the dry wet value of the pd project. <br>
 * @param x A pointer to the lfs_dist_dsp object <br>
 * @param dryWet the dryWet value 0-1<br>
 */
void lfs_dist_dsp_setDryWet(lfs_dist_dsp *x, float dryWet)
{
    x->dryWet = dryWet;
}

/**
 * @related lfs_dist_dsp_setdistortionMod
 * @brief Sets the distortion type of the pure data project <br>
 * @param x A pointer to the lfs_dist_dsp object <br>
 * @param distortionMod sets the distortion algorithm 0-16 <br>
 */
void lfs_dist_dsp_setDistortionMod(lfs_dist_dsp *x, float distortionMod)
{
    x->distortionMod = (int)distortionMod ;
}

/**
 * @related lfs_dist_dsp_setSaturation
 * @brief Sets the saturation of the pure data project <br>
 * @param x A pointer to the lfs_dist_dsp object <br>
 * @param distortionMod sets the saturation 1-10 of the saturation for algo 2,4,5,6 <br>
 */
void lfs_dist_dsp_setSaturation(lfs_dist_dsp *x, float saturation)
{
    x->saturation = saturation ;
}

/**
 * @related sgn
 * @brief Adds lfs_dist_dsp_perform to the signal chain. <br>
 * @param input the input sample that will be calculated<br>
 * @return the sign function, value will always be -1, 0 or 1<br>
 */
float sgn(float input)
{
    return input / fabsf(input);
}

/**
 * @related dry_wet_process
 * @brief Adds lfs_dist_dsp_perform to the signal chain. <br>
 * @param inSample A pointer to the input sample<br>
 * @param outSample A pointer to the output sample<br>
 * @param dryWet A pointer to the dry wet value as set in the pd patch<br>
 */
void dry_wet_process(float* inSample, float* outSample, float* dryWet)
{
    float dryWetInv = 1.f - *dryWet;
    // inSample is distorted signal, outSample is not yet distorted
    *outSample = (*inSample * dryWetInv) + (*outSample * *dryWet);
}

/**
 * @related lfs_dist_process
 * @brief Adds the distortion to the signal, the signal processing is actually happening here<br>
 * @param x Pointer to the lfs_dist_dsp object <br>
 * @param in Pointer to the input vector <br>
 * @param out Pointer to the output vector<br>
 * @param vectorSize Length of the Vector<br>
 */
void lfs_dist_process(lfs_dist_dsp *x, lfs_INPUTVECTOR *in, lfs_OUTPUTVECTOR *out, int vectorSize)
{
    int i = 0;
    int mod = x->distortionMod;
    float dryWet = x->dryWet;
    float sat = x->saturation;
    
    switch(mod){
            
            // #####################################################
            // ############ SOFT CLIP SIGMOID WAVESHAPERS ##########
            // #####################################################
            
        // Arraya (ARRY)
        case 0:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = (3.f * inputSig)/2 * ( 1 - (powf(inputSig, 2) / 3));
                
                // Mix
                dry_wet_process(&inputSig, out++, &dryWet);
        
                i++;
            }break;
            
        // Sigmoid (SIG)
        case 1:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = 2 * ( 1 / ( 1 + expf( -sat * inputSig ) )) - 1;
                
                // Mix
                dry_wet_process(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
            
        // Sigmoid 2 (SIG2)
        case 2:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig =  ((expf(inputSig)-1)*(M_E+1)) / ( (expf(inputSig) + 1)*(M_E - 1) );
                
                // Mix
                dry_wet_process(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
            
        // Hyperbolic Tangent (TANH)
        // Good for diode simulation
        case 3:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig =  tanhf(sat * inputSig) / tanhf(sat) ;
                
                // Mix
                dry_wet_process(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
            
        // Arctangent (ATAN)
        case 4:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig =  atanf(sat * inputSig) / atanf(sat) ;
                
                // Mix
                dry_wet_process(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
            
        // Fuzz Exponential 1 (FEXP1)
        case 5:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig =  sgn(inputSig) * ( (1-expf(-fabsf(sat * inputSig))) / (1 - expf(-sat)) ) ;
                
                // Mix
                dry_wet_process(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
            
            // ##########################################
            // ############ EXOTIC DISTORTIONS ##########
            // ##########################################
        
        // Fuzz Exponential 2 (FEXP2)
        case 6:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = sgn(-inputSig) *  ( 1.f-expf(fabsf(inputSig) ) / (M_E-1.f) );
                
                // Mix
                dry_wet_process(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
        
        // Exponential 2 (EXP2)
        case 7:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = ( M_E - expf(1.f-inputSig) ) /  ( M_E-1.f) ;
                
                // Mix
                dry_wet_process(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
        
        // Arctangent Square Root (ATSR)
        case 8:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = 2.5f * (atanf(0.9f*inputSig)) + 2.5f * sqrtf(1-powf((0.9f * inputSig), 2.f)) - 2.5f ;
                
                // Mix
                dry_wet_process(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
        
        // Square Sign (SQS)
        case 9:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = powf(inputSig, 2.f) * sgn(inputSig);
                
                // Mix
                dry_wet_process(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
            
        // Cube (CUBE)
        case 10:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = powf(inputSig, 3.f);
                
                // Mix
                dry_wet_process(&inputSig, out++, &dryWet);
        
                i++;
            }
            break;
            
        // Hard Clipper (HCLIP)
        case 11:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                if(fabsf(inputSig) > 0.5){
                    inputSig = 0.5f * sgn(inputSig);
                }
                
                // Mix
                dry_wet_process(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
            
        // Half Wave Rectifier (HWR)
        case 12:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = 0.5f * (inputSig + fabsf(inputSig));
                
                // Mix
                dry_wet_process(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
            
        // Full Wave Rectifier (FWR)
        case 13:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = fabsf(inputSig);
                
                // Mix
                dry_wet_process(&inputSig, out++, &dryWet);
               
                i++;
            }
            break;
            
        // Square Law (SQR)
        case 14:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = powf(inputSig, 2);
                
                // Mix
                dry_wet_process(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
            
        // Absolute Square Root (ASQRT)
        case 15:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = sqrtf(fabsf(inputSig));
                
                // Mix
                dry_wet_process(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
    }
    
}

/**
 * @related lfs_dist_dsp_perform
 * @brief Adds lfs_dist_dsp_perform to the signal chain. <br>
 * @param x Pointer to the lfs_dist_dsp object <br>
 * @param in Pointer to the input vector <br>
 * @param out Pointer to the output vector<br>
 * @param vectorSize Length of the Vector<br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void lfs_dist_dsp_perform(lfs_dist_dsp *x, lfs_INPUTVECTOR *in, lfs_OUTPUTVECTOR *out, int vectorSize)
{
	lfs_dist_process(x, in, out, vectorSize);
}
