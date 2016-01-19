#ifndef CONTROLLERPODORIENTPARAMS_H
#define CONTROLLERPODORIENTPARAMS_H


//Controller parameters
#define Py 0.08
#define Pp 0.4
#define Pr 0.4

#define Dy 0.06
#define Dp 0.05
#define Dr 0.05


//Geometry and Thrust coefficients for PDcontroller (transforming ref Torque into omega), from controlHelperParams.Q2Ts //@TODO Compute from params in params.hpp file not from matlab computation!
//F1 = aTotalThrust*TotalThrust + aTauyaw*TauYaw - aTauPR*TauPitch - aTauPR*TauRoll
#define ATOTALTHRUST 0.25
#define ATAUYAW 35.2669
#define ATAUPR 2.1427


#endif
