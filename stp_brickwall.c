#include "stp_brickwall.h"
#include <math.h>
stp_brickwall *stp_brickwall_new()
{
    stp_brickwall *x = (stp_brickwall *)malloc(sizeof(stp_brickwall));
    x->dryWet = 0.0;
    x->clippingLevel= 0.0;
    return x;
}

void stp_brickwall_free(stp_brickwall *x)
{
    free(x);
}

void stp_brickwall_setdryWet(stp_brickwall *x, float dryWet)
{
    x->dryWet = dryWet;
}

void stp_brickwall_setClippingLevel(stp_brickwall *x, float clippingLevel)
{
    x->clippingLevel = clippingLevel;
}

void stp_brickwall_dryWet(stp_brickwall *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize)
{
    int i = 0;
    while(i < vectorSize)
    {
    	float inputsig  = *in++;
        *out++ = (inputsig * (1-(x->dryWet))) + ((inputsig * inputsig * inputsig) * x->dryWet);
        i++;
    }
}

void stp_brickwall_clip(stp_brickwall *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize)
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

void stp_brickwall_perform(stp_brickwall *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize)
{
    stp_brickwall_dryWet(x, in, out, vectorSize);
    stp_brickwall_clip(x, in, out, vectorSize);
}
