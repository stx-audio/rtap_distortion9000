/**
 * @file stp_brickwall_pd.c
 * @author Lynn, Felix, Stephan
 * Audiocommunication Group, Technical University Berlin <br>
 * A distortion object for pure data <br>
 * <br>
 * @brief A Pure Data distortion object for generating overtones<br>
 */

#include "stp_dist_dsp.h"
#include <math.h>

const float econst = M_E;

stp_dist_dsp *stp_dist_dsp_new()
{
	stp_dist_dsp *x = (stp_dist_dsp *)malloc(sizeof(stp_dist_dsp));
    x->dryWet = 0.0;
    x->distortionMod = 0;
    x->saturation = 0;
    return x;
}

/**
 * @related stp_dist_dsp_free
 * @brief Adds stp_brickwall_tilde_perform to the signal chain. <br>
 * @param x A pointer the stp_brickwall_tilde object <br>
 * @param sp A pointer the input and output vectors <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void stp_dist_dsp_free(stp_dist_dsp *x)
{
    free(x);
}

/**
 * @related stp_dist_dsp_setdryWet
 * @brief Adds stp_brickwall_tilde_perform to the signal chain. <br>
 * @param x A pointer the stp_brickwall_tilde object <br>
 * @param sp A pointer the input and output vectors <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void stp_dist_dsp_setDryWet(stp_dist_dsp *x, float dryWet)
{
    x->dryWet = dryWet;
}

/**
 * @related stp_dist_dsp_setdistortionMod
 * @brief Adds stp_brickwall_tilde_perform to the signal chain. <br>
 * @param x A pointer the stp_brickwall_tilde object <br>
 * @param sp A pointer the input and output vectors <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void stp_dist_dsp_setDistortionMod(stp_dist_dsp *x, float distortionMod)
{
    x->distortionMod = (int)distortionMod ;
}

void stp_dist_dsp_setSaturation(stp_dist_dsp *x, float saturation)
{
    x->saturation = saturation ;
}

/**
 * @related sgn
 * @brief Adds stp_brickwall_tilde_perform to the signal chain. <br>
 * @param input A pointer the stp_brickwall_tilde object <br>
 * @param sp A pointer the input and output vectors <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
float sgn(float input)
{
    return input / fabsf(input);
}

/**
 * @related sgn
 * @brief Adds stp_brickwall_tilde_perform to the signal chain. <br>
 * @param x A pointer the stp_brickwall_tilde object <br>
 * @param sp A pointer the input and output vectors <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void dry_wet_process(float* inSample, float* outSample, float* dryWet)
{
    float dryWetInv = 1.f - *dryWet;
    
    *outSample = (*inSample * dryWetInv) + (*outSample * *dryWet);
}

/**
 * @related stp_dist_dsp_dryWet
 * @brief Adds stp_brickwall_tilde_perform to the signal chain. <br>
 * @param x A pointer the stp_brickwall_tilde object <br>
 * @param sp A pointer the input and output vectors <br>
 */
void stp_dist_process(stp_dist_dsp *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize)
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
        
        // Fuzz Exponential (FEXP2)
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
        
        // Exponential (EXP2)
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
 * @related stp_dist_dsp_perform
 * @brief Adds stp_brickwall_tilde_perform to the signal chain. <br>
 * @param x A pointer the stp_brickwall_tilde object <br>
 * @param sp A pointer the input and output vectors <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void stp_dist_dsp_perform(stp_dist_dsp *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize)
{
	stp_dist_process(x, in, out, vectorSize);
}
