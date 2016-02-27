
#include "watchdog.hpp"

using namespace std;


void errorPrint(bool isSomePodCritical, bool isSomePodFatal, bool isStateCritical, bool isStateFatal, int16_t controlMode)
{
    string message = "-----\n\nProblem(s) with drone:\n ";

    if(isSomePodCritical) message 	= message + "> Some POD is critical\n";
    if(isSomePodFatal) message    	= message + "> Some POD is fatal\n";
    if(isStateCritical) message   	= message + "> Some state is critical\n";
    if(isStateFatal) message      	= message + "> Some state is fatal\n";
    if(controlMode == CMODE_FATAL) message 	= message + "> Control mode is FATAL\n";

    printf("%s at t=%" PRId64 ". Also check history above.\n-----\n\n", message.c_str(),GetTimeStamp());

}

bool checkPODstati(statusPod_t statusPod,watchdog_t* watchdog) //this is nasty, should be a member function, but then it cannot be called from within static podBase_t compuation function. See suggestion on github-issues on "can multiple pods be run simulatenously
    {
	bool statusMatch = false;
        std::string channelName;
	//std::string negativePod = "";

	agile::statusPod_t* statusMessage;

        messageAdmin_t::iterator iterator = watchdog->messageAdmin.begin();
        messageAdmin_t::iterator end = watchdog->messageAdmin.end();

	//watchDog does not list its on status in messageAdmin!
	statusMatch = (watchdog->statusPod.status == statusPod); if (statusMatch) printf("watchdogPod bad, with POD status %d\n",watchdog->statusPod.status);			

	//check all other Podstati subscribed to
        while(((!statusMatch) && (iterator != end)))
        {
	    //check if channel is status channel
	    channelName = iterator->first;	 
	    if (channelName.substr(0,5)=="status") 

		{
		statusMessage = reinterpret_cast<agile::statusPod_t*>(iterator->second.message);//this is crude;

		    statusMatch = (statusMessage->status==statusPod);

		    if (statusMatch)
		    {
			printf("bad Pod: %s with POD status %d\n",channelName.substr(6).c_str(),statusMessage->status);			
		    }
		}
            ++iterator;
        }

	return statusMatch;
    };


/*
Implementation of loop function for computations in this specific POD
*/

