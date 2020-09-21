#include "stp_dist_dsp.h"
#include <math.h>
stp_dist_dsp *stp_dist_dsp_new()
{
	stp_dist_dsp *x = (stp_dist_dsp *)malloc(sizeof(stp_dist_dsp));
    x->dryWet = 0.0;
    x->distortionMod= 0;
    return x;
}

void stp_dist_dsp_free(stp_dist_dsp *x)
{
    free(x);
}

void stp_dist_dsp_setdryWet(stp_dist_dsp *x, float dryWet)
{
    x->dryWet = dryWet;
}

void stp_dist_dsp_setdistortionMod(stp_dist_dsp *x, float distortionMod )
{
    x->distortionMod = distortionMod ;
}

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
void stp_dist_dsp_perform(stp_dist_dsp *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize)
{
	stp_dist_dsp_dryWet(x, in, out, vectorSize);
/**stp_dist_dsp_clip(x, in, out, vectorSize);**/
}
