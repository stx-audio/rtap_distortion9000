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
    x->mix = 0;
    x->distortionMod = 0;
    x->saturation = 1;
    x->frequency = 1000;
    x->quality = 1;
    x->gain = 20;
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
 * @related lfs_dist_dsp_setMix
 * @brief Sets the dry wet value of the output signal. <br>
 * @param x A pointer to the lfs_dist_dsp object <br>
 * @param mix the mix value 0-1<br>
 */
void lfs_dist_dsp_setMix(lfs_dist_dsp *x, float mix)
{
    x->mix = mix;
}

/**
 * @related lfs_dist_dsp_setdistortionMod
 * @brief Sets the distortion algorithm of the distortion object <br>
 * @param x A pointer to the lfs_dist_dsp object <br>
 * @param distortionMod sets the distortion algorithm 0-16 <br>
 */
void lfs_dist_dsp_setDistortionMod(lfs_dist_dsp *x, float distortionMod)
{
    x->distortionMod = (int)distortionMod ;
}

/**
 * @related lfs_dist_dsp_setSaturation
 * @brief Sets the saturation of the applied algorithm <br>
 * @param x A pointer to the lfs_dist_dsp object <br>
 * @param distortionMod sets the saturation 1-10 of the saturation for algo 2,4,5,6 <br>
 */
void lfs_dist_dsp_setSaturation(lfs_dist_dsp *x, float saturation)
{
    x->saturation = saturation ;
}

/**
 * @related lfs_dist_dsp_setFrequency
 * @brief Sets center frequency of the bandpass filter <br>
 * @param x A pointer to the lfs_dist_dsp object <br>
 * @param frequency sets center frequency of the bandpass filter between 50 and 20000 Hz <br>
 */
void lfs_dist_dsp_setFrequency(lfs_dist_dsp *x, float frequency)
{
    x->frequency = frequency;
}

/**
 * @related lfs_dist_dsp_setQuality
 * @brief Sets q value of the bandpass filter <br>
 * @param x A pointer to the lfs_dist_dsp object <br>
 * @param quality sets the quality of the bandpass filter between 0 and 16 <br>
 */
void lfs_dist_dsp_setQuality(lfs_dist_dsp *x, float quality)
{
    x->quality = quality;
}

/**
 * @related lfs_dist_dsp_setGain
 * @brief Sets the gain at center frequency of the bandpass filter<br>
 * @param x A pointer to the lfs_dist_dsp object <br>
 * @param gain sets the filter gain at center frequency <br>
 */
void lfs_dist_dsp_setGain(lfs_dist_dsp *x, float gain)
{
    x->gain = gain;
}


// ###########################################
// ################ BANDPASS #################
// ###########################################

/**
 * @param store1 variable for storing sample1 of the delay line <br>
 * @param store2 variable for storing sample2 of the delay line <br>
 * @param g variable for storing sample2 of the delay line <br>
 * @param factor1 filter coefficient <br>
 * @param factor2 filter coefficient <br>
 * @param sampleRate sample rate<br>
 */

float store1 = 0;
float store2 = 0;
float g, factor0, factor1, factor2;
float sampleRate = 44100;

/**
 * @related processBandpass
 * @brief should be called once for every sample <br>
 * @param in the input sample<br>
 * @param out the output sample<br>
 * @var var1 <br>
 * @var var2 <br>
 * @var var3 <br>
 * <br>
 * This function applies the mathematical formula for an analog modeling resonant bandpass filter to each sample and updates the delay line
 * <br>
 */
void processBandpass( float* in, float* out)
{
    float var3 = *in - store2;
    float var1 = store1 * factor0 + var3 * factor1;
    float var2 = store2 + store1 * factor1 + var3 * factor2;
    
    store1 = var1 * 2.0 - store1;
    store2 = var2 * 2.0 - store2;
    
    *out = var1;
}

/**
 * @related setBandpass
 * @brief Calculation of filter coefficients and resonance; should be called once for every buffer <br>
 * @param f the frequency of the filter <br>
 * @param q the quality of the filter <br>
 * @var factor -  filter coefficient<br>
 * @var res - resonance of the filter <br>
 * @var k  <br>
 */
void setBandpass( float* f,  float* q )
{
    float factor = tanf( M_PI * *f * (1/sampleRate)) ;
    
    float res = 1.0f - (1.0f / (2.0f * *q));
    
    float k = 2.0 - 2.0 * res;
    
    factor0 = 1.0 / (1.0 + factor*(factor + k));
    factor1 = factor*factor0;
    factor2 = factor*factor1;
}

