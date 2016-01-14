
#include "remoteController.hpp"

using namespace std;


/*
de/increment and potentially saturatates values
*/
void crement(double* var, double step, double min, double max)
{
	*var += step;
	if (*var > max) {*var = max;};
	if (*var < min) {*var = min;};
	printf("%f (%f..%f)\n",*var,min,max);
};


/*
Implementation of loop function for computations in this specific POD
*/

gboolean podBase_t::gtimerfuncComputations (gpointer data) {

	  /* General Infrastructure (maintain this structure!) */
	  remoteController_t* podWorker = (remoteController_t*) data;	
  	  std::lock_guard<std::mutex> guard(podMutex);	  

	  /*--------*/
	  

  

  	  /* Computations */
	 int ch;
  	 remoteController_t::changemode(1);

		
	 if ( !remoteController_t::kbhit() )
	   	{
	   	//nothing new on keyboard
	   	}
	else
		{
		//we got sth on keyboard!
		ch = getchar();
		remoteController_t::changemode(0);
		
		//FLY MODE
		if (podWorker->controllerAdjustmode == CADJUSTMODE_NULL)		
			{
			switch (ch)
				{
				case ',': podWorker->controllerAdjustmode =  CADJUSTMODE_P; printf("Controlleradjust switched to P-values!\n");  break;
				case '.': podWorker->controllerAdjustmode =  CADJUSTMODE_D; printf("Controlleradjust switched to D-values!\n");  break;
				case '/':
					if (podWorker->controlMode.controlMode == CMODE_PIDORIENT)
					{
						podWorker->controllerAdjustmode =  CADJUSTMODE_BIAS;
						printf("Controlleradjust switched to Bias-values!\n");
					};
					break;


				//control mode cases
				case 'e': 
					podWorker->controlMode.controlMode = CMODE_FATAL;					
					podWorker->resetPitchRollReference();
					printf("Emergency issued!\n");  						
					break;
				case '6':
					podWorker->controlMode.controlMode = CMODE_PIDORIENT;					
					podWorker->resetPitchRollReference();
					printf("PIDOrient controller enabled, with ref. orient reset!\n");  						
					break;
				case '7':
					podWorker->controlMode.controlMode =  CMODE_PIDPOSE;
					podWorker->resetPitchRollReference();	
					printf("PIDPosition controller enabled, with ref. orient reset!\n");					
					break;
				case '8':
					podWorker->controlMode.controlMode =  CMODE_SOCORIENT;
					podWorker->resetPitchRollReference();
					printf("SOCOrient controller enabled, with ref. orient reset!\n");
					break;

				case '9':
					podWorker->controlMode.controlMode =  CMODE_SOCPOSE;
					podWorker->resetPitchRollReference();
					printf("SOCPOse controller enabled, with ref. orient reset!\n");
					break;


				//altitude-related references
				case 'i': 
					if (podWorker->controlMode.controlMode == CMODE_PIDPOSE) //increase reference altitude
						{
						podWorker->poseRef.position[2] -=  STEPSALTITUDE;
						}
					else if (podWorker->controlMode.controlMode == CMODE_PIDORIENT) //increase thrust bias in PDOrient controller
						{
						crement(&(podWorker->powerAdjust.tBiasPDO), +STEPSTBIASPDO, MINTBIASPDO, MAXTBIASPDO);
						printf("thrust-bias for PDOrient: %f\n",podWorker->powerAdjust.tBiasPDO);
						};
					 break;

				case 'k': 
					if (podWorker->controlMode.controlMode ==  CMODE_PIDPOSE)
						{
						podWorker->poseRef.position[2] +=  STEPSALTITUDE;
						if ( podWorker->poseRef.position[2] >=0) podWorker->poseRef.position[2]=0;
						}
					else if (podWorker->controlMode.controlMode ==  CMODE_PIDORIENT)
						{
						crement(&(podWorker->powerAdjust.tBiasPDO), -STEPSTBIASPDO, MINTBIASPDO, MAXTBIASPDO);
						printf("thrust-bias for PDOrient: %f\n",podWorker->powerAdjust.tBiasPDO);
						};
					 break;


				//orientaion-related references
				case 'a': podWorker->poseRef.orientEuler[2] -=  STEPSPITCHROLL; break;
				case 'd': podWorker->poseRef.orientEuler[2] +=  STEPSPITCHROLL; break;
				case 'w': podWorker->poseRef.orientEuler[1] -=  STEPSPITCHROLL; break;
				case 's': podWorker->poseRef.orientEuler[1] +=  STEPSPITCHROLL; break;
				case 'j': podWorker->poseRef.orientEuler[0] -=  STEPSYAW; break;
				case 'l': podWorker->poseRef.orientEuler[0] +=  STEPSYAW; break;
			
				//power adjustment to happen in motor commander
				case '+':
					crement(&(podWorker->powerAdjust.powerGain), +STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
					printf("powerGain in Poweradjuster: %f\n",podWorker->powerAdjust.powerGain);
					break;
				case '-':
					crement(&(podWorker->powerAdjust.powerGain), -STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
					printf("powerGain in Poweradjuster: %f\n",podWorker->powerAdjust.powerGain);	
					break;
				} //end switch

				//When pitch roll was controlled, set controlMode to orient control of pitch roll reference angle exists
				if ( (podWorker->controlMode.controlMode > CMODE_NULL) && ((ch=='a')||(ch=='d')||(ch=='w')||(ch=='s')) )
					{
					if (    (podWorker->controlMode.controlMode !=  CMODE_PIDORIENT) &&  ((abs(podWorker->poseRef.orientEuler[1])>EPSREMOTE) || (abs(podWorker->poseRef.orientEuler[2])>EPSREMOTE))    )
						{
						podWorker->controlMode.controlMode =  CMODE_PIDORIENT;
						printf("auto-switch to PIDOrient controller, with ref. orient reset!\n");  
						}
					}
			} //endif flymode

		//CONTROLLER PDORIENT ADJUSTMODE BIAS
		else if ((podWorker->controlMode.controlMode == CMODE_PIDORIENT) && (podWorker->controllerAdjustmode == CADJUSTMODE_BIAS))
			{
			switch (ch)
				{
				//control mode cases
				case 'e': 
					podWorker->controlMode.controlMode =  CMODE_FATAL;					
					podWorker->resetPitchRollReference();						
					break;

				case 'm': podWorker->controllerAdjustmode =  CADJUSTMODE_NULL; printf("Controlleradjust mode turned off!\n"); break;				
				case '.': podWorker->controllerAdjustmode =  CADJUSTMODE_D;    printf("Controlleradjust switched to D-values!\n");  break;
				case ',': podWorker->controllerAdjustmode =  CADJUSTMODE_P;    printf("Controlleradjust switched to P-values!\n");  break;

				//thrust-related controller  adjustments
				case 'i': crement(&(podWorker->powerAdjust.tBiasPDO), +STEPSTBIASPDO, MINTBIASPDO, MAXTBIASPDO); printf("thrust-bias for PDOrient: %f\n",podWorker->powerAdjust.tBiasPDO); break;

				case 'k': crement(&(podWorker->powerAdjust.tBiasPDO), -STEPSTBIASPDO, MINTBIASPDO, MAXTBIASPDO); printf("thrust-bias for PDOrient: %f\n",podWorker->powerAdjust.tBiasPDO); break;

				//orient-related controller  adjustments
				case 'a': crement(&(podWorker->powerAdjust.rBiasPDO), -STEPSRBIASPDO, MINRBIASPDO, MAXRBIASPDO); break;
				case 'd': crement(&(podWorker->powerAdjust.rBiasPDO), +STEPSRBIASPDO, MINRBIASPDO, MAXRBIASPDO); break;
				case 'w': crement(&(podWorker->powerAdjust.pBiasPDO), -STEPSPBIASPDO, MINPBIASPDO, MAXPBIASPDO); break;
				case 's': crement(&(podWorker->powerAdjust.pBiasPDO), +STEPSPBIASPDO, MINPBIASPDO, MAXPBIASPDO); break;
				case 'j': crement(&(podWorker->powerAdjust.yBiasPDO), -STEPSYBIASPDO, MINYBIASPDO, MAXYBIASPDO); break;
				case 'l': crement(&(podWorker->powerAdjust.yBiasPDO), +STEPSYBIASPDO, MINYBIASPDO, MAXYBIASPDO); break;
			
				//power adjustment to happen in motor commander
				case '+':
					crement(&(podWorker->powerAdjust.powerGain), +STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
					printf("powerGain in Poweradjuster: %f\n",podWorker->powerAdjust.powerGain);
					break;
				case '-':
					crement(&(podWorker->powerAdjust.powerGain), -STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
					printf("powerGain in Poweradjuster: %f\n",podWorker->powerAdjust.powerGain);	
					break;
	
				}
			} //endif Bias-controladjuster	

		//CONTROLLER PDORIENT ADJUSTMODE P-value
		else if ((podWorker->controlMode.controlMode == CMODE_PIDORIENT) && (podWorker->controllerAdjustmode == CADJUSTMODE_P))
			{
			switch (ch)
				{
				//control mode cases
				case 'e': 
					podWorker->controlMode.controlMode =  CMODE_FATAL;					
					podWorker->resetPitchRollReference();						
					break;

				case 'm': podWorker->controllerAdjustmode =  CADJUSTMODE_NULL; printf("Controlleradjust mode turned off!\n"); break;				
				case '.': podWorker->controllerAdjustmode =  CADJUSTMODE_D; printf("Controlleradjust switched to D-values!\n");  break;
				case '/': podWorker->controllerAdjustmode =  CADJUSTMODE_BIAS; printf("Controlleradjust switched to Bias-values!\n");  break;

				//thrust-related controller  adjustments
				case 'i': crement(&(podWorker->powerAdjust.ptAdjustPDO), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
				case 'k': crement(&(podWorker->powerAdjust.ptAdjustPDO), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;						 

				//orient-related controller  adjustments
				case 'a': crement(&(podWorker->powerAdjust.prAdjustPDO), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
				case 'd': crement(&(podWorker->powerAdjust.prAdjustPDO), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
				case 'w': crement(&(podWorker->powerAdjust.ppAdjustPDO), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
				case 's': crement(&(podWorker->powerAdjust.ppAdjustPDO), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
				case 'j': crement(&(podWorker->powerAdjust.pyAdjustPDO), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
				case 'l': crement(&(podWorker->powerAdjust.pyAdjustPDO), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
			
				//power adjustment to happen in motor commander
				case '+':
					crement(&(podWorker->powerAdjust.powerGain), +STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
					printf("powerGain in Poweradjuster: %f\n",podWorker->powerAdjust.powerGain);
					break;
				case '-':
					crement(&(podWorker->powerAdjust.powerGain), -STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
					printf("powerGain in Poweradjuster: %f\n",podWorker->powerAdjust.powerGain);	
					break;
	
				}
			} //endif P-controladjuster	

		//CONTROLLER PDORIENT ADJUSTMODE D-value
		else if ((podWorker->controlMode.controlMode == CMODE_PIDORIENT) && (podWorker->controllerAdjustmode == CADJUSTMODE_D))
			{
			switch (ch)
				{
				//control mode cases
				case 'e': 
					podWorker->controlMode.controlMode =  CMODE_FATAL;					
					podWorker->resetPitchRollReference();						
					break;

				case 'm': podWorker->controllerAdjustmode =  CADJUSTMODE_NULL; printf("Controlleradjust mode turned off!\n"); break;				
				case ',': podWorker->controllerAdjustmode =  CADJUSTMODE_P; printf("Controlleradjust switched to P-values!\n");  break;
				case '/': podWorker->controllerAdjustmode =  CADJUSTMODE_BIAS; printf("Controlleradjust switched to Bias-values!\n");  break;

				//thrust-related controller  adjustments
				case 'i': crement(&(podWorker->powerAdjust.dtAdjustPDO), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
				case 'k': crement(&(podWorker->powerAdjust.dtAdjustPDO), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;						 

				//orient-related controller  adjustments
				case 'a': crement(&(podWorker->powerAdjust.drAdjustPDO), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
				case 'd': crement(&(podWorker->powerAdjust.drAdjustPDO), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
				case 'w': crement(&(podWorker->powerAdjust.dpAdjustPDO), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
				case 's': crement(&(podWorker->powerAdjust.dpAdjustPDO), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
				case 'j': crement(&(podWorker->powerAdjust.dyAdjustPDO), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
				case 'l': crement(&(podWorker->powerAdjust.dyAdjustPDO), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
			
				//power adjustment to happen in motor commander
				case '+':
					crement(&(podWorker->powerAdjust.powerGain), +STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
					printf("powerGain in Poweradjuster: %f\n",podWorker->powerAdjust.powerGain);
					break;
				case '-':
					crement(&(podWorker->powerAdjust.powerGain), -STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
					printf("powerGain in Poweradjuster: %f\n",podWorker->powerAdjust.powerGain);	
					break;
	
				}
			} //endif D-controladjuster	








		//CONTROLLER PDPOSE ADJUSTMODE P-value
		else if ((podWorker->controlMode.controlMode == CMODE_PIDPOSE) && (podWorker->controllerAdjustmode == CADJUSTMODE_P))
			{
			switch (ch)
				{
				//control mode cases
				case 'e': 
					podWorker->controlMode.controlMode =  CMODE_FATAL;					
					podWorker->resetPitchRollReference();						
					break;

				case 'm': podWorker->controllerAdjustmode =  CADJUSTMODE_NULL; printf("Controlleradjust mode turned off!\n"); break;				
				case '.': podWorker->controllerAdjustmode =  CADJUSTMODE_D; printf("Controlleradjust switched to D-values!\n");  break;

				//position-related controller  adjustments
				case 'a': crement(&(podWorker->powerAdjust.pXYAdjustPDPOS), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
				case 'd': crement(&(podWorker->powerAdjust.pXYAdjustPDPOS), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;

				case 'i': crement(&(podWorker->powerAdjust.pZAdjustPDPOS), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
				case 'k': crement(&(podWorker->powerAdjust.pZAdjustPDPOS), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;						 

			
				//power adjustment to happen in motor commander
				case '+':
					crement(&(podWorker->powerAdjust.powerGain), +STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
					printf("powerGain in Poweradjuster: %f\n",podWorker->powerAdjust.powerGain);
					break;
				case '-':
					crement(&(podWorker->powerAdjust.powerGain), -STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
					printf("powerGain in Poweradjuster: %f\n",podWorker->powerAdjust.powerGain);	
					break;
	
				}
			} //endif P-controladjuster	

		//CONTROLLER PDPOSE ADJUSTMODE P-value
		else if ((podWorker->controlMode.controlMode == CMODE_PIDPOSE) && (podWorker->controllerAdjustmode == CADJUSTMODE_D))
			{
			switch (ch)
				{
				//control mode cases
				case 'e': 
					podWorker->controlMode.controlMode =  CMODE_FATAL;					
					podWorker->resetPitchRollReference();						
					break;

				case 'm': podWorker->controllerAdjustmode =  CADJUSTMODE_NULL; printf("Controlleradjust mode turned off!\n"); break;				
				case '.': podWorker->controllerAdjustmode =  CADJUSTMODE_D; printf("Controlleradjust switched to P-values!\n");  break;

				//position-related controller  adjustments
				case 'a': crement(&(podWorker->powerAdjust.dXYAdjustPDPOS), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
				case 'd': crement(&(podWorker->powerAdjust.dXYAdjustPDPOS), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;

				case 'i': crement(&(podWorker->powerAdjust.dZAdjustPDPOS), +STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;
				case 'k': crement(&(podWorker->powerAdjust.dZAdjustPDPOS), -STEPSPDGAINADJUST, MINPDGAINADJUST, MAXPDGAINADJUST); break;						 

			
				//power adjustment to happen in motor commander
				case '+':
					crement(&(podWorker->powerAdjust.powerGain), +STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
					printf("powerGain in Poweradjuster: %f\n",podWorker->powerAdjust.powerGain);
					break;
				case '-':
					crement(&(podWorker->powerAdjust.powerGain), -STEPSPOWERGAIN, MINPOWERGAIN, MAXPOWERGAIN);
					printf("powerGain in Poweradjuster: %f\n",podWorker->powerAdjust.powerGain);	
					break;
	
				}
			} //endif D-controladjuster		

		} //end if keyboard isHit

	  podWorker->controlMode.timestampJetson = GetTimeStamp();
	  podWorker->poseRef.timestampJetson = GetTimeStamp();
	  podWorker->powerAdjust.timestampJetson = GetTimeStamp();								

	  /*---------*/


	  /* Publishing */ 

	  podWorker->lcm.publish ("controlMode", &podWorker->controlMode);
	  podWorker->lcm.publish ("poseRef", &podWorker->poseRef); 	  
	  podWorker->lcm.publish ("powerAdjust", &podWorker->powerAdjust);

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
	  remoteController_t* podWorker = (remoteController_t*) data;	  
	  /*---------*/

	  /*Computation statusPOD*/	  	  
	   if (podWorker->computationInterval > MAXPODDELAY_X*podWorker->callInterval*MS2US)	
		{
		printf("remoteController: delay occured of % " PRId64 "us!\n",podWorker->computationInterval);
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
  remoteController_t podWorker = remoteController_t("remoteController",CALLINTERVAL_REMOTECONTROLLER); 	//provide your PODname here!  
  // 2) Create LCM
  if (!podWorker.lcm.good()) 
    return 1;

  /*---------*/

   // 3) Subscribe this POD to channels
  
  /*  POD-specific init procedures  */
  // Update and publish status of remoteController (this POD is the first to start)
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

  guint timerComputations   = g_timeout_add (podWorker.callInterval, podBase_t::gtimerfuncComputations, (gpointer)&podWorker);  
  guint timerStatusPod      =    g_timeout_add (podWorker.callInterval, podBase_t::gtimerfuncStatusPod,    (gpointer)&podWorker);
 
  g_main_loop_run (mainloop);

  /*---------*/

  return 0;
}







