/**
 * @file power_distortion_9000.h
 * @author Thomas Resch <br>
 * Audiocommunication Group, Technical University Berlin <br>
 * University of Applied Sciences Nordwestschweiz (FHNW), Music-Academy, Research and Development <br>
 * A very advanced gain plugin <br>
 * <br>
 * @brief Audio Object for adjusting the volume<br>
 * <br>
 * power_distortion_9000 allows for adjusting the level<br>
 * of any incoming audio signal. <br>
 * <br>
 */

#ifndef power_distortion_9000_h
#define power_distortion_9000_h

#include <stdio.h>
#include <stdlib.h>
#include "stp_defines.h"

/** Defines whether the input vector should be single or double precision */
#define STP_INPUTVECTOR_USEFLOAT
/** Defines whether the output vector should be single or double precision */
#define STP_OUTPUTVECTOR_USEFLOAT

/**
 * @struct power_distortion_9000
 * @brief A structure for a gain object <br>
 * @var stp::level The parameter value for adjusting the <br>
 * level of the incoming signal
 */

typedef struct power_distortion_9000
{
    t_object  x_obj;
    t_sample f;
    stp_brickwall *brickwall;
    t_outlet *x_out;
} power_distortion_9000;

/**
 * @related power_distortion_9000
 * @brief Creates a new gain object<br>
 * The function sets the level parameter of <br>
 * the gain class
 * @return a pointer to the newly created power_distortion_9000 object <br>
 */

power_distortion_9000 *power_distortion_9000_new();

/**
 * @related power_distortion_9000
 * @brief Frees a gain object<br>
 * @param x My gain object <br>
 * The function frees the allocated memory<br>
 * of a gain object
 */

void power_distortion_9000_free(power_distortion_9000 *x);

/**
 * @related power_distortion_9000
 * @brief Sets the gain parameter <br>
 * @param x My gain object <br>
 * @param level The gain value <br>
 * The function sets the level parameter of <br>
 * the gain class
 */

void power_distortion_9000_setLevel(power_distortion_9000 *x, float level);

/**
 * @related power_distortion_9000
 * @brief Performs the level adjustment in realtime <br>
 * @param x My gain object <br>
 * @param in The input vector <br>
 * @param out The output vector <br>
 * @param vectorSize The vectorSize <br>
 * The function power_distortion_9000_perform performs the gain adjustment of <br>
 * the incoming signal and copies the result to the output vector <br>
 */

void power_distortion_9000_perform(power_distortion_9000 *x, STP_INPUTVECTOR *in, STP_OUTPUTVECTOR *out, int vectorSize);

#endif /* power_distortion_9000_h */
