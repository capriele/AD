
#include "ramperPWM.hpp"

using namespace std;


/*
de/increment and potentially saturatates values
*/
void crement(int16_t* var, int16_t step, double min, double max)
{
	*var += step;
	if (*var > max) {*var = max;};
	if (*var < min) {*var = min;};
	//printf("%d (%f..%f)\n",*var,min,max);
};


/*
Implementation of loop function for computations in this specific POD
*/

gboolean podBase_t::gtimerfuncComputations (gpointer data) {

	  /* General Infrastructure (maintain this structure!) */
	  ramperPWM_t* podWorker = (ramperPWM_t*) data;	
  	  std::lock_guard<std::mutex> guard(podMutex);	  

	  /*--------*/
	  

  

  	  /* Computations */
	 int ch;
  	 ramperPWM_t::changemode(1);

		
	 if ( !ramperPWM_t::kbhit() )
	   	{
	   	//nothing new on keyboard
	   	}
	else
		{
		//we got sth on keyboard!
		ch = getchar();
		ramperPWM_t::changemode(0);
		
		switch (ch)
			{
			case '+':
				crement(&(podWorker->motorsPwms.pwms[podWorker->motorIdxToRamp]), +STEPSPWM, MINPWM, MAXPWM);
				printf("current pwm %d at motor %d \n",podWorker->motorsPwms.pwms[podWorker->motorIdxToRamp],podWorker->motorIdxToRamp);
				break;
			case '-':
				crement(&(podWorker->motorsPwms.pwms[podWorker->motorIdxToRamp]), -STEPSPWM, MINPWM, MAXPWM);
				printf("current pwm %d at motor %d \n",podWorker->motorsPwms.pwms[podWorker->motorIdxToRamp],podWorker->motorIdxToRamp);
				break;
			case '0':
				podWorker->motorIdxToRamp = 0;
				printf("selected motorindex: %d\n",podWorker->motorIdxToRamp);
				break;
			case '1':
				podWorker->motorIdxToRamp = 1;
				printf("selected motorindex: %d\n",podWorker->motorIdxToRamp);
				break;			
			case '2':
				podWorker->motorIdxToRamp = 2;
				printf("selected motorindex: %d\n",podWorker->motorIdxToRamp);
				break;			
			case '3':
				podWorker->motorIdxToRamp = 3;
				printf("selected motorindex: %d\n",podWorker->motorIdxToRamp);
				break;
			} //end switch	

		} //end if keyboard isHit

	  podWorker->motorsPwms.timestampJetson = GetTimeStamp();  							

	  /*---------*/


	  /* Publishing */ 

	  podWorker->lcm.publish ("motorsPwms", &podWorker->motorsPwms);

	  /*---------*/


	//Print to arduino
	char buf[32];
	//@TODO pay attention to mapping for model motors and real motors order:!!!!
	//model/sim -> flightcontorller: 3->1, 2->2, 4->3, 1->4
	sprintf(buf,"a%4d%4d%4d%4d",podWorker->motorsPwms.pwms[2],podWorker->motorsPwms.pwms[1],podWorker->motorsPwms.pwms[3],podWorker->motorsPwms.pwms[0]); //a is used as a flag for the arduino!
	//printf("this is to arduino %s\n",buf);
	if (podWorker->isWriteToArduino) 
		{
		podWorker->writeToArdStatus = write(podWorker->fd, buf, 18);
		};
        

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
	  ramperPWM_t* podWorker = (ramperPWM_t*) data;	  
	  /*---------*/

	  /*Computation statusPOD*/	  	  
	   if (podWorker->computationInterval > MAXPODDELAY_X*podWorker->callInterval*MS2US*1.5)	//@TODO remove hack for 50% more time
		{
		printf("ramperPWM: delay occured of % " PRId64 "us!\n",podWorker->computationInterval); //@TODO why has this guy delays oftentimes?
		podWorker->statusPod.status = POD_FATAL;
		}
	  else if ((!podWorker->checkMessagesUptodate()) )
		{
		podWorker->statusPod.status = POD_CRITICAL;
		}
	  else if ((podWorker->isWriteToArduino) && (podWorker->writeToArdStatus<0))  
		{
		printf("ramperPWM: error in write to arduino!\n"); //@TODO add errorhandling with return status of write (m)
		podWorker->statusPod.status = POD_OK;
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
  ramperPWM_t podWorker = ramperPWM_t("ramperPWM",CALLINTERVAL_MOTORCOMMANDER); 	//provide your PODname here!  
  //Unsubscribe from autosubscribed channels
  podWorker.unsubscribe("statusWatchdog");
  podWorker.unsubscribe("statusDrone");

  if ((argc>=2) && (strcmp(argv[1], "toArduino") == 0))	
	{
	podWorker.isWriteToArduino = true;
	}

  // 2) Create LCM
  if (!podWorker.lcm.good()) 
    return 1;

  /*---------*/

   // 3) Subscribe this POD to channels
  
  /*  POD-specific init procedures  */
  // Update and publish status of ramperPWM (this POD is the first to start)
  podWorker.publishStatus(POD_INITING);	


 /* Open the file descriptor in non-blocking mode */
  podWorker.fd = open(podWorker.portname, O_RDWR | O_NOCTTY);

  /* Set up the control structure */
  struct termios toptions;
	 
 /* Get currently set options for the tty */
 tcgetattr(podWorker.fd, &toptions);
 
/* Set custom options */
 
/* 115200 baud */
 cfsetispeed(&toptions, B115200);
 cfsetospeed(&toptions, B115200);
 /* 8 bits, no parity, no stop bits */
 toptions.c_cflag &= ~PARENB;
 toptions.c_cflag &= ~CSTOPB;
 toptions.c_cflag &= ~CSIZE;
 toptions.c_cflag |= CS8;
 /* no hardware flow control */
 toptions.c_cflag &= ~CRTSCTS;
 /* enable receiver, ignore status lines */
 toptions.c_cflag |= CREAD | CLOCAL;
 /* disable input/output flow control, disable restart chars */
 toptions.c_iflag &= ~(IXON | IXOFF | IXANY);
 /* disable canonical input, disable echo,
 disable visually erase chars,
 disable terminal-generated signals */
 toptions.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);
 /* disable output processing */
 toptions.c_oflag &= ~OPOST;
 
/* wait for 24 characters to come in before read returns */
 toptions.c_cc[VMIN] = 12;
 /* no minimum time to wait before read returns */
 toptions.c_cc[VTIME] = 0;
 
/* commit the options */
 tcsetattr(podWorker.fd, TCSANOW, &toptions);
 
/* Wait for the Arduino to reset */
 sleep(1);
 /* Flush anything already in the serial buffer */
 tcflush(podWorker.fd, TCIFLUSH);
 /* read up to 128 bytes from the fd */
//  int n = read(fd, buf, 128);

	

  /*---------*/


  /*  General Infrastructure: loops (maintain this structure!)  */

  printf("Init DONE...\n");
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







