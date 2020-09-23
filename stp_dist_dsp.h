/**
 * @file stp_dist_dsp.h
 * @author Felix, Stephan, Lynn br>
 * Audiocommunication Group, Technical University Berlin <br>
 * University of Applied Sciences Nordwestschweiz (FHNW), Music-Academy, Research and Development <br>
 * A very advanced gain plugin <br>
 * <br>
 * @brief Audio Object for a multiband distortion <br>
 * <br>
 * stp_brickwall allows for selecting the type of distortion and a drywet input<br>
 * of any incoming audio signal. <br>
 * <br>
 */

#ifndef stp_dist_dsp_h
#define stp_dist_dsp_h

#include <stdio.h>
#include <stdlib.h>


typedef float STP_INPUTVECTOR;
typedef float STP_OUTPUTVECTOR;

/**
 * @struct stp_dist_dsp
 * @brief A structure for a multiband distortion object <br>
 * @var stp::level The parameter value for adjusting the <br>
 * level of the incoming signal
 */

typedef struct stp_dist_dsp
{
    float dryWet; /**< parameter for adjusting amount of distortion for the incoming signal */
    float distortionMod; /**< parameter for choosing the desired distortion mod */
} stp_dist_dsp;

/**
 * @related stp_dist_dsp
 * @brief Creates a new dist_dsp object<br>
 * The function creates a new stp_dist_dsp object <br>
 * @return a pointer to the newly created stp_dist_dsp object <br>
 */

stp_dist_dsp *stp_dist_dsp_new();

/**
 * @related stp_dist_dsp
 * @brief Frees a stp_dist_dsp object<br>
 * @param x My dist_dsp object to delete.<br>
 * The function frees the allocated memory<br>
 * of a gain object
 */

void stp_dist_dsp_free(stp_dist_dsp *x);

/**
 * @related stp_dist_dsp
 * @brief Sets the drywet parameter <br>
 * @param x My stp_dist_dsp object <br>
 * @param dryWet The dryWet value <br>
 * The function sets the dryWet parameter of <br>
 * the gain class
 */

void stp_dist_dsp_setdryWet(stp_dist_dsp *x, float dryWet);

/**
 * @related stp_dist_dsp
 * @brief Sets the clippingLevel parameter <br>
 * @param x My stp_brickwall object <br>
 * @param clippingLevel The clippingLevel value <br>
 * The function sets the clippingLevel parameter of <br>
 * the gain class
 */

void stp_dist_dsp_setdistortionMod(stp_dist_dsp *x, float distortionMod);

/**
 * @related stp_dist_dsp
 * @brief Performs the drywet ratio of the signal in realtime <br>
 * @param x My gain object <br>
 * @param in The input vector <br>
 * @param out The output vector <br>
 * @param vectorSize The vectorSize <br>
 * The function stp_brickwall_makeUp multiplies the input signal <br>
 * with the parameter gain. <br>
 */

void stp_dist_dsp_dryWet(stp_dist_dsp *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize);

/**
 * @related stp_dist_dsp
 * @brief Performs the clipping in realtime <br>
 * @param x My gain object <br>
 * @param in The input vector <br>
 * @param out The output vector <br>
 * @param vectorSize The vectorSize <br>
 * The function stp_brickwall_clip sets every value of the input signal <br>
 * thats either greater then the parameter clippingLevel or smaller then the <br>
 * parameter -clippingLevel to either clippingLevel or -clippingLevel. <br>
 */

void stp_dist_dsp_distortionMod(stp_dist_dsp *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize);

/**
 * @related stp_brickwall
 * @brief Performs the level adjustment in realtime <br>
 * @param x My gain object <br>
 * @param in The input vector <br>
 * @param out The output vector <br>
 * @param vectorSize The vectorSize <br>
 * The function stp_brickwall_perform performs the gain adjustment of <br>
 * the incoming signal and copies the result to the output vector <br>
 */

void stp_dist_dsp_perform(stp_dist_dsp *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize);

#endif /* stp_brickwall_h */