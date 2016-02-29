#ifndef CONTROLLERSOCORIENT_H
#define CONTROLLERSOCORIENT_H

#include <stdlib.h>
#include <assert.h>

#include "../../framework/framework.hpp"

extern "C"{
#include "c3.h"
#include "c3sc.h"
}


//Controller parameters
#include "../../framework/paramsControllerPDOrient.hpp"


struct Controller
{
  struct FT1DArray * controls;
};

/*
Loads controller in ft-format
*/
void controller_load(struct Controller *, std::string);

/*
frees stored controller in ft-format
*/
void controller_free(struct Controller *);

/*
retrieves control policy from controller in ft-format; takes Controller *, pointer to state estimates and pointer to control actions
*/
void controller_eval(struct Controller *, double * state, double * controls);


class controllerSOCOrient_t : public podBase_t
{

public:

  std::string stateVariancesChannel;
  std::string controllerFilePath;
  struct Controller controller;

  //Pod-specific members to store received messages
  agile::poseRef_t   poseRef;
  agile::powerAdjust_t   powerAdjust;
  agile::stateVariances_t     stateVariances;

  //constructor  


  controllerSOCOrient_t (string podName, int64_t callInterval) : podBase_t(podName, callInterval)
  {    
    //Pod-specific initialization
    //...
  }

  //Pod-specifc members and functions for computations and storing results over more than 1 function cycle call
  agile::motorsWsRef_t motorsWsRef;


  bool updateStatus() override;
  bool doComputations() override;

  void getControlleraction(double * orientState, double * torquesRefs);
};








#endif