bool watchdog_t::doComputations()
{
    watchdog_t* podWorker = this;
    std::lock_guard<std::mutex> guard(podMutex);
    /*--------*/


    /* Computations */

    // statusDrone - computation via stateMachine (see statusDrone_StateMachine.pdf)

    //compute safety variables //@TODO replace this hardcoded mess: cycle through subscribed messages, check if .status available and then check that for ok/...
    agile::statusDrone_t statusDrone_old = statusDrone;

    /*bool isSomePodCritical = ((statusPod.status == POD_CRITICAL) || (statusImuAcquisition.status == POD_CRITICAL) || (statusStateEstimatorOrientCF.status == POD_CRITICAL) || (statusDetectorVis.status == POD_CRITICAL) || (statusRemoteController.status == POD_CRITICAL) || (statusControllerPDPose.status == POD_CRITICAL) || (statusControllerPDOrient.status == POD_CRITICAL) || (statusControllerSOCOrient.status == POD_CRITICAL) || (statusControllerSOCPose.status == POD_CRITICAL) || (statusMotorCommander.status == POD_CRITICAL));*/

   bool isSomePodCritical = checkPODstati(POD_CRITICAL,podWorker);

   /* bool isSomePodFatal = ((statusPod.status == POD_FATAL) || (statusImuAcquisition.status == POD_FATAL) ||  (statusStateEstimatorOrientCF.status == POD_FATAL) || (statusDetectorVis.status == POD_FATAL) || (statusRemoteController.status == POD_FATAL) || (statusControllerPDPose.status == POD_FATAL) || (statusControllerPDOrient.status == POD_FATAL) || (statusControllerSOCOrient.status == POD_FATAL) || (statusControllerSOCPose.status == POD_FATAL) || (statusMotorCommander.status == POD_FATAL));*/

   bool isSomePodFatal = checkPODstati(POD_FATAL,podWorker);

    bool isAllPodsOK = ((statusPod.status == POD_OK) && (statusImuAcquisition.status == POD_OK) && (statusStateEstimatorOrientCF.status == POD_OK) && (statusDetectorVis.status == POD_OK) && (statusRemoteController.status == POD_OK) && (statusControllerPDPose.status == POD_OK) && (statusControllerPDOrient.status == POD_OK) && (statusControllerSOCOrient.status == POD_OK) && (statusControllerSOCPose.status == POD_OK) && (statusMotorCommander.status == POD_OK));


    double yaw = 0.0;
    double pitch = 0.0;
    double roll = 0.0;
    quat2Euler(stateVariances.orient, &(yaw), &(pitch), &(roll));

    //@TODO anguarl rates in pqr bodyframe?
    //Check for critical state @TODO pqr rates or [0..2]?

    bool isStateCritical = ((abs(stateVariances.position[0]) > MAXPOS_CRITICAL) || (abs(stateVariances.position[1]) > MAXPOS_CRITICAL) || (abs(stateVariances.position[3]) > MAXALT_CRITICAL) || (abs(yaw) > MAXORIENT_Y_CRITICAL)  || (abs(pitch) > MAXORIENT_RP_CRITICAL) || (abs(roll) > MAXORIENT_RP_CRITICAL) || (abs(stateVariances.veloPositionBody[0]) > MAXVELPOS_CRITICAL) || (abs(stateVariances.veloPositionBody[1]) > MAXVELPOS_CRITICAL) || (abs(stateVariances.veloPositionBody[2]) > MAXVELPOS_CRITICAL)  || (abs(stateVariances.veloOrientBody[0]) > MAXVELORIENT_PQ_CRITICAL) || (abs(stateVariances.veloOrientBody[1]) > MAXVELORIENT_PQ_CRITICAL)  || (abs(stateVariances.veloOrientBody[2]) > MAXVELORIENT_R_CRITICAL) || (abs(imuRaw.accel[0]) > MAXACC_CRITICAL) || (abs(imuRaw.accel[1]) > MAXACC_CRITICAL) || (abs(imuRaw.accel[2]) > MAXACC_CRITICAL));

    bool isStateFatal = ((abs(stateVariances.position[0]) > MAXPOS_FATAL) || (abs(stateVariances.position[1]) > MAXPOS_FATAL) || (abs(stateVariances.position[3]) > MAXALT_FATAL) || (abs(yaw) > MAXORIENT_Y_FATAL)  || (abs(pitch) > MAXORIENT_RP_FATAL) || (abs(roll) > MAXORIENT_RP_FATAL) || (abs(stateVariances.veloPositionBody[0]) > MAXVELPOS_FATAL) || (abs(stateVariances.veloPositionBody[1]) > MAXVELPOS_FATAL) || (abs(stateVariances.veloPositionBody[2]) > MAXVELPOS_FATAL)  || (abs(stateVariances.veloOrientBody[0]) > MAXVELORIENT_PQ_FATAL) || (abs(stateVariances.veloOrientBody[1]) > MAXVELORIENT_PQ_FATAL)  || (abs(stateVariances.veloOrientBody[2]) > MAXVELORIENT_R_FATAL) || (abs(imuRaw.accel[0]) > MAXACC_FATAL) || (abs(imuRaw.accel[1]) > MAXACC_FATAL) || (abs(imuRaw.accel[2]) > MAXACC_FATAL));


    //"state machine"
    switch(statusDrone_old.status)
    {
    case DRONE_WAITPODS:
        if(isAllPodsOK)
        {
            statusDrone.status = DRONE_IDLE;
            printf("++drone from waitPodsOK to IDLE++\n");
        };
        break;

    case DRONE_IDLE:
        if(isSomePodFatal || (controlMode.controlMode == CMODE_FATAL) || isSomePodCritical || (controlMode.controlMode == CMODE_CRITICAL))
        {
            statusDrone.status = DRONE_FATAL;
            printf("++drone from IDLE to FATAL++\n");
            errorPrint(isSomePodCritical, isSomePodFatal, isStateCritical, isStateFatal, controlMode.controlMode);
        }

        else if(isAllPodsOK && (controlMode.controlMode > CMODE_NULL))
        {
            statusDrone.status = DRONE_TAKEOFF;
            takeoffMoment = GetTimeStamp();
            printf("++drone from IDLE to TAKEOFF++\n");
        }
        break;

    case DRONE_TAKEOFF:
        if(isStateFatal || isSomePodFatal || (controlMode.controlMode == CMODE_FATAL))
        {
            statusDrone.status = DRONE_FATAL;
            printf("++drone from TAKEOFF to FATAL++\n");
            errorPrint(isSomePodCritical, isSomePodFatal, isStateCritical, isStateFatal, controlMode.controlMode);
        }
        else if(isStateCritical || isSomePodCritical || (controlMode.controlMode == CMODE_CRITICAL))
        {
            statusDrone.status = DRONE_CRITICAL;
            printf("++drone from TAKEOFF to CRITICAL++\n");
            errorPrint(isSomePodCritical, isSomePodFatal, isStateCritical, isStateFatal, controlMode.controlMode);
        }
        else if((GetTimeStamp() - takeoffMoment) > TAKEOFFINTERVAL * MS2US)
        {
            statusDrone.status = DRONE_FLY;
            printf("++drone from TAKEOFF to FLY++\n");
        }

        break;
    case DRONE_FLY:
        if(isStateFatal || isSomePodFatal || (controlMode.controlMode == CMODE_FATAL))
        {
            statusDrone.status = DRONE_FATAL;
            printf("++drone from FLY to FATAL++\n");
            errorPrint(isSomePodCritical, isSomePodFatal, isStateCritical, isStateFatal, controlMode.controlMode);
        }
        else if(isStateCritical || isSomePodCritical || (controlMode.controlMode == CMODE_CRITICAL))
        {
            statusDrone.status = DRONE_CRITICAL;
            printf("++drone from FLY to CRITICAL\n");
            errorPrint(isSomePodCritical, isSomePodFatal, isStateCritical, isStateFatal, controlMode.controlMode);
        }

        break;

    case DRONE_CRITICAL:
        if(isStateFatal || isSomePodFatal || (controlMode.controlMode == CMODE_FATAL))
        {
            statusDrone.status = DRONE_FATAL;
            printf("++drone from CRITICAL to FATAL++\n");
            errorPrint(isSomePodCritical, isSomePodFatal, isStateCritical, isStateFatal, controlMode.controlMode);
        }
        else if(!isStateCritical && !isSomePodCritical && (controlMode.controlMode > CMODE_FATAL))
        {
            statusDrone.status = DRONE_FLY;
            printf("++drone from CRITICAL to FLY\n");
        }
        break;

    case DRONE_FATAL: ; break;

    default: statusDrone.status = DRONE_FATAL;
    }

    statusDrone.timestampJetson = GetTimeStamp();

    /*---------*/


    /* Publishing */

    // statusDrone - publish
    lcm.publish("statusDrone", &statusDrone);

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

bool watchdog_t::updateStatus()
{
    watchdog_t* podWorker = this;
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

    }

    /*---------*/

    /*Publishing statusPOD (keep this infrastructure!)*/
    publishStatus(statusPod.status);
    /*---------*/

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

    if(podWorker.stateVariancesChannel == "stateVariancesSim")
        podWorker.subscribe(podWorker.stateVariancesChannel, CALLINTERVAL_SIMULATOR, &(podWorker.stateVariances), &podBase_t::handleMessage<agile::stateVariances_t>);
    else if(podWorker.stateVariancesChannel == "stateVariancesOrientCF")
        podWorker.subscribe(podWorker.stateVariancesChannel, CALLINTERVAL_STATEESTIMATORORIENTCF, &(podWorker.stateVariances), &podBase_t::handleMessage<agile::stateVariances_t>);
    /*else if (podWorker.stateVariancesChannel == "stateVariancesOrientV1")
      podWorker.subscribe(podWorker.stateVariancesChannel,CALLINTERVAL_STATEESTIMATORORIENTV1, &(podWorker.stateVariances), &podBase_t::handleMessage<agile::stateVariances_t>);*/

    podWorker.subscribe(podWorker.imuRawChannel, CALLINTERVAL_IMUACQUISITION * 2, &(podWorker.imuRaw), &podBase_t::handleMessage<agile::imuRaw_t>); //@TODO hack: callinterval x2 to prevent shut downs from this delay
    podWorker.subscribe("controlMode", CALLINTERVAL_REMOTECONTROLLER*AGEMSG_X_TUNNEL, &(podWorker.controlMode), &podBase_t::handleMessage<agile::controlMode_t>); 

    podWorker.subscribe("statusImuAcquisition", CALLINTERVAL_IMUACQUISITION, &(podWorker.statusImuAcquisition), &podBase_t::handleMessage<agile::statusPod_t>);
    //podWorker.subscribe("statusStateEstimatorOrientV1",CALLINTERVAL_STATEESTIMATORORIENTV1,&(podWorker.statusStateEstimatorOrientV1), &podBase_t::handleMessage<agile::statusPod_t>);
    podWorker.subscribe("statusStateEstimatorOrientCF", CALLINTERVAL_STATEESTIMATORORIENTCF, &(podWorker.statusStateEstimatorOrientCF), &podBase_t::handleMessage<agile::statusPod_t>);
    podWorker.subscribe("statusDetectorVis", CALLINTERVAL_DETECTORVIS, &(podWorker.statusDetectorVis), &podBase_t::handleMessage<agile::statusPod_t>);
    podWorker.subscribe("statusRemoteController", CALLINTERVAL_REMOTECONTROLLER*AGEMSG_X_TUNNEL, &(podWorker.statusRemoteController), &podBase_t::handleMessage<agile::statusPod_t>); 
    podWorker.subscribe("statusControllerPDPose", CALLINTERVAL_CONTROLLERPDPOSE, &(podWorker.statusControllerPDPose), &podBase_t::handleMessage<agile::statusPod_t>);
    podWorker.subscribe("statusControllerPDOrient", CALLINTERVAL_CONTROLLERPDORIENT, &(podWorker.statusControllerPDOrient), &podBase_t::handleMessage<agile::statusPod_t>);
    podWorker.subscribe("statusControllerSOCOrient", CALLINTERVAL_CONTROLLERSOCORIENT, &(podWorker.statusControllerSOCOrient), &podBase_t::handleMessage<agile::statusPod_t>);
    podWorker.subscribe("statusControllerSOCPose", CALLINTERVAL_CONTROLLERSOCPOSE, &(podWorker.statusControllerSOCPose), &podBase_t::handleMessage<agile::statusPod_t>);
    podWorker.subscribe("statusMotorCommander", CALLINTERVAL_MOTORCOMMANDER, &(podWorker.statusMotorCommander), &podBase_t::handleMessage<agile::statusPod_t>);


    /*---------*/




    /*  POD-specific init procedures  */
    printf("Initializing...\n");
    usleep(1500000);				 //imuAcquisition waits for watchdogs' POD_Initing and might miss it if too early @TODO maybe add proper waiting-for-imuAcquisition-message
    podWorker.publishStatus(POD_INITING);	 // Update and publish status of watchdogPod (this POD is the second to start after imuAcquisition)

    podWorker.statusDrone.status = DRONE_WAITPODS;
    podWorker.initComputationInterval();

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







