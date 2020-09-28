/**
 * @file lfs_dist_dsp.h
 * @author Lynn, Felix, Stephan <br>
 * Audiocommunication Group, Technical University Berlin <br>
 * University of Applied Sciences Nordwestschweiz (FHNW), Music-Academy, Research and Development <br>
 * An exemplary testing unit for the rtap_distortion9000 object. <br>
 * It creates a known signal, alternating from -2 to 2 <br>
 * and then checks whether the output signal holds the correct values<br>
 * for the given exemplary parameters drywet= 1 and exotic distortion = 1.<br>
 * It is called from the rtap_distortion9000~ object inside pure data with<br>
 * the message "integrationtest".<br>
 * <br>
 * @brief Unit and Integration Test for rtap_distortion9000<br>
 * <br>
 * Performs Unit Tests for lfs_dist_dsp_setDryWet, lfs_dist_dsp_setDistortionMod<br>
 * and lfs_dist_dsp_setSaturation plus an Integration Test for lfs_dist_process.<br>
 * <br>
 */

#ifndef lfs_dist_process_unittest_h
#define lfs_dist_process_unittest_h

#include "lfs_dist_dsp.h"

typedef struct lfs_dist_dsp_test
{
    lfs_dist_dsp *dist_process2Test;
    int vectorSize;
    int testSignalLength;
    int numberOfVectors2Test;
    lfs_INPUTVECTOR *inputSignal;
    lfs_OUTPUTVECTOR *outputSignal;
    
}   lfs_dist_dsp_test;

lfs_dist_dsp_test *lfs_dist_dsp_test_new(lfs_dist_dsp *dist_process2Test, int vectorSize, int testSignalLength);

void lfs_dist_dsp_test_free(lfs_dist_dsp_test *x);

void lfs_dist_dsp_test_integration(lfs_dist_dsp_test *x);

#endif /* lfs_dist_process_unittest */
