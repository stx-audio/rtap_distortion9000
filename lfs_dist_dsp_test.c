

#include "lfs_dist_dsp_test.h"
#include "m_pd.h"

lfs_dist_dsp_test *lfs_dist_dsp_test_new(lfs_dist_dsp *dist_process2Test, int vectorSize, int testSignalLength)
{
	lfs_dist_dsp_test *x = (lfs_dist_dsp_test *)malloc(sizeof(lfs_dist_dsp_test));
    x->dist_process2Test = dist_process2Test;
    x->testSignalLength = testSignalLength;
    x->vectorSize = vectorSize;
    x->numberOfVectors2Test = testSignalLength/vectorSize;
    x->inputSignal = (lfs_INPUTVECTOR *)malloc(testSignalLength * sizeof(lfs_INPUTVECTOR));
    x->outputSignal = (lfs_OUTPUTVECTOR *)malloc(testSignalLength * sizeof(lfs_OUTPUTVECTOR));

    return x;
}

void lfs_dist_dsp_test_free(lfs_dist_dsp_test *x)
{
    free(x->inputSignal);
    free(x->outputSignal);
    free(x);
}

void lfs_dist_dsp_test_createInputSignal(lfs_dist_dsp_test *x)
{
    for(int i = 0; i < x->testSignalLength; i++)
    {
        if(i % 2)
            x->inputSignal[i] = 2;
        else
            x->inputSignal[i] = -2;
    }
}

int lfs_dist_dsp_test_unitdryWet(lfs_dist_dsp_test *x)
{
    int unitTestFailed = 0;
    lfs_dist_dsp_setDryWet(x->dist_process2Test, 1);
    
    for(int i = 0; i < x->numberOfVectors2Test; i++)
    {
        lfs_INPUTVECTOR *inputSignalPtr = x->inputSignal + i * x->vectorSize;
        lfs_OUTPUTVECTOR *outputSignalPtr = x->outputSignal + i * x->vectorSize;
        lfs_dist_process(x->dist_process2Test, inputSignalPtr, outputSignalPtr, x->vectorSize);
    }
    
    for(int i = 0; i < x->testSignalLength; i++)
    {
        if(i % 2)
        {
            post("Output at index %d is %f, should be %f", i, x->outputSignal[i], 1.0);
            if(x->outputSignal[i] != 1)
                unitTestFailed = 1;
        }
        else
        {
            post("Output at index %d is %f, should be %f", i,x->outputSignal[i], -1.0);
            if(x->outputSignal[i] != -1)
                unitTestFailed = 1;
        }
    }
    
    if(!unitTestFailed)
        post("\nUnit Test for function lfs_dist_dsp_setDryWet() succeeded!\n");
    else
        post("\nUnit Test for function lfs_dist_dsp_setDryWet() failed!\n");
    
    return unitTestFailed;
}

int lfs_dist_dsp_test_unitDistortionMod(lfs_dist_dsp_test *x)
{
    int unitTestFailed = 0;
    lfs_dist_dsp_setDistortionMod(x->dist_process2Test, 5);
    
    for(int i = 0; i < x->numberOfVectors2Test; i++)
    {
        lfs_INPUTVECTOR *inputSignalPtr = x->inputSignal + i * x->vectorSize;
        lfs_OUTPUTVECTOR *outputSignalPtr = x->outputSignal + i * x->vectorSize;
        lfs_dist_process(x->dist_process2Test, inputSignalPtr, outputSignalPtr, x->vectorSize);
    }

    for(int i = 0; i < x->testSignalLength; i++)
    {
        if(i % 2)
        {
            post("Output at index %d is %f, should be %f", i, x->outputSignal[i], 5.0);
            if(x->outputSignal[i] != 5)
                unitTestFailed = 1;
        }
        else
        {
            post("Output at index %d is %f, should be %f", i,x->outputSignal[i], -5.0);
            if(x->outputSignal[i] != -5)
                unitTestFailed = 1;
        }
    }

    if(!unitTestFailed)
        post("\nUnit Test for function lfs_dist_dsp_setDistortionMod() succeeded!\n");
    else
        post("\nUnit Test for function lfs_dist_dsp_setDistortionMod() failed!\n");

    return unitTestFailed;
}
int lfs_dist_dsp_test_unitSaturation(lfs_dist_dsp_test *x)
{
    int unitTestFailed = 0;
    lfs_dist_dsp_setSaturation(x->dist_process2Test, 8);

    for(int i = 0; i < x->numberOfVectors2Test; i++)
    {
        lfs_INPUTVECTOR *inputSignalPtr = x->inputSignal + i * x->vectorSize;
        lfs_OUTPUTVECTOR *outputSignalPtr = x->outputSignal + i * x->vectorSize;
        lfs_dist_process(x->dist_process2Test, inputSignalPtr, outputSignalPtr, x->vectorSize);
    }
    
    for(int i = 0; i < x->testSignalLength; i++)
    {
        if(i % 2)
        {
            post("Output at index %d is %f, should be %f", i, x->outputSignal[i], 8.0);
            if(x->outputSignal[i] != 8)
                unitTestFailed = 1;
        }
        else
        {
            post("Output at index %d is %f, should be %f", i,x->outputSignal[i], -8.0);
            if(x->outputSignal[i] != -8)
                unitTestFailed = 1;
        }
    }
    
    if(!unitTestFailed)
        post("\nUnit Test for function lfs_dist_dsp_setSaturation() succeeded!\n");
    else
        post("\nUnit Test for function lfs_dist_dsp_setSaturation() failed!\n");
    
    return unitTestFailed;
}
void lfs_dist_dsp_test_integration(lfs_dist_dsp_test *x)
{
    int integrationTestFailed = 0;
    
    lfs_dist_dsp_test_createInputSignal(x);
    
    integrationTestFailed |= lfs_dist_dsp_test_unitdryWet(x);
    integrationTestFailed |= lfs_dist_dsp_test_unitDistortionMod(x);
    integrationTestFailed |= lfs_dist_dsp_test_unitSaturation(x);
    
    if(!integrationTestFailed)
        post("\nIntegration Test for lfs_dist_dsp~ succeeded!\n");
    else
        post("\nIntegration Test for lfs_dist_dsp~ failed!\n");
}

