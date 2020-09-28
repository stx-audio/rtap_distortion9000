/**
 * @file lfs_dist_dsp.h
 * @author Lynn, Felix, Stephan <br>
 * Audiocommunication Group, Technical University Berlin <br>
 * @brief Audio Object for distortion <br>
 * <br>
 * RTAP Distortion 9000 allows to select one of 16 distortion types and choose how much distortion will be applied to the signal. It is recommended to use multiple instances with bandpass filters to get the most out of this external.
 * <br>
 */

#ifndef lfs_dist_dsp_h
#define lfs_dist_dsp_h

#include <stdio.h>
#include <stdlib.h>


typedef float lfs_INPUTVECTOR;
typedef float lfs_OUTPUTVECTOR;

/**
 * @struct lfs_dist_dsp
 * @brief A structure for a multiband distortion object <br>
 * @var dryWet parameter for adjusting amount of distortion for the incoming signal<br>
 * @var distortionMod parameter for choosing the desired distortion mod <br>
 * @var saturation parameter for setting more saturation for saturation mods 2 (SIG), 4 (TANH), 5 (ATAN) and 6 (FEXP1)<br>
 */
typedef struct lfs_dist_dsp
{
    float dryWet; /**< parameter for adjusting amount of distortion for the incoming signal */
    float distortionMod; /**< parameter for choosing the desired distortion mod */
    float saturation; /**<parameter for setting more saturation for some saturation mods 2 (SIG), 4 (TANH), 5 (ATAN) and 6 (FEXP1)*/
    int integrationstest;
} lfs_dist_dsp;

/**
 * @related lfs_dist_dsp_new
 * @brief Creates a new dist_dsp object<br>
 * The function creates a new lfs_dist_dsp object <br>
 * @return a pointer to the newly created lfs_dist_dsp object <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
lfs_dist_dsp *lfs_dist_dsp_new();

/**
 * @related lfs_dist_dsp_free
 * @brief destructor of the dsp object<br>
 * @param x A pointer to the lfs_dist_dsp object <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void lfs_dist_dsp_free(lfs_dist_dsp *x);

/**
 * @related lfs_dist_dsp_setdryWet
 * @brief Sets the dry wet value of the pd project. <br>
 * @param x A pointer to the lfs_dist_dsp object <br>
 * @param dryWet the dryWet value 0-1<br>
 */
void lfs_dist_dsp_setDryWet(lfs_dist_dsp *x, float dryWet);

/**
 * @related lfs_dist_dsp_setdistortionMod
 * @brief Sets the distortion type of the pure data project <br>
 * @param x A pointer to the lfs_dist_dsp object <br>
 * @param distortionMod sets the distortion algorithm 0-16 <br>
 */
void lfs_dist_dsp_setDistortionMod(lfs_dist_dsp *x, float distortionMod);

/**
 * @related lfs_dist_dsp_setSaturation
 * @brief Sets the saturation of the pure data project <br>
 * @param x A pointer to the lfs_dist_dsp object <br>
 * @param distortionMod sets the saturation 1-10 of the saturation for algo 2,4,5,6 <br>
 */
void lfs_dist_dsp_setSaturation(lfs_dist_dsp *x, float distortionMod);

/**
 * @related sgn
 * @brief Adds lfs_dist_dsp_perform to the signal chain. <br>
 * @param input the input sample that will be calculated<br>
 * @return the sign function, value will always be -1, 0 or 1<br>
 */
float sgn(float input);

/**
 * @related dry_wet_process
 * @brief Adds lfs_dist_dsp_perform to the signal chain. <br>
 * @param inSample A pointer to the input sample<br>
 * @param outSample A pointer to the output sample<br>
 * @param dryWet A pointer to the dry wet value as set in the pd patch<br>
 */
void dry_wet_process(float* inSample, float* outSample, float* dryWet);

/**
 * @related lfs_dist_process
 * @brief Adds the distortion to the signal, the signal processing is actually happening here<br>
 * @param x Pointer to the lfs_dist_dsp object <br>
 * @param in Pointer to the input vector <br>
 * @param out Pointer to the output vector<br>
 * @param vectorSize Length of the Vector<br>
 */
void lfs_dist_process(lfs_dist_dsp *x, lfs_INPUTVECTOR *in, lfs_OUTPUTVECTOR *out, int vectorSize);

/**
 * @related lfs_brickwall
 * @brief Performs the level adjustment in realtime <br>
 * @param x My gain object <br>
 * @param in The input vector <br>
 * @param out The output vector <br>
 * @param vectorSize The vectorSize <br>
 * The function lfs_brickwall_perform performs the gain adjustment of <br>
 * the incoming signal and copies the result to the output vector <br>
 */
void lfs_dist_dsp_perform(lfs_dist_dsp *x, lfs_INPUTVECTOR *in, lfs_OUTPUTVECTOR *out, int vectorSize);

#endif /* lfs_brickwall_h */
