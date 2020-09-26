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
		x = 1.0;
	}
	if (x<0)
	{
		x = -1.0;
	}
	if (x == 0.0)
	{
		x = 0.0;
	}
	return x;
}


/**
 * @related stp_dist_dsp_dryWet
 * @brief Adds stp_brickwall_tilde_perform to the signal chain. <br>
 * @param x A pointer the stp_brickwall_tilde object <br>
 * @param sp A pointer the input and output vectors <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void stp_dist_dsp_dryWet(stp_dist_dsp *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize)
{
    int i = 0;
    int mod = 0;
    mod = x->distortionMod;
    switch(mod){
    				case 1:
					while(i < vectorSize)
					{
						float inputsig  = *in++;
						*out++ = (inputsig * (1-(x->dryWet))) + ((inputsig * inputsig * inputsig) * x->dryWet);
						i++;
					}break;

    				case 2:
    					while(i < vectorSize)
					{
						float inputsig  = *in++;


						*out++ = (inputsig * (1-(x->dryWet))) + ((sqrt(fabs(inputsig))) * x->dryWet);
						i++;
					}break;
    				case 3:
    									while(i < vectorSize)
    									{
    										float inputsig  = *in++;
    										if (fabs(inputsig) > 0.5)
    										{
    											*out++ = (inputsig * (1-(x->dryWet))) + (0.5*sgn(inputsig)*x->dryWet);
    										}
    										else{
    										*out++ = inputsig;
    										}
    										i++;
    									}break;
}
}
/**void stp_dist_dsp_clip(stp_dist_dsp *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize)
{
    int i = 0;
    while(i < vectorSize)
    {
        *out = *in;
        
        if(*out > x->clippingLevel)
            *out = x->clippingLevel;
        if(*out < -x->clippingLevel)
            *out = -x->clippingLevel;
        
        out++;
        in++;
        i++;
    }
}
**/

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
