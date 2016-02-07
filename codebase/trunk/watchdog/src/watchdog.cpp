
#include "watchdog.hpp"

using namespace std;


void errorPrint(bool isSomePodCritical, bool isSomePodFatal, bool isStateCritical, bool isStateFatal, int16_t controlMode)
{
    string message = "Watchdog: Problem(s) with drone:\n ";

    if(isSomePodCritical) message 	= message + "> Some POD is critical\n";
    if(isSomePodFatal) message    	= message + "> Some POD is fatal\n";
    if(isStateCritical) message   	= message + "> Some state is critical\n";
    if(isStateFatal) message      	= message + "> Some state is fatal\n";
    if(controlMode == CMODE_FATAL) message 	= message + "> Control mode is FATAL\n";

    printf("%s\n", message.c_str());

}

/*
Implementation of loop function for computations in this specific POD
*/

gboolean podBase_t::gtimerfuncComputations(gpointer data)
{

    /* General Infrastructure (maintain this structure!) */
    watchdog_t* podWorker = (watchdog_t*) data;
    std::lock_guard<std::mutex> guard(podMutex);
    /*--------*/


    /* Computations */

    // statusDrone - computation via stateMachine (see statusDrone_StateMachine.pdf)

    //compute safety variables //@TODO replace this hardcoded mess: cycle through subscribed messages, check if .status available and then check that for ok/...
    agile::statusDrone_t statusDrone_old = podWorker->statusDrone;

    bool isSomePodCritical = ((podWorker->statusPod.status == POD_CRITICAL) || (podWorker->statusImuAcquisition.status == POD_CRITICAL) || /*(podWorker->statusStateEstimatorOrientV1.status==POD_CRITICAL) ||*/ (podWorker->statusStateEstimatorOrientCF.status == POD_CRITICAL) || (podWorker->statusDetectorVis.status == POD_CRITICAL) || (podWorker->statusRemoteController.status == POD_CRITICAL) || (podWorker->statusControllerPDPose.status == POD_CRITICAL) || (podWorker->statusControllerPDOrient.status == POD_CRITICAL) || (podWorker->statusControllerSOCOrient.status == POD_CRITICAL) || (podWorker->statusControllerSOCPose.status == POD_CRITICAL) || (podWorker->statusMotorCommander.status == POD_CRITICAL));

    bool isSomePodFatal = ((podWorker->statusPod.status == POD_FATAL) || (podWorker->statusImuAcquisition.status == POD_FATAL) || /*(podWorker->statusStateEstimatorOrientV1.status==POD_FATAL) ||*/ (podWorker->statusStateEstimatorOrientCF.status == POD_FATAL) || (podWorker->statusDetectorVis.status == POD_FATAL) || (podWorker->statusRemoteController.status == POD_FATAL) || (podWorker->statusControllerPDPose.status == POD_FATAL) || (podWorker->statusControllerPDOrient.status == POD_FATAL) || (podWorker->statusControllerSOCOrient.status == POD_FATAL) || (podWorker->statusControllerSOCPose.status == POD_FATAL) || (podWorker->statusMotorCommander.status == POD_FATAL));

    bool isAllPodsOK = ((podWorker->statusPod.status == POD_OK) && (podWorker->statusImuAcquisition.status == POD_OK) && /*(podWorker->statusStateEstimatorOrientV1.status==POD_OK) &&*/ (podWorker->statusStateEstimatorOrientCF.status == POD_OK) && (podWorker->statusDetectorVis.status == POD_OK) && (podWorker->statusRemoteController.status == POD_OK) && (podWorker->statusControllerPDPose.status == POD_OK) && (podWorker->statusControllerPDOrient.status == POD_OK) && (podWorker->statusControllerSOCOrient.status == POD_OK) && (podWorker->statusControllerSOCPose.status == POD_OK) && (podWorker->statusMotorCommander.status == POD_OK));


    double yaw = 0.0;
    double pitch = 0.0;
    double roll = 0.0;
    quat2Euler(podWorker->stateVariances.orient, &(yaw), &(pitch), &(roll));

    //@TODO anguarl rates in pqr bodyframe?
    //Check for critical state @TODO pqr rates or [0..2]?

    bool isStateCritical = ((abs(podWorker->stateVariances.position[0]) > MAXPOS_CRITICAL) || (abs(podWorker->stateVariances.position[1]) > MAXPOS_CRITICAL) || (abs(podWorker->stateVariances.position[3]) > MAXALT_CRITICAL) || (abs(yaw) > MAXORIENT_Y_CRITICAL)  || (abs(pitch) > MAXORIENT_RP_CRITICAL) || (abs(roll) > MAXORIENT_RP_CRITICAL) || (abs(podWorker->stateVariances.veloPositionBody[0]) > MAXVELPOS_CRITICAL) || (abs(podWorker->stateVariances.veloPositionBody[1]) > MAXVELPOS_CRITICAL) || (abs(podWorker->stateVariances.veloPositionBody[2]) > MAXVELPOS_CRITICAL)  || (abs(podWorker->stateVariances.veloOrientBody[0]) > MAXVELORIENT_PQ_CRITICAL) || (abs(podWorker->stateVariances.veloOrientBody[1]) > MAXVELORIENT_PQ_CRITICAL)  || (abs(podWorker->stateVariances.veloOrientBody[2]) > MAXVELORIENT_R_CRITICAL) || (abs(podWorker->imuRaw.accel[0]) > MAXACC_CRITICAL) || (abs(podWorker->imuRaw.accel[1]) > MAXACC_CRITICAL) || (abs(podWorker->imuRaw.accel[2]) > MAXACC_CRITICAL));

    bool isStateFatal = ((abs(podWorker->stateVariances.position[0]) > MAXPOS_FATAL) || (abs(podWorker->stateVariances.position[1]) > MAXPOS_FATAL) || (abs(podWorker->stateVariances.position[3]) > MAXALT_FATAL) || (abs(yaw) > MAXORIENT_Y_FATAL)  || (abs(pitch) > MAXORIENT_RP_FATAL) || (abs(roll) > MAXORIENT_RP_FATAL) || (abs(podWorker->stateVariances.veloPositionBody[0]) > MAXVELPOS_FATAL) || (abs(podWorker->stateVariances.veloPositionBody[1]) > MAXVELPOS_FATAL) || (abs(podWorker->stateVariances.veloPositionBody[2]) > MAXVELPOS_FATAL)  || (abs(podWorker->stateVariances.veloOrientBody[0]) > MAXVELORIENT_PQ_FATAL) || (abs(podWorker->stateVariances.veloOrientBody[1]) > MAXVELORIENT_PQ_FATAL)  || (abs(podWorker->stateVariances.veloOrientBody[2]) > MAXVELORIENT_R_FATAL) || (abs(podWorker->imuRaw.accel[0]) > MAXACC_FATAL) || (abs(podWorker->imuRaw.accel[1]) > MAXACC_FATAL) || (abs(podWorker->imuRaw.accel[2]) > MAXACC_FATAL));


    //"state machine"
    switch(statusDrone_old.status)
    {
    case DRONE_WAITPODS:
        if(isAllPodsOK)
        {
            podWorker->statusDrone.status = DRONE_IDLE;
            printf("watchdog: drone from waitPodsOK to IDLE\n");
        };
        break;

    case DRONE_IDLE:
        if(isSomePodFatal || (podWorker->controlMode.controlMode == CMODE_FATAL) || isSomePodCritical || (podWorker->controlMode.controlMode == CMODE_CRITICAL))
        {
            podWorker->statusDrone.status = DRONE_FATAL;
            printf("watchdog: drone from IDLE to FATAL\n");
            errorPrint(isSomePodCritical, isSomePodFatal, isStateCritical, isStateFatal, podWorker->controlMode.controlMode);
        }

        else if(isAllPodsOK && (podWorker->controlMode.controlMode > CMODE_NULL))
        {
            podWorker->statusDrone.status = DRONE_TAKEOFF;
            podWorker->takeoffMoment = GetTimeStamp();
            printf("watchdog: drone from IDLE to TAKEOFF\n");
        }
        break;

    case DRONE_TAKEOFF:
        if(isStateFatal || isSomePodFatal || (podWorker->controlMode.controlMode == CMODE_FATAL))
        {
            podWorker->statusDrone.status = DRONE_FATAL;
            printf("watchdog: drone from TAKEOFF to FATAL\n");
            errorPrint(isSomePodCritical, isSomePodFatal, isStateCritical, isStateFatal, podWorker->controlMode.controlMode);
        }
        else if(isStateCritical || isSomePodCritical || (podWorker->controlMode.controlMode == CMODE_CRITICAL))
        {
            podWorker->statusDrone.status = DRONE_CRITICAL;
            printf("watchdog: drone from TAKEOFF to CRITICAL\n");
            errorPrint(isSomePodCritical, isSomePodFatal, isStateCritical, isStateFatal, podWorker->controlMode.controlMode);
        }
        else if((GetTimeStamp() - podWorker->takeoffMoment) > TAKEOFFINTERVAL * MS2US)
        {
            podWorker->statusDrone.status = DRONE_FLY;
            printf("watchdog: drone from TAKEOFF to FLY\n");
        }

        break;
    case DRONE_FLY:
        if(isStateFatal || isSomePodFatal || (podWorker->controlMode.controlMode == CMODE_FATAL))
        {
            podWorker->statusDrone.status = DRONE_FATAL;
            printf("watchdog: drone from FLY to FATAL\n");
            errorPrint(isSomePodCritical, isSomePodFatal, isStateCritical, isStateFatal, podWorker->controlMode.controlMode);
        }
        else if(isStateCritical || isSomePodCritical || (podWorker->controlMode.controlMode == CMODE_CRITICAL))
        {
            podWorker->statusDrone.status = DRONE_CRITICAL;
            printf("watchdog: drone from FLY to CRITICAL\n");
            errorPrint(isSomePodCritical, isSomePodFatal, isStateCritical, isStateFatal, podWorker->controlMode.controlMode);
        }

        break;

    case DRONE_CRITICAL:
        if(isStateFatal || isSomePodFatal || (podWorker->controlMode.controlMode == CMODE_FATAL))
        {
            podWorker->statusDrone.status = DRONE_FATAL;
            printf("watchdog: drone from CRITICAL to FATAL\n");
            errorPrint(isSomePodCritical, isSomePodFatal, isStateCritical, isStateFatal, podWorker->controlMode.controlMode);
        }
        else if(!isStateCritical && !isSomePodCritical && (podWorker->controlMode.controlMode > CMODE_FATAL))
        {
            podWorker->statusDrone.status = DRONE_FLY;
            printf("watchdog: drone from CRITICAL to FLY\n");
        }
        break;

    case DRONE_FATAL: ; break;

    default: podWorker->statusDrone.status = DRONE_FATAL;
    }

    podWorker->statusDrone.timestampJetson = GetTimeStamp();

    /*---------*/


    /* Publishing */

    // statusDrone - publish
    podWorker->lcm.publish("statusDrone", &podWorker->statusDrone);

    /*---------*/



    /*General Infrastructure (maintain this structure!)*/
    podWorker->updateComputationInterval();
    return TRUE;
    /*---------*/
}
/*--------------------------*/





