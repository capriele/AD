#ifndef CONTROLLERPDORIENT_H
#define CONTROLLERPDORIENT_H

#include "../../framework/framework.hpp"

//Controller parameters
#include "../../framework/paramsControllerPDOrient.hpp"
#include "../../framework/paramsRemoteController.hpp"

class controllerPDOrient_t : public podBase_t
{

public:

    std::string stateVariancesChannel;

    //Pod-specific members to store received messages
    agile::poseRef_t   poseRef;
    agile::powerAdjust_t   powerAdjust;
    agile::stateVariances_t     stateVariances;

    //constructor
    controllerPDOrient_t (string podName, int64_t callInterval) : podBase_t(podName, callInterval)
    {
        //Pod-specific initialization
        //...
	this->powerAdjust.tBiasPDO = INITBIASPDO;
	this->powerAdjust.yBiasPDO = INITYBIASPDO;
	this->powerAdjust.pBiasPDO = INITPBIASPDO;
	this->powerAdjust.rBiasPDO = INITRBIASPDO;

	this->powerAdjust.pyAdjustPDO = INITPDGAINADJUST_YZ;
	this->powerAdjust.dyAdjustPDO = INITPDGAINADJUST_YZ;
	this->powerAdjust.ppAdjustPDO = INITPDGAINADJUST_PRX;
	this->powerAdjust.dpAdjustPDO = INITPDGAINADJUST_PRX;
	this->powerAdjust.prAdjustPDO = INITPDGAINADJUST_PRX;
	this->powerAdjust.drAdjustPDO = INITPDGAINADJUST_PRX;
	this->powerAdjust.ptAdjustPDO = INITPDGAINADJUST_PRX;
	this->powerAdjust.dtAdjustPDO = INITPDGAINADJUST_PRX;


	this->powerAdjust.pXYAdjustPDPOS = INITPDGAINADJUST_PRX;
	this->powerAdjust.dXYAdjustPDPOS = INITPDGAINADJUST_PRX;
	this->powerAdjust.pZAdjustPDPOS = INITPDGAINADJUST_YZ;
	this->powerAdjust.dZAdjustPDPOS = INITPDGAINADJUST_YZ;

	this->poseRef.position[0] = 0.0;
	this->poseRef.position[1] = 0.0;
	this->poseRef.position[2] = -1.0;

	this->poseRef.orientEuler[0] = 0.0;
	this->poseRef.orientEuler[1] = 0.0;
	this->poseRef.orientEuler[2] = 0.0;

    }

    //Pod-specifc members and functions for computations and storing results over more than 1 function cycle call
    agile::motorsWsRef_t motorsWsRef;

    bool updateStatus() override;
    bool doComputations() override;
};








#endif
