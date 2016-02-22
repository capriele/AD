#ifndef PARAMSREMOTECONTROLLER_H
#define PARAMSREMOTECONTROLLER_H

//powerAdjustment (Gain) Motorcommander
#define INITPOWERGAIN 50
#define MINPOWERGAIN 10
#define MAXPOWERGAIN 150
#define STEPSPOWERGAIN 5

//total thrust bias PD-Orient-controller
#define INITBIASPDO -6.0
#define MINTBIASPDO -6.0
#define MAXTBIASPDO 4.0
#define STEPSTBIASPDO 0.5

//yaw  bias (via added value) PD-Orient-controller //@TODO correct minmaxsteps
#define INITYBIASPDO 0.0
#define MINYBIASPDO -3.0
#define MAXYBIASPDO 3.0
#define STEPSYBIASPDO 0.2

//pitch bias (via added value) PD-Orient-controller //@TODO correct minmaxsteps
#define INITPBIASPDO 0
#define MINPBIASPDO -3.0
#define MAXPBIASPDO 3.0
#define STEPSPBIASPDO 0.1

//roll bias (via added value) PD-Orient-controller //@TODO correct minmaxsteps
#define INITRBIASPDO 0
#define MINRBIASPDO -3.0
#define MAXRBIASPDO 3.0
#define STEPSRBIASPDO 0.1

//PD-gain tuner
#define INITPDGAINADJUST 0.1
#define MINPDGAINADJUST 0.1
#define MAXPDGAINADJUST 2.0
#define STEPSPDGAINADJUST 0.05




//reference values
#define STEPSALTITUDE 0.2 //[m]
#define STEPSPITCHROLL 0.02 //[euler]
#define STEPSYAW 0.05 //[euler]

//epsilon
#define EPSREMOTE 0.00001

#endif
