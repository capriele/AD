
#include "poseEstimVis.hpp"

using namespace std;


/*
Implementation of loop function for computations in this specific POD
*/

gboolean podBase_t::gtimerfuncComputations (gpointer data) {
	
	  /* General Infrastructure (keep this infrastructure!) */  	  
	  poseEstimVis_t* podWorker = (poseEstimVis_t*) data;
	  std::lock_guard<std::mutex> guard(podMutex);

   	   /*--------*/


  	  /* Computations */	
	  /*---example that does not really compute anything---*/


		//find example for position reconstruction in codebase_lowlevel/vision/locator.cpp

	  //displays data from channel "testdata"
	  printf(" data: %f\n",podWorker->testdata.position[2]);	

	  // statusDrone - computation via stateMachine 
	  agile::statusDrone_t statusDrone_old = podWorker->statusDrone; 	    
 	  podWorker->statusDrone = agile::statusDrone_t ();
	  
	  	
	  switch(statusDrone_old.status)
	  {   
	  //implement safety statemachine
	    case DRONE_WAITPODS: ; break;
	    case DRONE_IDLE: ; break;
	    case DRONE_TAKEOFF: ; break;
	    case DRONE_FLY: ; break;
	    case DRONE_CRITICAL: ; break;
	    case DRONE_FATAL: ; break;
	  }
	  podWorker->statusDrone.status = DRONE_WAITPODS;
	  podWorker->statusDrone.timestampJetson = GetTimeStamp();  

	  /*---------*/


	  /* Publishing computation result, in this case, the status update of drone status */ 

	  // statusDrone - publish
	  podWorker->lcm.publish ("statusDronePUBLISHONEXMAPLECHANNEL", &podWorker->statusDrone); 	//choose channelName here!	

	  /*---------*/



	  /*General Infrastructure (keep this infrastructure!)*/
	 podWorker->updateComputationInterval();
	 return TRUE;
	 /*---------*/
	}
/*--------------------------*/





/*
Implementation of loop function for publishing statusPod
*/

gboolean podBase_t::gtimerfuncStatusPod (gpointer data) {

	  /*General Infrastructure (keep this infrastructure!)*/
	  poseEstimVis_t* podWorker = (poseEstimVis_t*) data;
	  /*---------*/

	  /*Computation statusPOD*/
	  /*--compute updated pod status--*/	  	  
	   if (podWorker->computationInterval > MAXPODDELAY_X*podWorker->callInterval*MS2US)	
		{
		printf("poseEstimVis: delay occured of % " PRId64 "us!\n",podWorker->computationInterval);
		podWorker->statusPod.status = POD_FATAL;
		}
	   else if ( (podWorker->checkMessagesUptodate() == MSGS_LATE) )
		{
		podWorker->statusPod.status = POD_CRITICAL;
		}
	   else if ( (podWorker->checkMessagesUptodate() == MSGS_DEAD) )
		{
		podWorker->statusPod.status = POD_FATAL;
		}
	   else
		{
		podWorker->statusPod.status = POD_OK;
		};
	/*---------*/

	/*Publishing statusPOD (keep this infrastructure!)*/
	 podWorker->publishStatus(podWorker->statusPod.status);
	/*---------*/

	return TRUE;
};
/*--------------------------*/




/*
main to initialize and start the POD
*/

int main (int argc, char** argv) {

  /* General Infrastructure: setup (keep this infrastructure!)  */  

  // 1) Create the app
  poseEstimVis_t podWorker = poseEstimVis_t("poseEstimVis",CALLINTERVAL_poseEstimVis); 	//provide your PODname here! 

  // 2) Create LCM
  if (!podWorker.lcm.good()) 
    return 1;

  // 3) Subscribe this POD to channels
  podWorker.subscribe("testdata", 	CALLINTERVAL_TESTSENDER,  &(podWorker.testdata), 	&podBase_t::handleMessage<agile::pose_t>);
  podWorker.subscribe("statusTestsender",CALLINTERVAL_TESTSENDER, &(podWorker.statusTestsender), &podBase_t::handleMessage<agile::statusPod_t>);
  //podWorker.subscribe("statusWatchdog",  CALLINTERVAL_WATCHDOG, &(podWorker.statusWatchdog),   &podBase_t::handleMessage<agile::statusPod_t>); // IMPORTANT -> POD is autosubscribed to this (and statusDrone) via constructor of Pod-base-class!

  //podWorker.unsubscribe("statusTestsender"); //this is an example how to unsubscribe!

  
  
  

  /*---------*/

  


  /*  POD-specific init procedures  */
  // Update and publish status of poseEstimVis (keep this infrastructure!)
  printf("Initializing POD...\n");
  podWorker.publishStatus(POD_INITING);	

  //Initialization stuff
  //...	

  printf("Initializing POD... DONE\n");
  /*---------*/



  /*  General Infrastructure: loops (keep this infrastructure!)  */

  //  POD waits for watchdogPOD to be up and running (POD_OK)  before continues

  printf("Waiting for watchdog to be POD_OK...\n");
  while (podWorker.statusWatchdog.status!=POD_OK)
	{
	podWorker.lcm.handle();
	}
  printf("Watchdog POD_OK! Continuing...\n");
  podWorker.initComputationInterval();

  //Create message listener thread to listen to subscribed channels and stored received messages	
  std::thread listenerThread(listen,&podWorker);

  // Create mainloop, add loop functions, run
  GMainLoop* mainloop = g_main_loop_new (NULL, FALSE);

  guint timerComputations = g_timeout_add (podWorker.callInterval, podBase_t::gtimerfuncComputations, (gpointer)&podWorker);  
  guint timerStatusPod =    g_timeout_add (podWorker.callInterval, podBase_t::gtimerfuncStatusPod,    (gpointer)&podWorker);	

  g_main_loop_run (mainloop);

  /*---------*/

  return 0;
}