/*
Implementation of loop function for publishing statusPod
*/

gboolean podBase_t::gtimerfuncStatusPod(gpointer data)
{

    /*General Infrastructure (maintain this infrastructure!)*/
    watchdog_t* podWorker = (watchdog_t*) data;
    /*---------*/

    /*Computation statusPOD*/
    if(podWorker->computationInterval > MAXPODDELAY_X * podWorker->callInterval * MS2US)
    {
        printf("watchdog: delay occured of % " PRId64 "us!\n", podWorker->computationInterval);
        podWorker->statusPod.status = POD_FATAL;
    }
    else if((podWorker->checkMessagesUptodate() == MSGS_LATE))
    {
        podWorker->statusPod.status = POD_CRITICAL;
    }
    else if((podWorker->checkMessagesUptodate() == MSGS_DEAD))
    {
        podWorker->statusPod.status = POD_FATAL;
    }
    else
    {
        podWorker->statusPod.status = POD_OK;
    };
    /*---------*/

    /*Publishing statusPOD*/
    podWorker->publishStatus(podWorker->statusPod.status);


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
    watchdog_t podWorker = watchdog_t("watchdog", CALLINTERVAL_WATCHDOG);

    if(argc < 3)
    {
        printf("Please provide imuRaw-Channel (1rst arg) and stateVariances-Channel (2nd) that watchdog will use for safety checks!\n");
        return EXIT_FAILURE;
    }
    else
    {
        podWorker.imuRawChannel = argv[1];
        podWorker.stateVariancesChannel = argv[2];
    };



    // 2) Create LCM
    if(!podWorker.lcm.good())
        return 1;

    // 3) Subscribe this POD to channels

    podWorker.unsubscribe("statusWatchdog"); //unsubscribed from base-class-autosubscribed channels
    podWorker.unsubscribe("statusDrone");	  //unsubscribed from base-class-autosubscribed channels

    if(strcmp(podWorker.stateVariancesChannel.c_str(), "stateVariancesSim") == 0)
        podWorker.subscribe(podWorker.stateVariancesChannel.c_str(), CALLINTERVAL_SIMULATOR, &(podWorker.stateVariances), &podBase_t::handleMessage<agile::stateVariances_t>);
    else if(strcmp(podWorker.stateVariancesChannel.c_str(), "stateVariancesOrientCF") == 0)
        podWorker.subscribe(podWorker.stateVariancesChannel.c_str(), CALLINTERVAL_STATEESTIMATORORIENTCF, &(podWorker.stateVariances), &podBase_t::handleMessage<agile::stateVariances_t>);
    /*else if (strcmp(podWorker.stateVariancesChannel.c_str(),"stateVariancesOrientV1")==0)
      podWorker.subscribe(podWorker.stateVariancesChannel.c_str(),CALLINTERVAL_STATEESTIMATORORIENTV1, &(podWorker.stateVariances), &podBase_t::handleMessage<agile::stateVariances_t>);*/

    podWorker.subscribe(podWorker.imuRawChannel.c_str(), CALLINTERVAL_IMUACQUISITION * 2, &(podWorker.imuRaw), &podBase_t::handleMessage<agile::imuRaw_t>); //@TODO remove callinterval x2
    podWorker.subscribe("controlMode", CALLINTERVAL_REMOTECONTROLLER, &(podWorker.controlMode), &podBase_t::handleMessage<agile::controlMode_t>);

    podWorker.subscribe("statusImuAcquisition", CALLINTERVAL_IMUACQUISITION, &(podWorker.statusImuAcquisition), &podBase_t::handleMessage<agile::statusPod_t>);
    //podWorker.subscribe("statusStateEstimatorOrientV1",CALLINTERVAL_STATEESTIMATORORIENTV1,&(podWorker.statusStateEstimatorOrientV1), &podBase_t::handleMessage<agile::statusPod_t>);
    podWorker.subscribe("statusStateEstimatorOrientCF", CALLINTERVAL_STATEESTIMATORORIENTCF, &(podWorker.statusStateEstimatorOrientCF), &podBase_t::handleMessage<agile::statusPod_t>);
    podWorker.subscribe("statusDetectorVis", CALLINTERVAL_DETECTORVIS, &(podWorker.statusDetectorVis), &podBase_t::handleMessage<agile::statusPod_t>);
    podWorker.subscribe("statusRemoteController", CALLINTERVAL_REMOTECONTROLLER, &(podWorker.statusRemoteController), &podBase_t::handleMessage<agile::statusPod_t>);
    podWorker.subscribe("statusControllerPDPose", CALLINTERVAL_CONTROLLERPDPOSE, &(podWorker.statusControllerPDPose), &podBase_t::handleMessage<agile::statusPod_t>);
    podWorker.subscribe("statusControllerPDOrient", CALLINTERVAL_CONTROLLERPDORIENT, &(podWorker.statusControllerPDOrient), &podBase_t::handleMessage<agile::statusPod_t>);
    podWorker.subscribe("statusControllerSOCOrient", CALLINTERVAL_CONTROLLERSOCORIENT, &(podWorker.statusControllerSOCOrient), &podBase_t::handleMessage<agile::statusPod_t>);
    podWorker.subscribe("statusControllerSOCPose", CALLINTERVAL_CONTROLLERSOCPOSE, &(podWorker.statusControllerSOCPose), &podBase_t::handleMessage<agile::statusPod_t>);
    podWorker.subscribe("statusMotorCommander", CALLINTERVAL_MOTORCOMMANDER, &(podWorker.statusMotorCommander), &podBase_t::handleMessage<agile::statusPod_t>);


    /*---------*/




    /*  POD-specific init procedures  */
    printf("Initializing...\n");
    podWorker.publishStatus(POD_INITING);	 // Update and publish status of watchdogPod (this POD is the first to start)

    podWorker.statusDrone.status = DRONE_WAITPODS;

    //Create message listener thead
    std::thread listenerThread(listen, &podWorker);

    // Create mainloop, add loop functions, run
    GMainLoop* mainloop = g_main_loop_new(NULL, FALSE);

    guint timerComputations = g_timeout_add(podWorker.callInterval, podBase_t::gtimerfuncComputations, (gpointer)&podWorker);
    guint timerStatusPod =    g_timeout_add(podWorker.callInterval, podBase_t::gtimerfuncStatusPod, (gpointer)&podWorker);

    printf("Initializing...DONE \n");
    printf("Drone waiting for all PODs to be ok ... \n");

    g_main_loop_run(mainloop);

    /*---------*/

    return 0;
}







