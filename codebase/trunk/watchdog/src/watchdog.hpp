#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "../../framework/framework.hpp"
#include <unistd.h>



class watchdog_t : public podBase_t
{

public:

    //Pod-specific members to store received messages

    agile::stateVariances_t      stateVariances;
    agile::imuRaw_t     	       imuRaw;

    agile::controlMode_t controlMode;


    agile::statusPod_t  statusImuAcquisition;
    agile::statusPod_t  statusStateEstimatorOrientCF;
    //agile::statusPod_t  statusStateEstimatorOrientV1;
    agile::statusPod_t  statusDetectorVis;
    agile::statusPod_t  statusRemoteController;
    agile::statusPod_t  statusControllerPDPose;
    agile::statusPod_t  statusControllerPDOrient;
    agile::statusPod_t  statusControllerSOCOrient;
    agile::statusPod_t  statusControllerSOCPose;
    agile::statusPod_t  statusMotorCommander;

    //constructor
    watchdog_t (string podName, int64_t callInterval) : podBase_t(podName, callInterval)
    {
        takeoffMoment = -1;
        controlMode.controlMode = CMODE_NULL;
        //Pod-specific initialization
        //...
    }

    //Pod-specifc members and function for computations and storing results
    int64_t takeoffMoment;

    std::string imuRawChannel;
    std::string stateVariancesChannel;    

    bool updateStatus() override;
    bool doComputations() override;
};








#endif
