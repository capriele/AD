#include "controllerSOCOrient.hpp"


using namespace std;


void controller_load(struct Controller * c,std::string filename)
{
  char filename2[256];
  strcpy(filename2,filename.c_str());
  c->controls = ft1d_array_load(filename2);
  if (c->controls == NULL){
    fprintf(stderr,"Loaded Controller is NULL\n");
    exit(1);
  }
}

void controller_free(struct Controller * c)
{
  if (c != NULL){
    ft1d_array_free(c->controls); c->controls = NULL;
  }
}

void controller_eval(struct Controller * c, double * state, double * controls)
{
  ft1d_array_eval2(c->controls,state,controls);
}

//


/*
Implementation of loop function for computations in this specific POD
*/


void controllerSOCOrient_t::getControlleraction(double * orientState, double * torquesRefs)
{

  controller_eval(&(this->controller),orientState,torquesRefs);

}

/*
Implementation of loop function for computations in this specific POD
*/


bool controllerSOCOrient_t::doComputations()
{
    controllerSOCOrient_t* podWorker = this;
    std::lock_guard<std::mutex> guard(podMutex);
    /*--------*/

    /* Computations */

    //compute euler angle estimate from quaternion estimates
    double yaw_hat, pitch_hat, roll_hat;
    quat2Euler(stateVariances.orient, &(yaw_hat), &(pitch_hat), &(roll_hat));

    double torquesRefs[3]; //output of Alex orientation-controller: torques about body-axes x,y,z (see Dropbox (MIT)\AgileDrones\writeups_docs\architecture\controlarchitecture\orientation_model/quadrotordynamics.pdf)

    double orientState[6]; //orientation-related state: euler angles yaw pitch roll + angular rates about body-axes x,y,z
    orientState[0] = yaw_hat; //@TODO replace this by error (angle_hat - angle_ref) if we want to enable the user to control the angles for non-zero references
    orientState[1] = pitch_hat;
    orientState[2] = roll_hat;
    orientState[3] = stateVariances.veloOrientBody[0];
    orientState[4] = stateVariances.veloOrientBody[1];
    orientState[5] = stateVariances.veloOrientBody[2];

    //get optimal torque references from Alex controller
    getControlleraction(orientState, torquesRefs);

    //compute total thrust as function of user-requested thrust
    double totalThrust = - QUADMASS * GRAVITY - powerAdjust.tBiasPDO;

    //convert [totalthrust;body torque references] into the four motor speeds
    motorsWsRef.wsRef[0] = sqrt(-THRUST2OMEGA2 * (ATOTALTHRUST * totalThrust + ATAUYAW * torquesRefs[2] - ATAUPR * torquesRefs[1] - ATAUPR * torquesRefs[0]));
    motorsWsRef.wsRef[1] = sqrt(-THRUST2OMEGA2 * (ATOTALTHRUST * totalThrust - ATAUYAW * torquesRefs[2] - ATAUPR * torquesRefs[1] + ATAUPR * torquesRefs[0]));
    motorsWsRef.wsRef[2] = sqrt(-THRUST2OMEGA2 * (ATOTALTHRUST * totalThrust + ATAUYAW * torquesRefs[2] + ATAUPR * torquesRefs[1] + ATAUPR * torquesRefs[0]));
    motorsWsRef.wsRef[3] = sqrt(-THRUST2OMEGA2 * (ATOTALTHRUST * totalThrust - ATAUYAW * torquesRefs[2] + ATAUPR * torquesRefs[1] - ATAUPR * torquesRefs[0]));


    motorsWsRef.timestampJetson = GetTimeStamp();

    /*---------*/


    /* Publishing */

    // statusDrone - publish
    lcm.publish("motorsWsRefSOCOrient", &motorsWsRef); 	//choose channelName here!

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

bool controllerSOCOrient_t::updateStatus()
{
    controllerSOCOrient_t* podWorker = this;
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

    /*Publishing statusPOD (keep this infrastructure!)*/
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
    controllerSOCOrient_t podWorker = controllerSOCOrient_t("controllerSOCOrient", CALLINTERVAL_CONTROLLERSOCORIENT); 	//provide your PODname here!
    //podWorker.controllerFilePath = "controllerSOCOrient.ft";

    if(argc < 3)
    {
        printf("Please provide estimationChannel and controller's filename that controller will use!\n");
        return EXIT_FAILURE;
    };

    podWorker.stateVariancesChannel = argv[1];
    podWorker.controllerFilePath    = argv[2];

    // 2) Create LCM
    if(!podWorker.lcm.good())
        return 1;

    // 3) Subscribe this POD to channels

    //channel that provides state-estimates
    if(podWorker.stateVariancesChannel == "stateVariancesOrientV1")
        podWorker.subscribe(podWorker.stateVariancesChannel, CALLINTERVAL_STATEESTIMATORORIENTV1, 	 &(podWorker.stateVariances), 	&podBase_t::handleMessage<agile::stateVariances_t>);
    else if(podWorker.stateVariancesChannel == "stateVariancesOrientCF")
        podWorker.subscribe(podWorker.stateVariancesChannel, CALLINTERVAL_STATEESTIMATORORIENTCF, 	 &(podWorker.stateVariances), 	&podBase_t::handleMessage<agile::stateVariances_t>);
    else podWorker.subscribe(podWorker.stateVariancesChannel, CALLINTERVAL_SIMULATOR, 	 &(podWorker.stateVariances), 	&podBase_t::handleMessage<agile::stateVariances_t>);

    //other channels
    podWorker.subscribe("poseRef",  CALLINTERVAL_REMOTECONTROLLER*AGEMSG_X_TUNNEL, &(podWorker.poseRef), &podBase_t::handleMessage<agile::poseRef_t>);
    podWorker.subscribe("powerAdjust",  CALLINTERVAL_REMOTECONTROLLER*AGEMSG_X_TUNNEL, &(podWorker.powerAdjust), &podBase_t::handleMessage<agile::powerAdjust_t>);

    /*---------*/




    /*  POD-specific init procedures  */
    // Update and publish status of controllerSOCOrient_t (this POD is the first to start)
    podWorker.publishStatus(POD_INITING);

    //Load controller lookup



    controller_load(&(podWorker.controller),podWorker.controllerFilePath);
   
    

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

    guint timerComputations = g_timeout_add(podWorker.callInterval, podBase_t::gtimerfuncComputations, (gpointer)&podWorker);
    guint timerStatusPod =    g_timeout_add(podWorker.callInterval, podBase_t::gtimerfuncStatusPod, (gpointer)&podWorker);

    g_main_loop_run(mainloop);

    /*---------*/

    return 0;
}
