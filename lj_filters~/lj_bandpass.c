#include "lj_bandpass.h"
#include <math.h>
#include <stdlib.h>

#define F_S 44100


LJ_bandpass *lj_bandpass_new()
{
    LJ_bandpass *x = (LJ_bandpass *)malloc(sizeof(LJ_bandpass));
    
    x->hp_f_c = 30;
    x->lp_f_c = 22000;
    
    x->hp_theta_c = M_PI * x->hp_f_c / F_S;
    x->hp_omega_c = M_PI * x->hp_f_c;
    x->hp_kappa = x->hp_omega_c/tan(x->hp_theta_c);
    x->hp_delta = pow(x->hp_kappa,2) + pow(x->hp_omega_c,2) + 2*x->hp_kappa*x->hp_omega_c;
    x->hp_a[0] = pow(x->hp_kappa,2)/x->hp_delta;
    x->hp_a[1] = (-2*pow(x->hp_kappa,2))/x->hp_delta;
    x->hp_a[2] = pow(x->hp_kappa,2)/x->hp_delta;
    x->hp_b[0] = (-2*pow(x->hp_kappa,2)+2*pow(x->hp_omega_c,2))/x->hp_delta;
    x->hp_b[1] = (-2*x->hp_kappa*x->hp_omega_c + pow(x->hp_kappa,2) + pow(x->hp_omega_c,2))/x->hp_delta;
    
    
    x->lp_theta_c = M_PI*x->lp_f_c/F_S;
    x->lp_omega_c = M_PI*x->lp_f_c;
    x->lp_kappa = x->lp_omega_c/tan(x->lp_theta_c);
    x->lp_delta = pow(x->lp_kappa,2) + pow(x->lp_omega_c,2) + 2*x->lp_kappa*x->lp_omega_c;
    x->lp_a[0] = pow(x->lp_omega_c,2)/x->lp_delta;
    x->lp_a[1] = 2*pow(x->lp_omega_c,2)/x->lp_delta;
    x->lp_a[2] = pow(x->lp_omega_c,2)/x->lp_delta;
    x->lp_b[0] = (-2*pow(x->lp_kappa,2)+2*pow(x->lp_omega_c,2))/x->lp_delta;
    x->lp_b[1] = (-2*x->lp_kappa*x->lp_omega_c + pow(x->lp_kappa,2) + pow(x->lp_omega_c,2))/x->lp_delta;
    
    /**initializing delay line with zeros*/
    
    x->hp_z0 = 0.0;
    x->hp_z1 = 0.0;
    x->hp_z2 = 0.0;
    
    x->lp_z0 = 0.0;
    x->lp_z1 = 0.0;
    x->lp_z2 = 0.0;
    
    return x;
}

void lj_bandpass_free(LJ_bandpass *x)
{
    free(x);
}

void lj_bandpass_setHighCutoff(LJ_bandpass *x, float hp_fc)
{
    x->hp_f_c = hp_fc;
}

void lj_bandpass_setLowCutoff(LJ_bandpass *x, float lp_fc)
{
    x->lp_f_c = lp_fc;
}

void lj_bandpass_perform(LJ_bandpass *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize)
{
    /**sending signal through highpass first*/
    
    int i = 0;
    while(i++ < vectorSize)
    {
        x->hp_z0 = *in++ + (x->hp_b[0] * x->hp_z1) + (x->hp_b[1] * x->hp_z2);
        *out++ = (x->hp_a[0] * x->hp_z1) + (x->hp_a[1] * x->hp_z1) + (x->hp_a[2] * x->hp_z2);
        
        /**updating delayline*/
        x->hp_z2 = x->hp_z1;
        x->hp_z1 = x->hp_z0;
       
    }
    
    /**then through lowpass*/
    
    int j = 0;
    while(j++ < vectorSize)
    {
        x->lp_z0 = *in++ + (x->lp_b[0] * x->lp_z1) + (x->lp_b[1] * x->lp_z2);
        *out++ = (x->lp_a[0] * x->lp_z1) + (x->lp_a[1] * x->lp_z1) + (x->lp_a[2] * x->lp_z2);
        
        /**updating delayline*/
        x->lp_z2 = x->lp_z1;
        x->lp_z1 = x->lp_z0;
       
    }
}
