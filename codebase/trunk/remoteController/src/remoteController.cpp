
#include "remoteController.hpp"

using namespace std;


/*
de/increment and potentially saturatates values
*/
void crement(double* var, double step, double min, double max)
{
    *var += step;
    if(*var > max)
    {
        *var = max;
    };
    if(*var < min)
    {
        *var = min;
    };
    printf("%f (%f..%f)\n", *var, min, max);
};


/*
Implementation of loop function for computations in this specific POD
*/

bool remoteController_t::doComputations()
{
    remoteController_t* podWorker = this;
    std::lock_guard<std::mutex> guard(podMutex);

    /*--------*/




    /* Computations */
    int ch;
    remoteController_t::changemode(1);


    if(!remoteController_t::kbhit())
    {
        //nothing new on keyboard
    }
    else
    {
        //we got sth on keyboard!
        ch = getchar();
        remoteController_t::changemode(0);

        //FLY MODE
        if(controllerAdjustmode == CADJUSTMODE_NULL)
        {
            switch(ch)
            {
            case ',': controllerAdjustmode =  CADJUSTMODE_P; printf("Controlleradjust switched to P-values!\n");  break;
            case '.': controllerAdjustmode =  CADJUSTMODE_D; printf("Controlleradjust switched to D-values!\n");  break;
            case '/':
                if(controlMode.controlMode == CMODE_PIDORIENT)
                {
                    controllerAdjustmode =  CADJUSTMODE_BIAS;
                    printf("Controlleradjust switched to Bias-values!\n");
                };
                break;


            //control mode cases
            case 'e':
                controlMode.controlMode = CMODE_FATAL;
                resetPitchRollReference();
                printf("Emergency issued!\n");
                break;
            case '6':
                controlMode.controlMode = CMODE_PIDORIENT;
                resetPitchRollReference();
                printf("PIDOrient controller enabled, with ref. orient reset!\n");
                break;
            case '7':
                controlMode.controlMode =  CMODE_PIDPOSE;
                resetPitchRollReference();
                printf("PIDPosition controller enabled, with ref. orient reset!\n");
                break;
            case '8':
                controlMode.controlMode =  CMODE_SOCORIENT;
                resetPitchRollReference();
                printf("SOCOrient controller enabled, with ref. orient reset!\n");
                break;

            case '9':
                controlMode.controlMode =  CMODE_SOCPOSE;
                resetPitchRollReference();
                printf("SOCPOse controller enabled, with ref. orient reset!\n");
                break;


            //altitude-related references
            case 'w':
                if(controlMode.controlMode == CMODE_PIDPOSE)  //increase reference altitude
                {
                    poseRef.position[2] -=  STEPSALTITUDE;
                }
                else if(controlMode.controlMode == CMODE_PIDORIENT)  //increase thrust bias in PDOrient controller
                {
                    crement(&(powerAdjust.tBiasPDO), +STEPSTBIASPDO, MINTBIASPDO, MAXTBIASPDO);
                    printf("thrust-bias for PDOrient: %f\n", powerAdjust.tBiasPDO);
                };
                break;

            case 's':
                if(controlMode.controlMode ==  CMODE_PIDPOSE)
                {
                    poseRef.position[2] +=  STEPSALTITUDE;
                    if(poseRef.position[2] >= 0) poseRef.position[2] = 0;
                }
                else if(controlMode.controlMode ==  CMODE_PIDORIENT)
                {
                    crement(&(powerAdjust.tBiasPDO), -STEPSTBIASPDO, MINTBIASPDO, MAXTBIASPDO);
                    printf("thrust-bias for PDOrient: %f\n", powerAdjust.tBiasPDO);
                };
                break;


            //orientaion-related references
            case 'j': poseRef.orientEuler[2] -=  STEPSPITCHROLL; break;
            case 'l': poseRef.orientEuler[2] +=  STEPSPITCHROLL; break;
            case 'i': poseRef.orientEuler[1] -=  STEPSPITCHROLL; break;
            case 'k': poseRef.orientEuler[1] +=  STEPSPITCHROLL; break;
            case 'a': poseRef.orientEuler[0] -=  STEPSYAW; break;
            case 'd': poseRef.orientEuler[0] +=  STEPSYAW; break;

            //power adjustment to happen in motor commander
            case '+':
                crement(&(powerAdjust.powerGain), +STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
                printf("powerGain in Poweradjuster: %f\n", powerAdjust.powerGain);
                break;
            case '-':
                crement(&(powerAdjust.powerGain), -STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
                printf("powerGain in Poweradjuster: %f\n", powerAdjust.powerGain);
                break;
            } //end switch

            //When pitch roll was controlled, set controlMode to orient control of pitch roll reference angle exists
            if((controlMode.controlMode > CMODE_NULL) && ((ch == 'j') || (ch == 'l') || (ch == 'i') || (ch == 'k')))
            {
                if((controlMode.controlMode !=  CMODE_PIDORIENT) && ((abs(poseRef.orientEuler[1]) > EPSREMOTE) || (abs(poseRef.orientEuler[2]) > EPSREMOTE)))
                {
                    controlMode.controlMode =  CMODE_PIDORIENT;
                    printf("auto-switch to PIDOrient controller, with ref. orient reset!\n");
                }
            }
        } //endif flymode

        //CONTROLLER PDORIENT ADJUSTMODE BIAS
        else if((controlMode.controlMode == CMODE_PIDORIENT) && (controllerAdjustmode == CADJUSTMODE_BIAS))
        {
            switch(ch)
            {
            //control mode cases
            case 'e':
                controlMode.controlMode =  CMODE_FATAL;
                resetPitchRollReference();
                break;

            case 'm': controllerAdjustmode =  CADJUSTMODE_NULL; printf("Controlleradjust mode turned off!\n"); break;
            case '.': controllerAdjustmode =  CADJUSTMODE_D;    printf("Controlleradjust switched to D-values!\n");  break;
            case ',': controllerAdjustmode =  CADJUSTMODE_P;    printf("Controlleradjust switched to P-values!\n");  break;

            //thrust-related controller  adjustments
            case 'w': crement(&(powerAdjust.tBiasPDO), +STEPSTBIASPDO, MINTBIASPDO, MAXTBIASPDO); printf("thrust-bias for PDOrient: %f\n", powerAdjust.tBiasPDO); break;

            case 's': crement(&(powerAdjust.tBiasPDO), -STEPSTBIASPDO, MINTBIASPDO, MAXTBIASPDO); printf("thrust-bias for PDOrient: %f\n", powerAdjust.tBiasPDO); break;

            //orient-related controller  adjustments
            case 'j': crement(&(powerAdjust.rBiasPDO), -STEPSRBIASPDO, MINRBIASPDO, MAXRBIASPDO); break;
            case 'l': crement(&(powerAdjust.rBiasPDO), +STEPSRBIASPDO, MINRBIASPDO, MAXRBIASPDO); break;
            case 'i': crement(&(powerAdjust.pBiasPDO), -STEPSPBIASPDO, MINPBIASPDO, MAXPBIASPDO); break;
            case 'k': crement(&(powerAdjust.pBiasPDO), +STEPSPBIASPDO, MINPBIASPDO, MAXPBIASPDO); break;
            case 'a': crement(&(powerAdjust.yBiasPDO), -STEPSYBIASPDO, MINYBIASPDO, MAXYBIASPDO); break;
            case 'd': crement(&(powerAdjust.yBiasPDO), +STEPSYBIASPDO, MINYBIASPDO, MAXYBIASPDO); break;

            //power adjustment to happen in motor commander
            case '+':
                crement(&(powerAdjust.powerGain), +STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
                printf("powerGain in Poweradjuster: %f\n", powerAdjust.powerGain);
                break;
            case '-':
                crement(&(powerAdjust.powerGain), -STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
                printf("powerGain in Poweradjuster: %f\n", powerAdjust.powerGain);
                break;

            }
        } //endif Bias-controladjuster

        //CONTROLLER PDORIENT ADJUSTMODE P-value
        else if((controlMode.controlMode == CMODE_PIDORIENT) && (controllerAdjustmode == CADJUSTMODE_P))
        {
            switch(ch)
            {
            //control mode cases
            case 'e':
                controlMode.controlMode =  CMODE_FATAL;
                resetPitchRollReference();
                break;

            case 'm': controllerAdjustmode =  CADJUSTMODE_NULL; printf("Controlleradjust mode turned off!\n"); break;
            case '.': controllerAdjustmode =  CADJUSTMODE_D; printf("Controlleradjust switched to D-values!\n");  break;
            case '/': controllerAdjustmode =  CADJUSTMODE_BIAS; printf("Controlleradjust switched to Bias-values!\n");  break;

            //thrust-related controller  adjustments
            case 'w': crement(&(powerAdjust.ptAdjustPDO), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
            case 's': crement(&(powerAdjust.ptAdjustPDO), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;

            //orient-related controller  adjustments
            case 'j': crement(&(powerAdjust.prAdjustPDO), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
            case 'l': crement(&(powerAdjust.prAdjustPDO), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
            case 'i': crement(&(powerAdjust.ppAdjustPDO), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
            case 'k': crement(&(powerAdjust.ppAdjustPDO), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
            case 'a': crement(&(powerAdjust.pyAdjustPDO), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
            case 'd': crement(&(powerAdjust.pyAdjustPDO), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;

            //power adjustment to happen in motor commander
            case '+':
                crement(&(powerAdjust.powerGain), +STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
                printf("powerGain in Poweradjuster: %f\n", powerAdjust.powerGain);
                break;
            case '-':
                crement(&(powerAdjust.powerGain), -STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
                printf("powerGain in Poweradjuster: %f\n", powerAdjust.powerGain);
                break;

            }
        } //endif P-controladjuster

        //CONTROLLER PDORIENT ADJUSTMODE D-value
        else if((controlMode.controlMode == CMODE_PIDORIENT) && (controllerAdjustmode == CADJUSTMODE_D))
        {
            switch(ch)
            {
            //control mode cases
            case 'e':
                controlMode.controlMode =  CMODE_FATAL;
                resetPitchRollReference();
                break;

            case 'm': controllerAdjustmode =  CADJUSTMODE_NULL; printf("Controlleradjust mode turned off!\n"); break;
            case ',': controllerAdjustmode =  CADJUSTMODE_P; printf("Controlleradjust switched to P-values!\n");  break;
            case '/': controllerAdjustmode =  CADJUSTMODE_BIAS; printf("Controlleradjust switched to Bias-values!\n");  break;

            //thrust-related controller  adjustments
            case 'w': crement(&(powerAdjust.dtAdjustPDO), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
            case 's': crement(&(powerAdjust.dtAdjustPDO), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;

            //orient-related controller  adjustments
            case 'j': crement(&(powerAdjust.drAdjustPDO), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
            case 'l': crement(&(powerAdjust.drAdjustPDO), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
            case 'i': crement(&(powerAdjust.dpAdjustPDO), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
            case 'k': crement(&(powerAdjust.dpAdjustPDO), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
            case 'a': crement(&(powerAdjust.dyAdjustPDO), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
            case 'd': crement(&(powerAdjust.dyAdjustPDO), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;

            //power adjustment to happen in motor commander
            case '+':
                crement(&(powerAdjust.powerGain), +STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
                printf("powerGain in Poweradjuster: %f\n", powerAdjust.powerGain);
                break;
            case '-':
                crement(&(powerAdjust.powerGain), -STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
                printf("powerGain in Poweradjuster: %f\n", powerAdjust.powerGain);
                break;

            }
        } //endif D-controladjuster








        //CONTROLLER PDPOSE ADJUSTMODE P-value
        else if((controlMode.controlMode == CMODE_PIDPOSE) && (controllerAdjustmode == CADJUSTMODE_P))
        {
            switch(ch)
            {
            //control mode cases
            case 'e':
                controlMode.controlMode =  CMODE_FATAL;
                resetPitchRollReference();
                break;

            case 'm': controllerAdjustmode =  CADJUSTMODE_NULL; printf("Controlleradjust mode turned off!\n"); break;
            case '.': controllerAdjustmode =  CADJUSTMODE_D; printf("Controlleradjust switched to D-values!\n");  break;

            //position-related controller  adjustments
            case 'j': crement(&(powerAdjust.pXYAdjustPDPOS), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
            case 'l': crement(&(powerAdjust.pXYAdjustPDPOS), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;

            case 'w': crement(&(powerAdjust.pZAdjustPDPOS), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
            case 's': crement(&(powerAdjust.pZAdjustPDPOS), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;


            //power adjustment to happen in motor commander
            case '+':
                crement(&(powerAdjust.powerGain), +STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
                printf("powerGain in Poweradjuster: %f\n", powerAdjust.powerGain);
                break;
            case '-':
                crement(&(powerAdjust.powerGain), -STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
                printf("powerGain in Poweradjuster: %f\n", powerAdjust.powerGain);
                break;

            }
        } //endif P-controladjuster

        //CONTROLLER PDPOSE ADJUSTMODE P-value
        else if((controlMode.controlMode == CMODE_PIDPOSE) && (controllerAdjustmode == CADJUSTMODE_D))
        {
            switch(ch)
            {
            //control mode cases
            case 'e':
                controlMode.controlMode =  CMODE_FATAL;
                resetPitchRollReference();
                break;

            case 'm': controllerAdjustmode =  CADJUSTMODE_NULL; printf("Controlleradjust mode turned off!\n"); break;
            case '.': controllerAdjustmode =  CADJUSTMODE_D; printf("Controlleradjust switched to P-values!\n");  break;

            //position-related controller  adjustments
            case 'j': crement(&(powerAdjust.dXYAdjustPDPOS), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
            case 'l': crement(&(powerAdjust.dXYAdjustPDPOS), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;

            case 'w': crement(&(powerAdjust.dZAdjustPDPOS), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
            case 's': crement(&(powerAdjust.dZAdjustPDPOS), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;


            //power adjustment to happen in motor commander
            case '+':
                crement(&(powerAdjust.powerGain), +STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
                printf("powerGain in Poweradjuster: %f\n", powerAdjust.powerGain);
                break;
            case '-':
                crement(&(powerAdjust.powerGain), -STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
                printf("powerGain in Poweradjuster: %f\n", powerAdjust.powerGain);
                break;

            }
        } //endif D-controladjuster

    } //end if keyboard isHit

    controlMode.timestampJetson = GetTimeStamp();
    poseRef.timestampJetson = GetTimeStamp();
    powerAdjust.timestampJetson = GetTimeStamp();

    /*---------*/


    /* Publishing */

    lcm.publish("controlMode", &controlMode);
    lcm.publish("poseRef", &poseRef);
    lcm.publish("powerAdjust", &powerAdjust);

    /*---------*/



    /*General Infrastructure (maintain this structure!)*/
    updateComputationInterval();
    return TRUE;
    /*---------*/
}
/*--------------------------*/





/*
Implementation of loop function for publishing statusPod
*/

bool remoteController_t::updateStatus()
{
    remoteController_t* podWorker = this;
    messageStatus_t messageStatus = checkMessagesUptodate();
    std::lock_guard<std::mutex> guard(podMutex);
    /*---------*/

    /*Computation statusPOD*/

    if(computationInterval > MAXPODDELAY_X * callInterval * MS2US)
    {
        printf("%s: delay in computation, dt=% " PRId64 "us at t=%" PRId64 "!\n", podName.c_str(), computationInterval,GetTimeStamp());
        statusPod.status = POD_FATAL;
    }
    else 
    {

	if(messageStatus == MSGS_LATE)
    	{
		statusPod.status = POD_CRITICAL;
	}
	else if(messageStatus == MSGS_DEAD)
	{
		statusPod.status = POD_FATAL;
	}
	else
	{
		statusPod.status = POD_OK;
	};

    };

    /*---------*/

    /*Publishing statusPOD*/
    publishStatus(statusPod.status);
    /*---------*/

    return TRUE;
};
/*--------------------------*/



/*
main to initialize and start the POD
*/

int main(int argc, char** argv)
{

    /* General Infrastructure: setup (maintain this structure!)  */

    // 1) Create the app
    remoteController_t podWorker = remoteController_t("remoteController", CALLINTERVAL_REMOTECONTROLLER); 	//provide your PODname here!
    // 2) Create LCM
    if(!podWorker.lcm.good())
        return 1;

    /*---------*/

    // 3) Subscribe this POD to channels
	//resubscribre statusWatchdog and Drone with different message-age tolerance settings
    podWorker.unsubscribe("statusWatchdog"); //unsubscribed from base-class-autosubscribed channels
    podWorker.unsubscribe("statusDrone");	  //unsubscribed from base-class-autosubscribed channels

    podWorker.subscribe("statusWatchdog", CALLINTERVAL_WATCHDOG * AGEMSG_X_TUNNEL, &(podWorker.statusWatchdog), &podBase_t::handleMessage<agile::statusPod_t>);
    podWorker.subscribe("statusDrone", CALLINTERVAL_WATCHDOG * AGEMSG_X_TUNNEL, &(podWorker.statusDrone), &podBase_t::handleMessage<agile::statusDrone_t>);

    /*  POD-specific init procedures  */
    // Update and publish status of remoteController (this POD is the first to start)
    podWorker.publishStatus(POD_INITING);


    /*---------*/



    /*  General Infrastructure: loops (maintain this structure!)  */

    //  POD-wait for watchdogPOD to be up and running (POD_OK)  before continue

    printf("Waiting for watchdog to be POD_OK...\n");

    while(podWorker.statusWatchdog.status != POD_OK)
    {
        podWorker.lcm.handle();
    }
    printf("Watchdog POD_OK! Continuing...\n");
    podWorker.initComputationInterval();

    //Create message listener thead
    std::thread listenerThread(listen, &podWorker);

    // Create mainloop, add loop functions, run

    GMainLoop* mainloop = g_main_loop_new(NULL, FALSE);

    guint timerComputations   = g_timeout_add(podWorker.callInterval, podBase_t::gtimerfuncComputations, (gpointer)&podWorker);
    guint timerStatusPod      =    g_timeout_add(podWorker.callInterval, podBase_t::gtimerfuncStatusPod, (gpointer)&podWorker);

    g_main_loop_run(mainloop);

    /*---------*/

    return 0;
}







