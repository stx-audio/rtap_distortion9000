/**
 * @file stp_brickwall_pd.c
 * @author Lynn <br> Felix <br> Stephan
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
    x->distortionMod= 0;
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
void stp_dist_dsp_setdryWet(stp_dist_dsp *x, float dryWet)
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
void stp_dist_dsp_setdistortionMod(stp_dist_dsp *x, float distortionMod )
{
    x->distortionMod = distortionMod ;
}

/**
 * @related sgn
 * @brief Adds stp_brickwall_tilde_perform to the signal chain. <br>
 * @param x A pointer the stp_brickwall_tilde object <br>
 * @param sp A pointer the input and output vectors <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
float sgn(float x)
{
    if (x>0)
    {
        x = 1.0f;
    }
    if (x<0)
    {
        x = -1.0;
    }
    if (x == 0.0)
    {
        x = 0.0f;
    }
    return x;
}

/**
 * @related sgn
 * @brief Adds stp_brickwall_tilde_perform to the signal chain. <br>
 * @param x A pointer the stp_brickwall_tilde object <br>
 * @param sp A pointer the input and output vectors <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void dryWetProcess(float* inSample, float* outSample, float* dryWet)
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
void stp_dist_dsp_dryWet(stp_dist_dsp *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize)
{
    int i = 0;
    int mod = x->distortionMod;
    float dryWet = x->dryWet;
    float k = 0; // saturation
    
    switch(mod){
            
            // #####################################################
            // ############ SOFT CLIP SIGMOID WAVESHAPERS ##########
            // #####################################################
            
        // Arraya (ARRY)
        case 1:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = (3.f * inputSig)/2 * ( 1 - (powf(inputSig, 2) / 3));
                
                // Mix
                dryWetProcess(&inputSig, out++, &dryWet);
        
                i++;
            }break;
            
        // Sigmoid (SIG)
        case 2:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = 2 * ( 1 / ( 1 + expf( -k * inputSig ) )) - 1;
                
                // Mix
                dryWetProcess(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
            
        // Sigmoid 2 (SIG2)
        case 3:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig =  ((expf(inputSig)-1)*(M_E+1)) / ( (expf(inputSig) + 1)*(M_E - 1) );
                
                // Mix
                dryWetProcess(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
            
        // Hyperbolic Tangent (TANH)
        // Good for diode simulation
        case 4:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig =  tanhf(k * inputSig) / tanhf(k) ;
                
                // Mix
                dryWetProcess(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
            
        // Arctangent (ATAN)
        case 5:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig =  atanf(k * inputSig) / atanf(k) ;
                
                // Mix
                dryWetProcess(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
            
        // Fuzz Exponential 1 (FEXP1)
        case 6:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig =  sgn(inputSig) * ( (1-expf(-fabsf(k * inputSig))) / (1 - expf(-k)) ) ;
                
                // Mix
                dryWetProcess(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
            
            // ##########################################
            // ############ EXOTIC DISTORTIONS ##########
            // ##########################################
        
        // Fuzz Exponential (FEXP2)
        case 7:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = sgn(-inputSig) *  ( 1.f-expf(fabsf(inputSig) ) / (M_E-1.f) );
                
                // Mix
                dryWetProcess(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
        
        // Exponential (EXP2)
        case 8:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = ( M_E - expf(1.f-inputSig) ) /  ( M_E-1.f) ;
                
                // Mix
                dryWetProcess(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
        
        // Arctangent Square Root (ATSR)
        case 9:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = 2.5f * (atanf(0.9f*inputSig)) + 2.5f * sqrtf(1-powf((0.9f * inputSig), 2.f)) - 2.5f ;
                
                // Mix
                dryWetProcess(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
        
        // Square Sign (SQS)
        case 10:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = powf(inputSig, 2.f) * sgn(inputSig);
                
                // Mix
                dryWetProcess(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
            
        // Cube (CUBE)
        case 11:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = powf(inputSig, 3.f);
                
                // Mix
                dryWetProcess(&inputSig, out++, &dryWet);
        
                i++;
            }
            break;
            
        // Hard Clipper (HCLIP)
        case 12:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                if(fabsf(inputSig) > 0.5){
                    inputSig = 0.5f * sgn(inputSig);
                }
                
                // Mix
                dryWetProcess(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
            
        // Half Wave Rectifier (HWR)
        case 13:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = 0.5f * (inputSig + fabsf(inputSig));
                
                // Mix
                dryWetProcess(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
            
        // Full Wave Rectifier (FWR)
        case 14:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = fabsf(inputSig);
                
                // Mix
                dryWetProcess(&inputSig, out++, &dryWet);
               
                i++;
            }
            break;
            
        // Square Law (SQR)
        case 15:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = powf(inputSig, 2);
                
                // Mix
                dryWetProcess(&inputSig, out++, &dryWet);
                
                i++;
            }
            break;
            
        // Absolute Square Root (ASQRT)
        case 16:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                // Distortion Algorithm
                inputSig = sqrtf(fabsf(inputSig));
                
                // Mix
                dryWetProcess(&inputSig, out++, &dryWet);
                
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
	stp_dist_dsp_dryWet(x, in, out, vectorSize);
/**stp_dist_dsp_clip(x, in, out, vectorSize);**/
}
