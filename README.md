# RTAP Distortion 9000

## Welcome to our pure data external!

This external was developed during the course "Real Time Audio Programming in C" at Berlin Institute of Technology.

The project contains the distortion external, an example patch with a filter bank and a doxygen documentation if you want to understand our code.

How to use the external on macOs:
Copy the external in your application folder "/Applications/Pd-0.xx-x.app/Contents/Resources/extra".
The x is your version number.

Then you can access this external by making a new object and type in "rtap_distortion9000~".

You will have the following inputs:
1. Distortion Algorithm, Value: 0-16, use it to change the algorithm
2. Dry/Wet, Value: 0-1, use it to change the mix between original signal and distorted signal
3. Saturation, Value: 1-10, use it to change the amount of distortion applied to the algorithm (only for algo 2,4,5,6)
4. Frequency, Value 10 - 22 kHz, use it to change the frequency of the bandpass filter
5. Quality, Value 0.1 - 16, use it to change the Q of the bandpass filter
6. Gain, Value 0 - 30, use it to change the gain of the bandpass filter

## The Distortion Algorithms:

### Soft Clip Sigmoid Waveshaper:
0. Arraya (ARRY)
1. Sigmoid (SIG)
2. Sigmoid 2 (SIG2)
3. Hyperbolic Tangent (TANH)
4. Arctangent (ATAN)
5. Fuzz Exponential 1 (FEXP1)

### Exotic Distortion Algorithms:
6. Fuzz Exponential 2 (FEXP2)
7. Exponential (EXP)
8. Arctangent Square Root (ATSR)
9. Square Sign (SQS)
10. Cube (CUBE)
11. Hard Clipper (HCLIP)
12. Half Wave Rectifier (HWR)
13. Full Wave Rectifier (FWR)
14. Square Law (SQR)
15. Absolute Square Root (ASQRT)


## Signal Flow

<img src="signal-flow.png"
     alt="Signal Flow"
     width="100" />

Kind regards,
Lynn, Felix & Stephan

PS: Thanks to our lecturer Markus Hädrich and Thomas Resch