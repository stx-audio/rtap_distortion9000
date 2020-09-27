#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define F_S 44100



typedef struct lowpass {
    float f_c;
    float theta_c;
    float omega_c;
    float kappa;
    float delta;
    float a[3];
    float b[2];
} Lowpass;

typedef struct highpass {
    float f_c; 
    float theta_c;
    float omega_c; 
    float kappa;
    float delta; 
    float a[3];
    float b[2];
} Highpass;


void calc_lowpass (Lowpass* lp){

    lp->theta_c = M_PI*lp->f_c/F_S;
    lp->omega_c = M_PI*lp->f_c;
    lp->kappa = lp->omega_c/tan(lp->theta_c);
    lp->delta = pow(lp->kappa,2) + pow(lp->omega_c,2) + 2*lp->kappa*lp->omega_c;
    lp->a[0] = pow(lp->omega_c,2)/lp->delta;
    lp->a[1] = 2*pow(lp->omega_c,2)/lp->delta; 
    lp->a[2] = pow(lp->omega_c,2)/lp->delta;
    lp->b[0] = (-2*pow(lp->kappa,2)+2*pow(lp->omega_c,2))/lp->delta;
    lp->b[1] = (-2*lp->kappa*lp->omega_c + pow(lp->kappa,2) + pow(lp->omega_c,2))/lp->delta;

}

void calc_highpass (Highpass* hp){

    hp->theta_c = M_PI * hp->f_c / F_S;
    hp->omega_c = M_PI * hp->f_c;
    hp->kappa = hp->omega_c/tan(hp->theta_c);
    hp->delta = pow(hp->kappa,2) + pow(hp->omega_c,2) + 2*hp->kappa*hp->omega_c;
    hp->a[0] = pow(hp->kappa,2)/hp->delta;
    hp->a[1] = (-2*pow(hp->kappa,2))/hp->delta; 
    hp->a[2] = pow(hp->kappa,2)/hp->delta;
    hp->b[0] = (-2*pow(hp->kappa,2)+2*pow(hp->omega_c,2))/hp->delta;
    hp->b[1] = (-2*hp->kappa*hp->omega_c + pow(hp->kappa,2) + pow(hp->omega_c,2))/hp->delta;

}

void hp_process(float* inSample, float* outSample, Highpass hp, float* w){
    w[0] = inSample + (hp->b[0] * w[1]) + (hp->b[1] * w[2]);
    outSample = (hp->a[0] * w[0]) + (hp->a[1] * w[1]) + (hp->a[2] * w[2]);
    w[2] = w[1]; 
    w[1] = w[0];
}

int main (){
    
    float signal[6] = {1.2, 2.2 ,3.2 ,6.4 ,2.4 ,7.5};
    float delayline[3] = {0,0,0};
    float output[6] = {0,0,0,0,0,0};

    Lowpass lp;
    Highpass hp;
     
    printf("fc für lowpass eingeben: \n");
    scanf("%f", &lp.f_c);
    calc_lowpass(&lp);
    printf("fc für highpass eingeben: \n");
    scanf("%f", &hp.f_c);
    calc_highpass(&hp);
    
    printf("%f\n", lp.a[0]);
    printf("%f\n", lp.a[1]);
    printf("%f\n", lp.a[2]);
    printf("%f\n", lp.b[0]);
    printf("%f\n", lp.b[1]);

    printf("a[0]: %f\n", hp.a[0]);
    printf("a[1]: %f\n", hp.a[1]);
    printf("a[2]: %f\n", hp.a[2]);
    printf("b[0]: %f\n", hp.b[0]);
    printf("b[1]: %f\n", hp.b[1]);

    for (int i = 0; i<6; i++) hp_process(&signal[i], &output[i], hp, delayline);
    
    printf("signal: ");
    for (int i = 0; i<6; i++)printf("%f", signal[i]);
    printf("processed signal: ");
    for (int i = 0; i<6; i++) printf("%f", output[i]);


    
    
}
