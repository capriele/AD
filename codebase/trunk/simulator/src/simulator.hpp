#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "../../framework/framework.hpp"
#include "../../framework/paramsSimulator.hpp"

#include <stddef.h>

extern "C" {
#include "SimulatorDrone.h"            /* Model's header file */
#include "rtwtypes.h"
#include "zero_crossing_types.h"
}





class simulator_t : public podBase_t
{

public:

    //Pod-specific members to store received messages of channels that this POD is subscribed to (Note that every pod is auto-subscribed to statusWatchdog and has a member to store this (see base class constructor))
    agile::motorsPwms_t   motorsPwms;

    //constructor
    simulator_t (string podName, int64_t callInterval) : podBase_t(podName, callInterval)
    {
        //Pod-specific initialization
        //...
        this->motorsPwms.pwms[0] = 0.0;
        this->motorsPwms.pwms[1] = 0.0;
        this->motorsPwms.pwms[2] = 0.0;
        this->motorsPwms.pwms[3] = 0.0;
    }

    //Pod-specifc members for storing values (if some values in the computation-cycle or statusUpdate-cycle are needed over more than 1 function call)
    agile::stateVariances_t stateVariancesSim;
    agile::imuRaw_t imuRawSim;
    agile::poseRef_t stateEulerSim;

    //Pod-specific member functions

};








#endif
