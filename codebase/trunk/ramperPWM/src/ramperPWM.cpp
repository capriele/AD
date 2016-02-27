
#include "ramperPWM.hpp"

using namespace std;


/*
de/increment and potentially saturatates values
*/
void crement(int16_t* var, int16_t step, double min, double max)
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
    //printf("%d (%f..%f)\n",*var,min,max);
};
int i;

/*
Implementation of loop function for computations in this specific POD
*/

bool ramperPWM_t::doComputations()
{
    ramperPWM_t* podWorker = this;
    std::lock_guard<std::mutex> guard(podMutex);

    /*--------*/




    /* Computations */
    int ch;
    ramperPWM_t::changemode(1);


    if(!ramperPWM_t::kbhit())
    {
        //nothing new on keyboard
    }
    else
    {
        //we got sth on keyboard!
        ch = getchar();
        ramperPWM_t::changemode(0);

        switch(ch)
        {
        case '+':
            if(motorIdxToRamp <= 3)
            {
                crement(&(motorsPwms.pwms[motorIdxToRamp]), +STEPSPWM, MINPWM, MAXPWM + 100);
                printf("current pwm %d at motor %d \n", motorsPwms.pwms[motorIdxToRamp], motorIdxToRamp);
            }
            else if(motorIdxToRamp == 9)
                for(i = 0; i < 4; i++)
                {
                    crement(&(motorsPwms.pwms[i]), +STEPSPWM, MINPWM, MAXPWM + 100);
                    printf("current pwm %d at motor %d \n", motorsPwms.pwms[i], i);
                }
            break;
        case '-':
            if(motorIdxToRamp <= 3)
            {
                crement(&(motorsPwms.pwms[motorIdxToRamp]), -STEPSPWM, MINPWM, MAXPWM + 100);
                printf("current pwm %d at motor %d \n", motorsPwms.pwms[motorIdxToRamp], motorIdxToRamp);
            }
            else if(motorIdxToRamp == 9)
                for(i = 0; i < 4; i++)
                {
                    crement(&(motorsPwms.pwms[i]), -STEPSPWM, MINPWM, MAXPWM + 100);
                    printf("current pwm %d at motor %d \n", motorsPwms.pwms[i], i);
                }
            break;
	//spcific pwm
        case 'u':
            if(motorIdxToRamp <= 3)
            {
                motorsPwms.pwms[motorIdxToRamp] = 1300;
                printf("current pwm %d at motor %d \n", motorsPwms.pwms[motorIdxToRamp], motorIdxToRamp);
            }
            else if(motorIdxToRamp == 9)
                for(i = 0; i < 4; i++)
                {
		motorsPwms.pwms[motorIdxToRamp] = 1300;
                printf("current pwm %d at motor %d \n", motorsPwms.pwms[motorIdxToRamp], motorIdxToRamp);
                }
            break;

        case 'i':
            if(motorIdxToRamp <= 3)
            {
                motorsPwms.pwms[motorIdxToRamp] = 1500;
                printf("current pwm %d at motor %d \n", motorsPwms.pwms[motorIdxToRamp], motorIdxToRamp);
            }
            else if(motorIdxToRamp == 9)
                for(i = 0; i < 4; i++)
                {
		motorsPwms.pwms[motorIdxToRamp] = 1500;
                printf("current pwm %d at motor %d \n", motorsPwms.pwms[motorIdxToRamp], motorIdxToRamp);
                }
            break;

        case 'o':
            if(motorIdxToRamp <= 3)
            {
                motorsPwms.pwms[motorIdxToRamp] = 1700;
                printf("current pwm %d at motor %d \n", motorsPwms.pwms[motorIdxToRamp], motorIdxToRamp);
            }
            else if(motorIdxToRamp == 9)
                for(i = 0; i < 4; i++)
                {
		motorsPwms.pwms[motorIdxToRamp] = 1700;
                printf("current pwm %d at motor %d \n", motorsPwms.pwms[motorIdxToRamp], motorIdxToRamp);
                }
            break;

        case '0':
            motorIdxToRamp = 0;
            printf("selected motorindex: %d\n", motorIdxToRamp);
            break;
        case '1':
            motorIdxToRamp = 1;
            printf("selected motorindex: %d\n", motorIdxToRamp);
            break;
        case '2':
            motorIdxToRamp = 2;
            printf("selected motorindex: %d\n", motorIdxToRamp);
            break;
        case '3':
            motorIdxToRamp = 3;
            printf("selected motorindex: %d\n", motorIdxToRamp);
            break;
        case 'a':
            motorIdxToRamp = 9;
            printf("selected motorindex: all\n");
            break;
        case 'e':
            for(i = 0; i < 4; i++)
            {
                motorsPwms.pwms[i] = MINPWM;
                printf("current pwm %d at motor %d \n", motorsPwms.pwms[i], i);
            }
            printf("emergency issued!\n");
            break;
        case 'c':
            for(i = 0; i < 4; i++)
            {
                motorsPwms.pwms[i] = MAXPWM + 100;
                printf("current pwm %d at motor %d \n", motorsPwms.pwms[i], i);
            }
            printf("all motors to max!\n");
            break;
        } //end switch

    } //end if keyboard isHit

    motorsPwms.timestampJetson = GetTimeStamp();

    /*---------*/


    /* Publishing */

    lcm.publish("motorsPwms", &motorsPwms);

    /*---------*/


    //Print to arduino
    char buf[32];
    //@TODO pay attention to mapping for model motors and real motors order:!!!!
    //model/sim -> flightcontorller: 3->1, 2->2, 4->3, 1->4
    sprintf(buf, "a%4d%4d%4d%4d", motorsPwms.pwms[2], motorsPwms.pwms[1], motorsPwms.pwms[3], motorsPwms.pwms[0]); //a is used as a flag for the arduino!
    //printf("this is to arduino %s\n",buf);
    if(isWriteToArduino)
    {
        writeToArdStatus = write(fd, buf, 18);
    };


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

bool ramperPWM_t::updateStatus()
{
    ramperPWM_t* podWorker = this;
    messageStatus_t messageStatus = checkMessagesUptodate();
    /*---------*/

    /*Computation statusPOD*/
    if(computationInterval > MAXPODDELAY_X * callInterval * MS2US * 1.5)	//@TODO remove hack for 50% more time //@TODO why has this guy delays oftentimes?
    {
        printf("%s: delay in computation, dt=% " PRId64 "us at t=%" PRId64 "!\n", podName.c_str(), computationInterval,GetTimeStamp());
        statusPod.status = POD_FATAL;
    }
    else if((isWriteToArduino) && (writeToArdStatus < 0))
    {
        printf("ramperPWM: error in write to arduino!\n");
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
    ramperPWM_t podWorker = ramperPWM_t("ramperPWM", CALLINTERVAL_MOTORCOMMANDER); 	//provide your PODname here!
    //Unsubscribe from autosubscribed channels
    podWorker.unsubscribe("statusWatchdog");
    podWorker.unsubscribe("statusDrone");

    if((argc == 3) && (strcmp(argv[1], "toArduino") == 0))
    {
        cout << "Writes to Motorarduino!" << endl;
        podWorker.isWriteToArduino = true;
        std::string tmp = argv[2];
        podWorker.usbPortname = "/dev/" + tmp;
        cout << podWorker.usbPortname << endl;
    }

    // 2) Create LCM
    if(!podWorker.lcm.good())
        return 1;

    /*---------*/

    // 3) Subscribe this POD to channels

    /*  POD-specific init procedures  */
    // Update and publish status of ramperPWM (this POD is the first to start)
    podWorker.publishStatus(POD_INITING);


    /* Open the file descriptor in non-blocking mode */
    podWorker.fd = open(podWorker.usbPortname.c_str(), O_RDWR | O_NOCTTY);

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
    std::thread listenerThread(listen, &podWorker);

    // Create mainloop, add loop functions, run

    GMainLoop* mainloop = g_main_loop_new(NULL, FALSE);

    guint timerComputations   = g_timeout_add(podWorker.callInterval, podBase_t::gtimerfuncComputations, (gpointer)&podWorker);
    guint timerStatusPod      =    g_timeout_add(podWorker.callInterval, podBase_t::gtimerfuncStatusPod, (gpointer)&podWorker);

    g_main_loop_run(mainloop);

    /*---------*/

    return 0;
}







