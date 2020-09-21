#include "stp_dist_dsp.h"
#include <math.h>
stp_dist_dsp *stp_dist_dsp()
{
	stp_dist_dsp *x = (stp_dist_dsp *)malloc(sizeof(stp_dist_dsp));
    x->dryWet = 0.0;
    x->clippingLevel= 0.0;
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

void stp_dist_dsp_setClippingLevel(stp_dist_dsp *x, float clippingLevel)
{
    x->clippingLevel = clippingLevel;
}

void stp_dist_dsp_dryWet(stp_dist_dsp *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize)
{
    int i = 0;
    while(i < vectorSize)
    {
    	float inputsig  = *in++;
        *out++ = (inputsig * (1-(x->dryWet))) + ((inputsig * inputsig * inputsig) * x->dryWet);
        i++;
    }
}

void stp_dist_dsp_clip(stp_dist_dsp *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize)
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

void stp_dist_dsp_perform(stp_dist_dsp *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize)
{
	stp_dist_dsp_dryWet(x, in, out, vectorSize);
	stp_dist_dsp_clip(x, in, out, vectorSize);
}
