
#include "motorCommander.hpp"

using namespace std;

struct timeval tv;

int16_t omegaToPwm(double omega)
{
    //int pwm = (int)(AOMEGA2TOPWM*pow(omega,2)+COMEGA2TOPWM);
    int pwm = static_cast<int>(AW2PWM * pow(omega - CW2PWM, 2) + BW2PWM);

    //PWM saturation
    if(pwm > MAXPWM)
        pwm = MAXPWM;
    if(pwm < ZERORPMPWM)
    {
        pwm = ZERORPMPWM;
        printf("pwm %d < %d", pwm, ZERORPMPWM);
    }

    return static_cast<int16_t>(pwm);
};


/*
Implementation of loop function for computations in this specific POD
*/

bool motorCommander_t::doComputations()
{
    motorCommander_t* podWorker = this;
    std::lock_guard<std::mutex> guard(podMutex);

    /*--------*/

    /* Computations */
    double powerGain = powerAdjust.powerGain;

    //Delay check
    //printf("%"PRId64"\n",enterComputationloop-motorsWsRefPDOrient.timestampJetson);

    //point motorsWsRefControlerToUse to values from contoller that control mode chooses
    switch(controlMode.controlMode)
    {
    case CMODE_PIDORIENT:
        motorsWsRefControlerToUse = &(motorsWsRefPDOrient);
        break;
    case CMODE_PIDPOSE:
        //@TODO add PID pose controller
        motorsWsRefControlerToUse = &(motorsWsRefPDPose);
        break;
    case CMODE_SOCORIENT:
        //@TODO add SOCOrient controller
        printf("no SOCOrient controller available, uses PD-orient!\n");
        motorsWsRefControlerToUse = &(motorsWsRefPDOrient);
        break;
    case CMODE_SOCPOSE:
        //@TODO add SOCPose controller
        printf("no SOCPose controller available, uses PD-orient!\n");
        motorsWsRefControlerToUse = &(motorsWsRefPDOrient);
        break;
    }
    switch(statusDrone.status)
    {
    case DRONE_WAITPODS: //outputs PWM 0
        motorsPwms.pwms[0] = static_cast<int16_t>(ZERORPMPWM);
        motorsPwms.pwms[1] = static_cast<int16_t>(ZERORPMPWM);
        motorsPwms.pwms[2] = static_cast<int16_t>(ZERORPMPWM);
        motorsPwms.pwms[3] = static_cast<int16_t>(ZERORPMPWM);
        break;

    case DRONE_IDLE:   //outputs PWM idle spin
        motorsPwms.pwms[0] = static_cast<int16_t>(IDLERPMPWM);
        motorsPwms.pwms[1] = static_cast<int16_t>(IDLERPMPWM);
        motorsPwms.pwms[2] = static_cast<int16_t>(IDLERPMPWM);
        motorsPwms.pwms[3] = static_cast<int16_t>(IDLERPMPWM);
        break;

    case DRONE_TAKEOFF: //outputs PWM suggested by PD-pose controller
        motorsPwms.pwms[0] = omegaToPwm(powerGain / 100 * motorsWsRefPDOrient.wsRef[0]);
        motorsPwms.pwms[1] = omegaToPwm(powerGain / 100 * motorsWsRefPDOrient.wsRef[1]);
        motorsPwms.pwms[2] = omegaToPwm(powerGain / 100 * motorsWsRefPDOrient.wsRef[2]);
        motorsPwms.pwms[3] = omegaToPwm(powerGain / 100 * motorsWsRefPDOrient.wsRef[3]);
        break;

    case DRONE_FLY:    //outputs PWM suggested by controller selecetd by controlMode
        motorsPwms.pwms[0] = omegaToPwm(powerGain / 100 * motorsWsRefControlerToUse->wsRef[0]);
        motorsPwms.pwms[1] = omegaToPwm(powerGain / 100 * motorsWsRefControlerToUse->wsRef[1]);
        motorsPwms.pwms[2] = omegaToPwm(powerGain / 100 * motorsWsRefControlerToUse->wsRef[2]);
        motorsPwms.pwms[3] = omegaToPwm(powerGain / 100 * motorsWsRefControlerToUse->wsRef[3]);
        break;

    case DRONE_CRITICAL://outputs PWM suggested by PD-pose controller //@TODO change this to PD-pose once available
        motorsPwms.pwms[0] = omegaToPwm(powerGain / 100 * motorsWsRefPDOrient.wsRef[0]);
        motorsPwms.pwms[1] = omegaToPwm(powerGain / 100 * motorsWsRefPDOrient.wsRef[1]);
        motorsPwms.pwms[2] = omegaToPwm(powerGain / 100 * motorsWsRefPDOrient.wsRef[2]);
        motorsPwms.pwms[3] = omegaToPwm(powerGain / 100 * motorsWsRefPDOrient.wsRef[3]);
        break;

    case DRONE_FATAL:
        motorsPwms.pwms[0] = static_cast<int16_t>(ZERORPMPWM);
        motorsPwms.pwms[1] = static_cast<int16_t>(ZERORPMPWM);
        motorsPwms.pwms[2] = static_cast<int16_t>(ZERORPMPWM);
        motorsPwms.pwms[3] = static_cast<int16_t>(ZERORPMPWM);
        break;

    default:
        printf("warning: no update in pwms sent, no critical statusPOD issued!\n");

    }

    //Shortcut emergecency without going throught watchdog
    if((statusPod.status < POD_INITING) && (statusDrone.status > DRONE_WAITPODS))
    {
        printf("motorCommanderStatus bad (emergency status this Pod -> motor stop) !\n"); //watchdog would also do, since this pod status is not OK anymore, but might take longer than shutting motors in here!
        motorsPwms.pwms[0] = static_cast<int16_t>(ZERORPMPWM);
        motorsPwms.pwms[1] = static_cast<int16_t>(ZERORPMPWM);
        motorsPwms.pwms[2] = static_cast<int16_t>(ZERORPMPWM);
        motorsPwms.pwms[3] = static_cast<int16_t>(ZERORPMPWM);
    }

    //add timestamp
    motorsPwms.timestampJetson = GetTimeStamp();

    /*---------*/


    /* Publishing compuation result */

    // publish
    lcm.publish("motorsPwms", &motorsPwms);

    //printf("motors: %d %d %d %d\n",motorsPwms.pwms[0],motorsPwms.pwms[1],motorsPwms.pwms[2],motorsPwms.pwms[3]);

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

bool motorCommander_t::updateStatus()
{
    motorCommander_t* podWorker = this;
    messageStatus_t messageStatus = checkMessagesUptodate();
    std::lock_guard<std::mutex> guard(podMutex);

    /*---------*/

    /*Computation statusPOD*/

    if(computationInterval > MAXPODDELAY_X * callInterval * MS2US * 1.5)	//@TODO remove hack for 50% more time //@TODO why has this POD delays oftentimes?
    {
        printf("%s: delay in computation, dt=% " PRId64 "us at t=%" PRId64 "!\n", podName.c_str(), computationInterval,GetTimeStamp());
        statusPod.status = POD_FATAL;
    }
    else if((isWriteToArduino) && (writeToArdStatus < 0))
    {
        printf("motorCommander: error in write to arduino!\n");
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
    motorCommander_t podWorker = motorCommander_t("motorCommander", CALLINTERVAL_MOTORCOMMANDER); 	//provide your PODname here!

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

    // 3) Subscribe this POD to channels
    podWorker.subscribe("powerAdjust", CALLINTERVAL_REMOTECONTROLLER*AGEMSG_X_TUNNEL,	 &(podWorker.powerAdjust), 	&podBase_t::handleMessage<agile::powerAdjust_t>);
    podWorker.subscribe("motorsWsRefPDOrient", CALLINTERVAL_CONTROLLERPDORIENT, 	 &(podWorker.motorsWsRefPDOrient), 	&podBase_t::handleMessage<agile::motorsWsRef_t>);
    podWorker.subscribe("motorsWsRefPDPose", CALLINTERVAL_CONTROLLERPDPOSE, 	 &(podWorker.motorsWsRefPDPose), 	&podBase_t::handleMessage<agile::motorsWsRef_t>);
    //podWorker.subscribe("statusDrone", CALLINTERVAL_WATCHDOG, &(podWorker.statusDrone), &podBase_t::handleMessage<agile::statusDrone_t>); //is autosubscribed via base class constructor!
    podWorker.subscribe("controlMode", CALLINTERVAL_REMOTECONTROLLER*AGEMSG_X_TUNNEL, &(podWorker.controlMode), &podBase_t::handleMessage<agile::controlMode_t>);
    //@TODO subscribe to more controllers

    /*---------*/




    /*  POD-specific init procedures  */
    // Update and publish status of motorCommander (this POD is the first to start)
    podWorker.publishStatus(POD_INITING);

     if (podWorker.isWriteToArduino)
    {
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
    }


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







