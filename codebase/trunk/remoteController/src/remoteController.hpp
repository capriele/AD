#ifndef REMOTECONTROLLER_H
#define REMOTECONTROLLER_H

#include "../../framework/framework.hpp"
#include "../../framework/paramsRemoteController.hpp"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>


class remoteController_t : public podBase_t
{

public:


    //Pod-specific members to store computation results
    agile::poseRef_t      poseRef;
    agile::controlMode_t 	controlMode;
    agile::powerAdjust_t 	powerAdjust;
    int16_t 		controllerAdjustmode; //PDOrient controller adjust mode none(0), P(1), D(2), bias(3)

    //constructor
    remoteController_t (string podName, int64_t callInterval) : podBase_t(podName, callInterval)
    {
        //Pod-specific initialization
        //...
        controlMode.controlMode    = CMODE_NULL;

        controllerAdjustmode = CADJUSTMODE_NULL;

        powerAdjust.powerGain 	= INITPOWERGAIN;

        //Biases for PDOrientController
        powerAdjust.tBiasPDO 	= INITBIASPDO;
        powerAdjust.yBiasPDO    = INITYBIASPDO;
        powerAdjust.pBiasPDO    = INITPBIASPDO;
        powerAdjust.rBiasPDO    = INITRBIASPDO;

        //Adjustments of PD-gains in PDOrientController
        powerAdjust.pyAdjustPDO = 1.0;
        powerAdjust.dyAdjustPDO = 1.0;
        powerAdjust.ppAdjustPDO = INITPDGAINADJUST;
        powerAdjust.dpAdjustPDO = INITPDGAINADJUST;
        powerAdjust.prAdjustPDO = INITPDGAINADJUST;
        powerAdjust.drAdjustPDO = INITPDGAINADJUST;
        powerAdjust.ptAdjustPDO = 1.0;
        powerAdjust.dtAdjustPDO = 1.0;

        //Adjustments of PD-gains in position loop of PDPose-controller (inner orientation loop gains come from PDOrientcontroller!)
        powerAdjust.pXYAdjustPDPOS = 1.0;
        powerAdjust.dXYAdjustPDPOS = 1.0;
        powerAdjust.pZAdjustPDPOS  = 1.0;
        powerAdjust.dZAdjustPDPOS  = 1.0;

        //Initial reference pose
        poseRef.position[0] = 0.0;
        poseRef.position[1] = 0.0;
        poseRef.position[2] = -1;
        poseRef.orientEuler[0] = 0.0;
        poseRef.orientEuler[1] = 0.0;
        poseRef.orientEuler[2] = 0.0;





    }



    void resetPitchRollReference()
    {
        poseRef.orientEuler[1] = 0.0;
        poseRef.orientEuler[2] = 0.0;
    }

    static void changemode(int dir)
    {
        static struct termios oldt, newt;

        if(dir == 1)
        {
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        }
        else
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }

    static int kbhit(void)
    {
        struct timeval tv;
        fd_set rdfs;

        tv.tv_sec = 0;
        tv.tv_usec = 0;

        FD_ZERO(&rdfs);
        FD_SET(STDIN_FILENO, &rdfs);

        select(STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv);
        return FD_ISSET(STDIN_FILENO, &rdfs);

    }

};




#endif
