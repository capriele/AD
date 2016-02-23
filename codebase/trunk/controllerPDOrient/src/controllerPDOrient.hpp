#ifndef CONTROLLERPDORIENT_H
#define CONTROLLERPDORIENT_H

#include "../../framework/framework.hpp"

//Controller parameters
#include "../../framework/paramsControllerPDOrient.hpp"

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
	this->powerAdjust.tBiasPDO;
	this->powerAdjust.yBiasPDO;
	this->powerAdjust.pBiasPDO;
	this->powerAdjust.rBiasPDO;

	this->powerAdjust.pyAdjustPDO;
	this->powerAdjust.dyAdjustPDO;
	this->powerAdjust.ppAdjustPDO;
	this->powerAdjust.dpAdjustPDO;
	this->powerAdjust.prAdjustPDO;
	this->powerAdjust.drAdjustPDO;
	this->powerAdjust.ptAdjustPDO;
	this->powerAdjust.dtAdjustPDO;


	this->powerAdjust.pXYAdjustPDPOS;
	this->powerAdjust.dXYAdjustPDPOS;
	this->powerAdjust.pZAdjustPDPOS;
	this->powerAdjust.dZAdjustPDPOS;

	this->poseRef.position[0] = 0.0;
	this->poseRef.position[1] = 0.0;
	this->poseRef.position[2] = -1.0;

	this->poseRef.orientEuler[0] = 0.0;
	this->poseRef.orientEuler[1] = 0.0;
	this->poseRef.orientEuler[2] = 0.0;

    }

    //Pod-specifc members and functions for computations and storing results over more than 1 function cycle call
    agile::motorsWsRef_t motorsWsRef;

};








#endif
