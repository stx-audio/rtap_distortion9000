/**
 * @file lfs_dist_dsp.h
 * @author Lynn, Felix, Stephan <br>
 * Audiocommunication Group, Technical University Berlin <br>
 * @brief Audio object for distortion <br>
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
 * @param mix parameter for storing amount of distortion for the incoming signal<br>
 * @param distortionMod parameter for storing the desired distortion mod <br>
 * @param saturation parameter for storing saturation for saturation mods 2 (SIG), 4 (TANH), 5 (ATAN) and 6 (FEXP1)<br>
 * @param frequency parameter to store center frequency of bandpass filter
 * @param quality parameter to store q value of the bandpass filter
 * @param gain parameter to set gain at center frequency of the bandpass filter
 */
typedef struct lfs_dist_dsp
{
    float mix;
    float distortionMod;
    float saturation;
    float frequency;
    float quality;
    float gain;
    
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
 * @related lfs_dist_dsp_setMix
 * @brief Sets the dry wet value of the pd project. <br>
 * @param x A pointer to the lfs_dist_dsp object <br>
 * @param mix the mix value 0-1<br>
 */
void lfs_dist_dsp_setMix(lfs_dist_dsp *x, float mix);

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
 * @param saturation sets the saturation 1-10 of the saturation for algo 2,4,5,6 <br>
 */
void lfs_dist_dsp_setSaturation(lfs_dist_dsp *x, float saturation);

/**
 * @related lfs_dist_dsp_setFrequency
 * @brief Sets the saturation of the pure data project <br>
 * @param x A pointer to the lfs_dist_dsp object <br>
 * @param frequency sets the center frequency of the bandpass filter between 50 and 20000 Hz <br>
 */
void lfs_dist_dsp_setFrequency(lfs_dist_dsp *x, float frequency);

/**
 * @related lfs_dist_dsp_setQuality
 * @brief Sets the q value of the bandpass filter <br>
 * @param x A pointer to the lfs_dist_dsp object <br>
 * @param quality sets the q value of the bandpass filter between 0 and 16 <br>
 */
void lfs_dist_dsp_setQuality(lfs_dist_dsp *x, float quality);

/**
 * @related lfs_dist_dsp_setGain
 * @brief Sets gain at center frequency of the bandpass filter <br>
 * @param x A pointer to the lfs_dist_dsp object <br>
 * @param gain sets gain at center frequency of the bandpass filter <br>
 */
void lfs_dist_dsp_setGain(lfs_dist_dsp *x, float gain);

/**
 * @related sgn
 * @brief Adds lfs_dist_dsp_perform to the signal chain. <br>
 * @param input the input sample that will be calculated<br>
 * @return the sign function, value will always be -1, 0 or 1<br>
 */
float sgn(float input);

/**
 * @related dry_wet_process
 * @brief Adds dry_wet_processto the signal chain. <br>
 * @param inSample A pointer to the input sample<br>
 * @param outSample A pointer to the output sample<br>
 * @param mix A pointer to the dry wet value as set in the pd patch<br>
 */
void dry_wet_process(float* inSample, float* outSample, float* mix);

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
 * @related lfs_dist_dsp_perform
 * @brief Performs filtering and distortion of the signal in realtime <br>
 * @param x distortion object <br>
 * @param in The input vector <br>
 * @param out The output vector <br>
 * @param vectorSize The vectorSize <br>
 * The function lfs_dist_dsp_perform performs filtering and distortion of <br>
 * the incoming signal and copies the result to the output vector <br>
 */
void lfs_dist_dsp_perform(lfs_dist_dsp *x, lfs_INPUTVECTOR *in, lfs_OUTPUTVECTOR *out, int vectorSize);

#endif /* lfs_dist_dsp_h */
