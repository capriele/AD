#include "controllerPDPose.hpp"

using namespace std;





/*
Implementation of loop function for computations in this specific POD
*/

gboolean podBase_t::gtimerfuncComputations (gpointer data) {
	
	  /* General Infrastructure (maintain this structure!) */	  
	  controllerPDPose_t* podWorker = (controllerPDPose_t*) data;
	  std::lock_guard<std::mutex> guard(podMutex);
	  /*--------*/
   	  
  	  /* Computations */
	  double yaw_hat, pitch_hat, roll_hat;
	  quat2Euler(podWorker->stateVariances.orient,&(yaw_hat),&(pitch_hat),&(roll_hat));	  


	 //compute euler orientation reference from global position error

	 double deltax = cos(yaw_hat)*(podWorker->poseRef.position[0] - podWorker->stateVariances.position[0]) + sin(yaw_hat)*(podWorker->poseRef.position[1] - podWorker->stateVariances.position[1]);
	 double deltay = -sin(yaw_hat)*(podWorker->poseRef.position[0] - podWorker->stateVariances.position[0]) + cos(yaw_hat)*(podWorker->poseRef.position[1] - podWorker->stateVariances.position[1]);
	 double deltaz = podWorker->poseRef.position[2] - podWorker->stateVariances.position[2];

	
	 double pitch_ref = -podWorker->powerAdjust.pXYAdjustPDPOS*Pxy*deltax + podWorker->powerAdjust.dXYAdjustPDPOS*Dxy*podWorker->stateVariances.veloPositionBody[0];
	 double roll_ref = podWorker->powerAdjust.pXYAdjustPDPOS*Pxy*deltay - podWorker->powerAdjust.dXYAdjustPDPOS*Dxy*podWorker->stateVariances.veloPositionBody[1];


	  //PD control state error to torque-equivalent about bodyframe axis
	  
	  double tau_p 		= podWorker->powerAdjust.ppAdjustPDO*Pp*(pitch_ref - pitch_hat) - podWorker->powerAdjust.dpAdjustPDO*Dp*podWorker->stateVariances.veloOrientBody[1] + podWorker->powerAdjust.pBiasPDO; //@TODO check convention, see PODdescriptions!
	  double tau_r 		= podWorker->powerAdjust.prAdjustPDO*Pr*(roll_ref - roll_hat)  - podWorker->powerAdjust.drAdjustPDO*Dr*podWorker->stateVariances.veloOrientBody[0] + podWorker->powerAdjust.rBiasPDO;	
	  double tau_y 		= podWorker->powerAdjust.pyAdjustPDO*Py*(podWorker->poseRef.orientEuler[0] - yaw_hat)   - podWorker->powerAdjust.dyAdjustPDO*Dy*podWorker->stateVariances.veloOrientBody[2] + podWorker->powerAdjust.yBiasPDO;	  
	  double totalThrust =    podWorker->powerAdjust.pZAdjustPDPOS*Pz*deltaz - podWorker->powerAdjust.dZAdjustPDPOS*Dz*podWorker->stateVariances.veloPositionBody[2] - QUADMASS*GRAVITY - podWorker->powerAdjust.tBiasPDO; // "-" because thrust "upwards" is equals negative thrust!

	  //printf("%f \n",podWorker->powerAdjust.tBiasPDO);

	  //printf("%f \n",podWorker->poweradjust.totalthrustGain);
  	  //control mixer that translates into required RPM (note: this is not too sound, desired torque generated by PD should translate via sqrt-function to RPM, not linearly, but it works (on Parrot's RS with MIT ROSMAT)


	  //printf("delay estimated state to now: %f\n",(GetTimeStamp()-podWorker->stateVariances.timestampJetson)/1000000.0);
	   
	  podWorker->motorsWsRef.wsRef[0] = sqrt(-THRUST2OMEGA2*(ATOTALTHRUST*totalThrust + ATAUYAW*tau_y - ATAUPR*tau_p - ATAUPR*tau_r));
	  podWorker->motorsWsRef.wsRef[1] = sqrt(-THRUST2OMEGA2*(ATOTALTHRUST*totalThrust - ATAUYAW*tau_y - ATAUPR*tau_p + ATAUPR*tau_r));
	  podWorker->motorsWsRef.wsRef[2] = sqrt(-THRUST2OMEGA2*(ATOTALTHRUST*totalThrust + ATAUYAW*tau_y + ATAUPR*tau_p + ATAUPR*tau_r));
	  podWorker->motorsWsRef.wsRef[3] = sqrt(-THRUST2OMEGA2*(ATOTALTHRUST*totalThrust - ATAUYAW*tau_y + ATAUPR*tau_p - ATAUPR*tau_r));
	

	  podWorker->motorsWsRef.timestampJetson = GetTimeStamp();  

	  /*---------*/


	  /* Publishing */ 

	  // statusDrone - publish
	  podWorker->lcm.publish ("motorsWsRefPDPose", &podWorker->motorsWsRef); 	//choose channelName here!	

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
	  controllerPDPose_t* podWorker = (controllerPDPose_t*) data;	  
	  /*---------*/

	  /*Computation statusPOD*/	  	  
	   if (podWorker->computationInterval > MAXPODDELAY_X*podWorker->callInterval*MS2US)	
		{
		printf("controllerPDPose: delay occured of % " PRId64 "us!\n",podWorker->computationInterval);
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
  controllerPDPose_t podWorker = controllerPDPose_t("controllerPDPose",CALLINTERVAL_CONTROLLERPDPOSE); 	//provide your PODname here! 

  if (argc<2) {
		printf("Please provide estimationChannel that controller will use!\n");
		return EXIT_FAILURE;
		};

  podWorker.stateVariancesChannel = argv[1];

  // 2) Create LCM
  if (!podWorker.lcm.good()) 
    return 1;

  // 3) Subscribe this POD to channels
  if (strcmp(podWorker.stateVariancesChannel.c_str(),"stateVariancesOrientV1")==0)
    podWorker.subscribe(podWorker.stateVariancesChannel.c_str(), CALLINTERVAL_STATEESTIMATORORIENTV1, 	 &(podWorker.stateVariances), 	&podBase_t::handleMessage<agile::stateVariances_t>);
  else if (strcmp(podWorker.stateVariancesChannel.c_str(),"stateVariancesOrientCF")==0)
     podWorker.subscribe(podWorker.stateVariancesChannel.c_str(), CALLINTERVAL_STATEESTIMATORORIENTCF, 	 &(podWorker.stateVariances), 	&podBase_t::handleMessage<agile::stateVariances_t>);
  else podWorker.subscribe(podWorker.stateVariancesChannel.c_str(), CALLINTERVAL_SIMULATOR, 	 &(podWorker.stateVariances), 	&podBase_t::handleMessage<agile::stateVariances_t>);

  podWorker.subscribe("poseRef",  CALLINTERVAL_REMOTECONTROLLER, &(podWorker.poseRef), &podBase_t::handleMessage<agile::poseRef_t>);
  podWorker.subscribe("powerAdjust",  CALLINTERVAL_REMOTECONTROLLER, &(podWorker.powerAdjust), &podBase_t::handleMessage<agile::powerAdjust_t>);	

  /*---------*/

  


  /*  POD-specific init procedures  */
  // Update and publish status of controllerPDPose_t (this POD is the first to start)
  podWorker.publishStatus(POD_INITING);	

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