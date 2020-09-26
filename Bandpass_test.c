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
    float a_0;
    float a_1; 
    float a_2;
    float b_1;
    float b_2;
} Lowpass;
/*
typedef struct highpass {
    float f_c = 0.0;
    float theta_c = M_PI*f_c/F_S;
    float omega_c = M_PI*f_c;
    float kappa = omega_c/tan(theta_c);
    float delta = pow(kappa) + pow(omega_c) + 2*kappa*omega_c;
    float a_0 = pow(kappa)/delta;
    float a_1 = (-2*pow(kappa))/delta; 
    float a_2 = pow(kappa)/delta;
    float b_1 = (-2*pow(kappa)+2*pow(omega_c))/delta;
    float b_2 = (-2*kappa*omega_c + pow(kappa) + pow(omega_c))/delta
} Highpass;
*/

int main (){
    
    Lowpass instance_lowpass;
     
    printf("fc für lowpass eingeben: \n");
    scanf("%f", &instance_lowpass.f_c);
    instance_lowpass.theta_c = M_PI*instance_lowpass.f_c/F_S;
    instance_lowpass.omega_c = M_PI*instance_lowpass.f_c;
    instance_lowpass.kappa = instance_lowpass.omega_c/tan(instance_lowpass.theta_c);
    instance_lowpass.delta = pow(instance_lowpass.kappa,2) + pow(instance_lowpass.omega_c,2) + 2*instance_lowpass.kappa*instance_lowpass.omega_c;
    instance_lowpass.a_0 = pow(instance_lowpass.omega_c,2)/instance_lowpass.delta;
    instance_lowpass.a_1 = 2*pow(instance_lowpass.omega_c,2)/instance_lowpass.delta; 
    instance_lowpass.a_2 = pow(instance_lowpass.omega_c,2)/instance_lowpass.delta;
    instance_lowpass.b_1 = (-2*pow(instance_lowpass.kappa,2)+2*pow(instance_lowpass.omega_c,2))/instance_lowpass.delta;
    instance_lowpass.b_2 = (-2*instance_lowpass.kappa*instance_lowpass.omega_c + pow(instance_lowpass.kappa,2) + pow(instance_lowpass.omega_c,2))/instance_lowpass.delta;
    printf("%f\n", instance_lowpass.a_0);
    printf("%f\n", instance_lowpass.a_1);
    printf("%f\n", instance_lowpass.a_2);
    printf("%f\n", instance_lowpass.b_1);
    printf("%f\n", instance_lowpass.b_2);
    

}


// float f_c = 0.0;
    // float theta_c = M_PI*f_c/F_S;
    // float omega_c = M_PI*f_c;
    // float kappa = omega_c/tan(theta_c);
    // float delta = pow(kappa) + pow(omega_c) + 2*kappa*omega_c;
    // float a_0 = pow(omega_c)/delta;
    // float a_1 = 2*pow(omega_c)/delta; 
    // float a_2 = pow(omega_c)/delta;
    // float b_1 = (-2*pow(kappa)+2*pow(omega_c))/delta;
    // float b_2 = (-2*kappa*omega_c + pow(kappa) + pow(omega_c))/delta