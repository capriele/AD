#ifndef CONTROLLERPODORIENT_H
#define CONTROLLERPODORIENT_H

#include "../../framework/framework.hpp"

//Controller parameters
#define Py 0.08
#define Pp 0.4
#define Pr 0.4

#define Dy 0.06
#define Dp 0.05
#define Dr 0.05

#define Pz 4
#define Dz 3

//Geometry and Thrust coefficients for PDcontroller (transforming ref Torque into omega), from controlHelperParams.Q2Ts
//F1 = aTotalThrust*TotalThrust + aTauyaw*TauYaw - aTauPR*TauPitch - aTauPR*TauRoll
#define ATOTALTHRUST 0.25
#define ATAUYAW 35.2669
#define ATAUPR 2.1427

#define THRUST2OMEGA2 455390

class controllerPDOrient_t : public podBase_t {
  
public: 

  std::string stateVariancesChannel;

  //Pod-specific members to store received messages
  agile::poseRef_t   poseRef;
  agile::powerAdjust_t   powerAdjust;	
  agile::stateVariances_t     stateVariances;

  //constructor
  controllerPDOrient_t (string podName, int64_t callInterval) : podBase_t(podName,callInterval)
	{	
	//Pod-specific initialization
	//...  	
	}

  //Pod-specifc members and functions for computations and storing results over more than 1 function cycle call
  agile::motorsWsRef_t motorsWsRef;

};








#endif
