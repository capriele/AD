#ifndef CONTROLLERPDPOSE_H
#define CONTROLLERPDPOSE_H

#include "../../framework/framework.hpp"

//PDOrient-Controller parameters
#include "../../framework/paramsControllerPDOrient.hpp"

//Position-loop Controller Parameters
#include "../../framework/paramsControllerPDPose.hpp"


class controllerPDPose_t : public podBase_t
{

public:

    std::string stateVariancesChannel;

    //Pod-specific members to store received messages
    agile::poseRef_t   poseRef;
    agile::powerAdjust_t   powerAdjust;
    agile::stateVariances_t     stateVariances;

    //constructor
    controllerPDPose_t (string podName, int64_t callInterval) : podBase_t(podName, callInterval)
    {
        //Pod-specific initialization
        //...
    }

    //Pod-specifc members and functions for computations and storing results over more than 1 function cycle call
    agile::motorsWsRef_t motorsWsRef;

};








#endif
