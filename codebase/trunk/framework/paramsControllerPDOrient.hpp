#ifndef CONTROLLERPODORIENTPARAMS_H
#define CONTROLLERPODORIENTPARAMS_H


//Controller parameters

#define Py 0.08
#define Pp 0.32
#define Pr 0.32

#define Dy 0.06
#define Dp 0.075
#define Dr 0.075


//static error gain test of PWM seems to yield same results as flight controller
/*
#define Py 0.08
#define Pp 1.3
#define Pr 1.3

#define Dy 0.06
#define Dp 0.18
#define Dr 0.18
*/

//Geometry and Thrust coefficients for PDcontroller (transforming ref Torque into omega), from controlHelperParams.Q2Ts //@TODO Compute from params in params.hpp file not from matlab computation!
//F1 = aTotalThrust*TotalThrust + aTauyaw*TauYaw - aTauPR*TauPitch - aTauPR*TauRoll
#define ATOTALTHRUST 0.25
#define ATAUYAW 35.2669
#define ATAUPR 2.1427


#endif
