#include "power_distortion_9000.h"

power_distortion_9000 *power_distortion_9000_new()
{
    power_distortion_9000 *x = (power_distortion_9000 *)malloc(sizeof(power_distortion_9000));
    x->level = 0.5;
    return x;
}

void power_distortion_9000_free(power_distortion_9000 *x)
{
    free(x);
}

void power_distortion_9000_setLevel(power_distortion_9000 *x, float level)
{
    x->level = level;
}

void power_distortion_9000_perform(power_distortion_9000 *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize)
{
    int i = 0;
    while(i < vectorSize)
    {
        *out++ = *in++ * x->level;
        i++;
        printf("%f", *out);
       
    }
}
