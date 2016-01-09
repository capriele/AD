#ifndef CONTROLLERPDPOSE_H
#define CONTROLLERPDPOSE_H

#include "../../framework/framework.hpp"

//PDOrient-Controller parameters
#include "../../controllerPDOrient/src/controllerPDOrientParams.hpp"

//Position-loop Controller Parameters
#define Pxy 0.03
#define Dxy 0.05

#define Pz 0 //@TODO put 4 here once there is an altitde estimate
#define Dz 0 //@TODO put 3 here once there is an altitde estimate


class controllerPDPose_t : public podBase_t {
  
public: 

  std::string stateVariancesChannel;

  //Pod-specific members to store received messages
  agile::poseRef_t   poseRef;
  agile::powerAdjust_t   powerAdjust;	
  agile::stateVariances_t     stateVariances;

  //constructor
  controllerPDPose_t (string podName, int64_t callInterval) : podBase_t(podName,callInterval)
	{	
	//Pod-specific initialization
	//...  	
	}

  //Pod-specifc members and functions for computations and storing results over more than 1 function cycle call
  agile::motorsWsRef_t motorsWsRef;

};








#endif
