#include "controllerSOCOrient.hpp"

using namespace std;





/*
Implementation of loop function for computations in this specific POD
*/

gboolean podBase_t::gtimerfuncComputations (gpointer data) {
	
	  /* General Infrastructure (maintain this structure!) */	  
	  controllerSOCOrient_t* podWorker = (controllerSOCOrient_t*) data;
	  std::lock_guard<std::mutex> guard(podMutex);
	  /*--------*/
   	  
  	  /* Computations */

	  //PD control state error to torque-equivalent about bodyframe axis
	  double yaw_hat, pitch_hat, roll_hat;
	  quat2Euler(podWorker->stateVariances.orient,&(yaw_hat),&(pitch_hat),&(roll_hat));	  

	  double totalThrust_usergain = podWorker->powerAdjust.tBiasPDO/(QUADMASS*GRAVITY/cos(pitch_hat)/cos(roll_hat)/ALPHAT);

	  double zetas[3]; //squared motor speeds, output of Alex controller
	
	  double orientState[6];
	  orientState[0] = yaw_hat;
	  orientState[1] = pitch_hat;
	  orientState[2] = roll_hat;
	  orientState[3] = podWorker->stateVariances.veloOrientBody[0];
	  orientState[4] = podWorker->stateVariances.veloOrientBody[1];
	  orientState[5] = podWorker->stateVariances.veloOrientBody[2];
	
	  getResultsControllerSOCOrient(orientState,podWorker.controllerLookup,zetas);
	   
	  podWorker->motorsWsRef.wsRef[0] = sqrt(totalThrust_usergain*zetas[0]);
	  podWorker->motorsWsRef.wsRef[1] = sqrt(totalThrust_usergain*zetas[1]);
	  podWorker->motorsWsRef.wsRef[2] = sqrt(totalThrust_usergain*zetas[2]);
	  podWorker->motorsWsRef.wsRef[3] = sqrt(totalThrust_usergain*(QUADMASS*GRAVITY/cos(pitch_hat)/cos(roll_hat)/ALPHAT-zetas[0]-zetas[1]-zetas[2]));
	

	  podWorker->motorsWsRef.timestampJetson = GetTimeStamp();  

	  /*---------*/


	  /* Publishing */ 

	  // statusDrone - publish
	  podWorker->lcm.publish ("motorsWsRefSOCOrient", &podWorker->motorsWsRef); 	//choose channelName here!	

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

gboolean podBase_t::gtimerfuncStatusPod (gpointer data) {

	  /*General Infrastructure (maintain this infrastructure!)*/
	  controllerSOCOrient_t* podWorker = (controllerSOCOrient_t*) data;	  
	  /*---------*/

	  /*Computation statusPOD*/	  	  
	   if (podWorker->computationInterval > MAXPODDELAY_X*podWorker->callInterval*MS2US)	
		{
		printf("controllerSOCOrient: delay occured of % " PRId64 "us!\n",podWorker->computationInterval);
		podWorker->statusPod.status = POD_FATAL;
		}
	  else if (!podWorker->checkMessagesUptodate())
		{		
		podWorker->statusPod.status = POD_CRITICAL;
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

int main (int argc, char** argv) {

  /* General Infrastructure: setup (maintain this structure!)  */  

  // 1) Create the app
  controllerSOCOrient_t podWorker = controllerSOCOrient_t("controllerSOCOrient",CALLINTERVAL_controllerSOCOrient); 	//provide your PODname here! 
  podWorker.controllerFilePath = "controllerSOCOrient.bin";

  if (argc<2) {
		printf("Please provide estimationChannel that controller will use!\n");
		return EXIT_FAILURE;
		};

  podWorker.stateVariancesChannel = argv[1];

  // 2) Create LCM
  if (!podWorker.lcm.good()) 
    return 1;

  // 3) Subscribe this POD to channels
  //@TODO add config.file or GUI that sets which channel to subscribe to
  podWorker.subscribe(podWorker.stateVariancesChannel.c_str(), CALLINTERVAL_SIMULATOR, 	 &(podWorker.stateVariances), 	&podBase_t::handleMessage<agile::stateVariances_t>);
  //podWorker.subscribe("stateVariancesSim", CALLINTERVAL_SIMULATOR, 	 &(podWorker.stateVariances), 	&podBase_t::handleMessage<agile::stateVariances_t>);
  //podWorker.subscribe("stateVariancesOrientV1", CALLINTERVAL_SIMULATOR, 	 &(podWorker.stateVariances), 	&podBase_t::handleMessage<agile::stateVariances_t>); 

  podWorker.subscribe("poseRef",  CALLINTERVAL_REMOTECONTROLLER, &(podWorker.poseRef), &podBase_t::handleMessage<agile::poseRef_t>);
  podWorker.subscribe("powerAdjust",  CALLINTERVAL_REMOTECONTROLLER, &(podWorker.powerAdjust), &podBase_t::handleMessage<agile::powerAdjust_t>);	

  /*---------*/

  


  /*  POD-specific init procedures  */
  // Update and publish status of controllerSOCOrient_t (this POD is the first to start)
  podWorker.publishStatus(POD_INITING);	

  //Load controller lookup
  
  if ((podWorker.controllerFileHandle = fopen(podWorker.controllerFilePath, "rb")) == NULL)
			{
				printf("ERROR opening controller file!\n");
				return 1;
			}
  else fread(podWorker.controllerLookup, sizeof(controllerLookup_t), 1, podWorker.controllerFileHandle);

  /*---------*/



  /*  General Infrastructure: loops (maintain this structure!)  */

  //  POD-wait for watchdogPOD to be up and running (POD_OK)  before continue

  printf("Waiting for watchdog to be POD_OK...\n");
  while (podWorker.statusWatchdog.status!=POD_OK)
	{
	podWorker.lcm.handle();
	}
  printf("Watchdog POD_OK! Continuing...\n");
  podWorker.initComputationInterval();

  //Create message listener thead	
  std::thread listenerThread(listen,&podWorker);

  // Create mainloop, add loop functions, run
  GMainLoop* mainloop = g_main_loop_new (NULL, FALSE);

  guint timerComputations = g_timeout_add (podWorker.callInterval, podBase_t::gtimerfuncComputations, (gpointer)&podWorker);  
  guint timerStatusPod =    g_timeout_add (podWorker.callInterval, podBase_t::gtimerfuncStatusPod,    (gpointer)&podWorker);	

  g_main_loop_run (mainloop);

  /*---------*/

  return 0;
}
