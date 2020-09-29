/**
 * @file lj_bandpass.h
 * @author Thomas Resch <br>
 * Audiocommunication Group, Technical University Berlin <br>
 * University of Applied Sciences Nordwestschweiz (FHNW), Music-Academy, Research and Development <br>
 * A very advanced gain plugin <br>
 * <br>
 * @brief Audio Object for adjusting the volume<br>
 * <br>
 * lj_bandpass allows for adjusting the level<br>
 * of any incoming audio signal. <br>
 * <br>
 */

#ifndef lj_bandpass_h
#define lj_bandpass_h

#include <stdio.h>
#include <stdlib.h>
#include "lj_defines.h"

/** Defines whether the input vector should be single or double precision */
#define STP_INPUTVECTOR_USEFLOAT
/** Defines whether the output vector should be single or double precision */
#define STP_OUTPUTVECTOR_USEFLOAT

/**
 * @struct lj_bandpass
 * @brief A structure for a bandpass object <br>
 * The data structure impements the parameters of 2nd order Linkwitz-Riley<br>
 * highpass and lowpass filters
 */

typedef struct lj_bandpass
{
    /**contains coefficients and parameters of highpass (hp ) and lowpass (lp)*/
    
    float hp_f_c;   /**cutoff frequency*/
    float hp_theta_c;
    float hp_omega_c;
    float hp_kappa;
    float hp_delta;
    float hp_a[3];  /**feedforward*/
    float hp_b[2];  /**feedback*/
    
    float lp_f_c;   /**cutoff frequency*/
    float lp_theta_c;
    float lp_omega_c;
    float lp_kappa;
    float lp_delta;
    float lp_a[3];  /**feedforward*/
    float lp_b[2];  /**feedback*/
    
    /**also contains delayline for feedback */
    float hp_z0;
    float hp_z1;
    float hp_z2;
    
    float lp_z0;
    float lp_z1;
    float lp_z2;
    
} LJ_bandpass;

/**
 * @related lj_bandpass
 * @brief Creates a new bandpass object<br>
 * The function sets the parameters of <br>
 * the bandpass class
 * @return a pointer to the newly created lj_bandpass object <br>
 */

LJ_bandpass *lj_bandpass_new();

/**
 * @related lj_bandpass
 * @brief Frees a bandpass object<br>
 * @param x My bandpass object <br>
 * The function frees the allocated memory<br>
 * of a bandpass object
 */

void lj_bandpass_free(LJ_bandpass *x);

/**
* @related lj_bandpass
* @brief sets highpass cutoff<br>
*/

void lj_bandpass_setHighCutoff(LJ_bandpass *x, float hp_fc);

/**
* @related lj_bandpass
* @brief sets lowpass cutoff<br>
*/

void lj_bandpass_setLowCutof(LJ_bandpass *x, float lp_fc);

void lj_bandpass_setLowCutoff(LJ_bandpass *x, float lp_fc);

/**
 * @related lj_bandpass
 * @brief Performs the application og the filter to each sample in realtime <br>
 * @param x My bandpass object <br>
 * @param in The input vector <br>
 * @param out The output vector <br>
 * @param vectorSize The vectorSize <br>
 * The function lj_bandpass_perform performs the application of the filter to each sample of<br>
 * the incoming signal and copies the result to the output vector <br>
 */

void lj_bandpass_perform(LJ_bandpass *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize);

#endif /* lj_bandpass_h */
