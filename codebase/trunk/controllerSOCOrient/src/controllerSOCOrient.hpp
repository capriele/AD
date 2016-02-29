#ifndef CONTROLLERSOCORIENT_H
#define CONTROLLERSOCORIENT_H

#include "../../framework/framework.hpp"

//Controller parameters
#include "../../framework/paramsControllerPDOrient.hpp"

class controllerSOCOrient_t : public podBase_t
{

public:

    std::string stateVariancesChannel;

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

    std::string controllerFilePath;
    FILE* controllerFileHandle;
    controllerLookup_t* controllerLookup;

    bool updateStatus() override;
    bool doComputations() override;
};








#endif