// ##########################################
// ################ PROCESS #################
// ##########################################

/**
 * @related sgn
 * @brief Normalizes the input sample <br>
 * @param input the input sample that will be calculated <br>
 * @return the sign function, value will always be -1, 0 or 1<br>
 */
float sgn(float input)
{
    return input / fabsf(input);
}

/**
 * @related dry_wet_process
 * @brief Adds dry_wet_process to the signal chain. <br>
 * @param inSample A pointer to the input sample<br>
 * @param outSample A pointer to the output sample<br>
 * @param mix A pointer to the dry wet value as set in the pd patch<br>
 */
void dry_wet_process(float* inSample, float* outSample, float* mix)
{
    float mixInv = 1.f - *mix;
    // inSample is distorted signal, outSample is not yet distorted
    *outSample = (*inSample * mixInv) + (*outSample * *mix);
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
    float mix = x->mix;
    float sat = x->saturation;
    float f = x->frequency;
    float q = x->quality;
    float A = powf(10, x->gain/40);
    float filterFactor = 1/(q*A) * (A*A-1);
    
    setBandpass(&f, &q);
    
    // This construct is just as long as it is, to avoid processing the switch case for every sample!
    
    switch(mod){
            
            // #####################################################
            // ############ SOFT CLIP SIGMOID WAVESHAPERS ##########
            // #####################################################
            
        // Arraya (ARRY)
        case 0:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                float filterOut;
                processBandpass(&inputSig, &filterOut);
                inputSig = filterOut * filterFactor;
                
                // Distortion Algorithm
                inputSig = (3.f * inputSig)/2 * ( 1 - (powf(inputSig, 2) / 3));
                
                inputSig *= 2.5;
                
                // Mix
                dry_wet_process(&inputSig, out++, &mix);
        
                i++;
            }break;
            
        // Sigmoid (SIG)
        case 1:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                float filterOut;
                processBandpass(&inputSig, &filterOut);
                inputSig = filterOut * filterFactor;
                
                // Distortion Algorithm
                inputSig = 2 * ( 1 / ( 1 + expf( -sat * inputSig ) )) - 1;
                
                inputSig *= 0.7;
                
                // Mix
                dry_wet_process(&inputSig, out++, &mix);
                
                i++;
            }
            break;
            
        // Sigmoid 2 (SIG2)
        case 2:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                float filterOut;
                processBandpass(&inputSig, &filterOut);
                inputSig = filterOut * filterFactor;
                
                // Distortion Algorithm
                inputSig =  ((expf(inputSig)-1)*(M_E+1)) / ( (expf(inputSig) + 1)*(M_E - 1) );
                
                inputSig *= 3;
                
                // Mix
                dry_wet_process(&inputSig, out++, &mix);
                
                i++;
            }
            break;
            
        // Hyperbolic Tangent (TANH)
        // Good for diode simulation
        case 3:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                float filterOut;
                processBandpass(&inputSig, &filterOut);
                inputSig = filterOut * filterFactor;
                
                // Distortion Algorithm
                inputSig =  tanhf(sat * inputSig) / tanhf(sat) ;
                
                inputSig *= 0.3;
                
                // Mix
                dry_wet_process(&inputSig, out++, &mix);
                
                i++;
            }
            break;
            
        // Arctangent (ATAN)
        case 4:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                float filterOut;
                processBandpass(&inputSig, &filterOut);
                inputSig = filterOut * filterFactor;
                
                // Distortion Algorithm
                inputSig =  atanf(sat * inputSig) / atanf(sat) ;
                
                inputSig *= 0.4;
                
                // Mix
                dry_wet_process(&inputSig, out++, &mix);
                
                i++;
            }
            break;
            
        // Fuzz Exponential 1 (FEXP1)
        case 5:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                float filterOut;
                processBandpass(&inputSig, &filterOut);
                inputSig = filterOut * filterFactor;
                
                // Distortion Algorithm
                inputSig = sgn(inputSig) * ( (1-expf(-fabsf(sat * inputSig))) / (1 - expf(-sat)) ) ;

                inputSig *= 0.3;
                
                // Mix
                dry_wet_process(&inputSig, out++, &mix);
                
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
                
                float filterOut;
                processBandpass(&inputSig, &filterOut);
                inputSig = filterOut * filterFactor;
                
                // Distortion Algorithm
                inputSig = sgn(-inputSig) *  ( 1.f-expf(fabsf(inputSig) ) / (M_E-1.f) );
                // Fuzz is super loud...
                inputSig *= 0.03;
                
                // Mix
                dry_wet_process(&inputSig, out++, &mix);
                
                i++;
            }
            break;
        
        // Exponential 2 (EXP2)
        case 7:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                float filterOut;
                processBandpass(&inputSig, &filterOut);
                inputSig = filterOut * filterFactor;
                
                // Distortion Algorithm
                inputSig = ( M_E - expf(1.f-inputSig) ) /  ( M_E-1.f) ;
                
                // Mix
                dry_wet_process(&inputSig, out++, &mix);
                
                i++;
            }
            break;
        
        // Arctangent Square Root (ATSR)
        case 8:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                float filterOut;
                processBandpass(&inputSig, &filterOut);
                inputSig = filterOut * filterFactor;
                
                // Distortion Algorithm
                inputSig = 2.5f * (atanf(0.9f*inputSig)) + 2.5f * sqrtf(1-powf((0.9f * inputSig), 2.f)) - 2.5f ;
                
                inputSig *= 4;
                
                // Mix
                dry_wet_process(&inputSig, out++, &mix);
                
                i++;
            }
            break;
        
        // Square Sign (SQS)
        case 9:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                float filterOut;
                processBandpass(&inputSig, &filterOut);
                inputSig = filterOut * filterFactor;
                
                // Distortion Algorithm
                inputSig = powf(inputSig, 2.f) * sgn(inputSig);
                
                inputSig *= 0.05;
                
                // Mix
                dry_wet_process(&inputSig, out++, &mix);
                
                i++;
            }
            break;
            
        // Cube (CUBE)
        case 10:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                float filterOut;
                processBandpass(&inputSig, &filterOut);
                inputSig = filterOut * filterFactor;
                
                // Distortion Algorithm
                inputSig = powf(inputSig, 3.f);
                
                // Cube is super quiet
                inputSig *= 30;
                
                // Mix
                dry_wet_process(&inputSig, out++, &mix);
        
                i++;
            }
            break;
            
        // Hard Clipper (HCLIP)
        case 11:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                float filterOut;
                processBandpass(&inputSig, &filterOut);
                inputSig = filterOut * filterFactor;
                
                // Distortion Algorithm
                if(fabsf(inputSig) > 0.5){
                    inputSig = 0.5f * sgn(inputSig);
                }
                
                // Mix
                dry_wet_process(&inputSig, out++, &mix);
                
                i++;
            }
            break;
            
        // Half Wave Rectifier (HWR)
        case 12:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                float filterOut;
                processBandpass(&inputSig, &filterOut);
                inputSig = filterOut * filterFactor;
                
                // Distortion Algorithm
                inputSig = 0.5f * (inputSig + fabsf(inputSig));
                
                // Mix
                dry_wet_process(&inputSig, out++, &mix);
                
                // HWR is super quiet
                inputSig *= 4;
                
                i++;
            }
            break;
            
        // Full Wave Rectifier (FWR)
        case 13:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                float filterOut;
                processBandpass(&inputSig, &filterOut);
                inputSig = filterOut * filterFactor;
                
                // Distortion Algorithm
                inputSig = fabsf(inputSig);
                
                inputSig *= 3.8;
                
                // Mix
                dry_wet_process(&inputSig, out++, &mix);
               
                i++;
            }
            break;
            
        // Square Law (SQR)
        case 14:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                float filterOut;
                processBandpass(&inputSig, &filterOut);
                inputSig = filterOut * filterFactor;
                
                // Distortion Algorithm
                inputSig = powf(inputSig, 2);
                
                inputSig *= 3.8;
                
                // Mix
                dry_wet_process(&inputSig, out++, &mix);
                
                i++;
            }
            break;
            
        // Absolute Square Root (ASQRT)
        case 15:
            while(i < vectorSize)
            {
                float inputSig  = *in++;
                
                float filterOut;
                processBandpass(&inputSig, &filterOut);
                inputSig = filterOut * filterFactor;
                
                // Distortion Algorithm
                inputSig = sqrtf(fabsf(inputSig));
                
                inputSig *= 2.2;
                
                // Mix
                dry_wet_process(&inputSig, out++, &mix);
                
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
