
#include "imuAcquisition.hpp"

using namespace std;


int serialport_read_until(int fd, char * buf)
{
    char until = '\n';
    char b[1];
    int i = 0;
    do
    {
        int n = read(fd, b, 1);  // read a char at a time
        if(n == -1) return -1;   // couldn't read
        if(n == 0)
        {
            usleep(100);   // wait 100 usec try again
            continue;
        }
        buf[i] = b[0]; i++;
    }
    while(b[0] != until);

    buf[i] = 0;  // null terminate the string
    return 0;
}


vector<int> char2vector(char *buf)
{
    vector<int> v(10);
    v[0] = 0;
    v[1] = 0;
    v[2] = 0;
    v[3] = 0;
    v[4] = 0;
    v[5] = 0;
    v[6] = 0;
    v[7] = 0;
    v[8] = 0;
    v[9] = 0;


    bool is_negative = false;
    int i = 0;

    char *extract;
    char *ptrend;

    //printf(">>%s<<\n",buf);


    /*for (int k=0; k<10; k++)
    {

       while(*(buf+i) != ' ' )
       {
    	if(*(buf+i) == '-')
    		is_negative = true;

    	else
    		v[k] = 10*v[k]+ *(buf+i) - '0';
    	i++;
       }
       i++;
       if(is_negative)
       {
    	v[k]=-1*v[k];
    	is_negative = false;
       }

    }*/

    while((buf != NULL) && i < 10)
    {
        //printf("extracvt form buf, i:%d\n",i);
        if(i == 0)
        {
            v[i] = strtol(buf, &ptrend, 10);
        }
        else
        {
            v[i] = strtol(ptrend, &ptrend, 10);
        };/*
	   printf("extracted\n");
	   v[i] = strtol(extract,&ptrend,10);*/
        //printf("atoit\n");
        i++;

    }
    return v;
}

/*
Implementation of loop function for computations in this specific POD
*/



gboolean podBase_t::gtimerfuncComputations(gpointer data)
{

    /* General Infrastructure (maintain this structure!) */
    imuAcquisition_t* podWorker = reinterpret_cast<imuAcquisition_t*>(data);
    std::lock_guard<std::mutex> guard(podMutex);

    /*--------*/


    /* imuAcquisition */

    agile::imuRaw_t imuRaw;
//	  imuRaw = podWorker->imuRaw;

    int readFromArdStatus = serialport_read_until(podWorker->fd, podWorker->buf);

    if((readFromArdStatus != 0) || (podWorker->buf[0] == 10))

        //@TODO without this, "publish imuRaw" is always half the imuAcuqition-callrate.
        //I.e., every other arduino reading is missed
        readFromArdStatus = serialport_read_until(podWorker->fd, podWorker->buf);

    double k = 1000000.0;

    vector<int> vec(10);

    if((readFromArdStatus == 0) && (podWorker->buf != NULL) && (podWorker->buf[0] != 10))	//check if buf has imu-data
    {
        vec = char2vector(podWorker->buf);
        if((vec[9] == 0 && vec[8] == 0 && vec[7] == 0) || (vec[6] == 0 && vec[5] == 0 && vec[4] == 0) || (vec[3] == 0 && vec[2] == 0 && vec[1] == 0))
            readFromArdStatus = serialport_read_until(podWorker->fd, podWorker->buf);

    }

    if(podWorker->statusCalib == -2)
        printf("Waiting for data to calibrate...\n");

    if((readFromArdStatus == 0) && (podWorker->buf != NULL) && (podWorker->buf[0] != 10))	//check if buf has imu-data
    {
        vec = char2vector(podWorker->buf); //@TODO warning, using th strtok routine empties the buffer!

        imuRaw.timestampArduino = vec[0]; // microseconds
        imuRaw.gyro[0]  = vec[1] / k;
        imuRaw.gyro[1]  = vec[2] / k;
        imuRaw.gyro[2]  = vec[3] / k;
        imuRaw.accel[0] = vec[4] / k;
        imuRaw.accel[1] = vec[5] / k;
        imuRaw.accel[2] = vec[6] / k;
        if(vec[7] != 0)
        {
            imuRaw.magn[0]  = vec[7] / k;
            imuRaw.magn[1]  = vec[8] / k;
            imuRaw.magn[2]  = vec[9] / k;
        }
        imuRaw.baro = 0;          //not using barometer with the current imu because it drops imu rate to 24Hz

        if(podWorker->statusCalib == -2)
        {
            printf("Starting calibration...\n");
            podWorker->starttimeCalib = GetTimeStamp();
            podWorker->statusCalib = -1;

            podWorker->imuCalib.gyro[0]  = imuRaw.gyro[0];
            podWorker->imuCalib.gyro[1]  = imuRaw.gyro[1];
            podWorker->imuCalib.gyro[2]  = imuRaw.gyro[2];
            podWorker->imuCalib.accel[0] = imuRaw.accel[0];
            podWorker->imuCalib.accel[1] = imuRaw.accel[1];
            podWorker->imuCalib.accel[2] = imuRaw.accel[2];
            podWorker->imuCalib.magn[0]  = imuRaw.magn[0];
            podWorker->imuCalib.magn[1]  = imuRaw.magn[1];
            podWorker->imuCalib.magn[2]  = imuRaw.magn[2];
            podWorker->imuCalib.baro = imuRaw.baro;

            podWorker->stateVariances.imuVarianceGyro[0] = 0.0;
            podWorker->stateVariances.imuVarianceGyro[1] = 0.0;
            podWorker->stateVariances.imuVarianceGyro[2] = 0.0;

            podWorker->delta[0] = imuRaw.gyro[0];
            podWorker->delta[1] = imuRaw.gyro[1];
            podWorker->delta[2] = imuRaw.gyro[2];
            podWorker->mean[0] = imuRaw.gyro[0];
            podWorker->mean[1] = imuRaw.gyro[1];
            podWorker->mean[2] = imuRaw.gyro[2];

            podWorker->nMeasurements = 1;

        }
        else if((podWorker->statusCalib == -1) && ((GetTimeStamp() - podWorker->starttimeCalib) < CALIBINTERVAL_IMU * MS2US))
        {
            int n = podWorker->nMeasurements;

            podWorker->imuCalib.gyro[0]  = podWorker->imuCalib.gyro[0] * n / (n + 1) + imuRaw.gyro[0] / (n + 1);
            podWorker->imuCalib.gyro[1]  = podWorker->imuCalib.gyro[1] * n / (n + 1) + imuRaw.gyro[1] / (n + 1);
            podWorker->imuCalib.gyro[2]  = podWorker->imuCalib.gyro[2] * n / (n + 1) + imuRaw.gyro[2] / (n + 1);
            podWorker->imuCalib.accel[0] = podWorker->imuCalib.accel[0] * n / (n + 1) + imuRaw.accel[0] / (n + 1);
            podWorker->imuCalib.accel[1] = podWorker->imuCalib.accel[1] * n / (n + 1) + imuRaw.accel[1] / (n + 1);
            podWorker->imuCalib.accel[2] = podWorker->imuCalib.accel[2] * n / (n + 1) + imuRaw.accel[2] / (n + 1);
            podWorker->imuCalib.magn[0]  = podWorker->imuCalib.magn[0] * n / (n + 1) + imuRaw.magn[0] / (n + 1);
            podWorker->imuCalib.magn[1]  = podWorker->imuCalib.magn[1] * n / (n + 1) + imuRaw.magn[1] / (n + 1);
            podWorker->imuCalib.magn[2]  = podWorker->imuCalib.magn[2] * n / (n + 1) + imuRaw.magn[2] / (n + 1);
            podWorker->imuCalib.baro = podWorker->imuCalib.baro * n / (n + 1) + imuRaw.baro / (n + 1);

            podWorker->delta[0] = imuRaw.gyro[0] - podWorker->mean[0];
            podWorker->delta[1] = imuRaw.gyro[1] - podWorker->mean[1];
            podWorker->delta[2] = imuRaw.gyro[2] - podWorker->mean[2];
            podWorker->mean[0] = podWorker->mean[0] + podWorker->delta[0] / n; //@TODO why not n+1? or above n instead of n+1? link/reference would be awesome. check where to put n=n+1
            podWorker->mean[1] = podWorker->mean[1] + podWorker->delta[1] / n;
            podWorker->mean[2] = podWorker->mean[2] + podWorker->delta[2] / n;

            podWorker->stateVariances.imuVarianceGyro[0] = podWorker->stateVariances.imuVarianceGyro[0] + podWorker->delta[0] * (imuRaw.gyro[0] - podWorker->mean[0]);
            podWorker->stateVariances.imuVarianceGyro[1] = podWorker->stateVariances.imuVarianceGyro[1] + podWorker->delta[1] * (imuRaw.gyro[1] - podWorker->mean[1]);
            podWorker->stateVariances.imuVarianceGyro[2] = podWorker->stateVariances.imuVarianceGyro[2] + podWorker->delta[2] * (imuRaw.gyro[2] - podWorker->mean[2]);




            podWorker->nMeasurements += 1;
        }
        else if(podWorker->statusCalib == -1)
        {
            printf("Calibration DONE...\n >> %f %f %f %f %f %f %f %f %f %f <<\n",
                   podWorker->imuCalib.gyro[0], podWorker->imuCalib.gyro[1],
                   podWorker->imuCalib.gyro[2], podWorker->imuCalib.accel[0],
                   podWorker->imuCalib.accel[1], podWorker->imuCalib.accel[2],
                   podWorker->imuCalib.magn[0], podWorker->imuCalib.magn[1],
                   podWorker->imuCalib.magn[2], podWorker->imuCalib.baro);

            //finishing up calib
            podWorker->stateVariances.imuBiasGyro[0] = podWorker->imuCalib.gyro[0];
            podWorker->stateVariances.imuBiasGyro[1] = podWorker->imuCalib.gyro[1];
            podWorker->stateVariances.imuBiasGyro[2] = podWorker->imuCalib.gyro[2];

            //@TODO ok to also store this here?
            podWorker->stateVariances.imuBiasAccel[0] = podWorker->imuCalib.accel[0];
            podWorker->stateVariances.imuBiasAccel[1] = podWorker->imuCalib.accel[1];
            podWorker->stateVariances.imuBiasAccel[2] = podWorker->imuCalib.accel[2] - GRAVITY; //@TODO warning: this assumes planar calibration!


            podWorker->stateVariances.imuVarianceGyro[0] = podWorker->stateVariances.imuVarianceGyro[0] / (podWorker->nMeasurements - 2);
            podWorker->stateVariances.imuVarianceGyro[1] = podWorker->stateVariances.imuVarianceGyro[1] / (podWorker->nMeasurements - 2);
            podWorker->stateVariances.imuVarianceGyro[2] = podWorker->stateVariances.imuVarianceGyro[2] / (podWorker->nMeasurements - 2);

            /*//stateEstimatorOrientV1 uses calib reslts as 0-reference vector
            podWorker->features.featureDirectionVersor[0][0] = podWorker->imuCalib.accel[0];
            podWorker->features.featureDirectionVersor[0][1] = podWorker->imuCalib.accel[1];
            podWorker->features.featureDirectionVersor[0][2] = podWorker->imuCalib.accel[2];

            podWorker->features.featureDirectionVersor[1][0] = podWorker->imuCalib.magn[0];
            podWorker->features.featureDirectionVersor[1][1] = podWorker->imuCalib.magn[1];
            podWorker->features.featureDirectionVersor[1][2] = podWorker->imuCalib.magn[2];
            */

            //ATTENTION: we publish the calibration data over estimation channels! -> estimator waits for this first message on this channel!
            //podWorker->lcm.publish ("features", &podWorker->features);
            //podWorker->lcm.publish ("stateVariancesOrientV1", &podWorker->stateVariances);
            podWorker->lcm.publish("stateVariancesOrientCF", &podWorker->stateVariances);

            podWorker->statusCalib = 0;

        }
        else if(podWorker->statusCalib == 0)
        {
            /*
            //This publishes accelerometer and magnetometervectors as featureDirectionVersors; later this job will be done by vectors pointing to the visual markers!
            podWorker->features.featureDirectionVersor[0][0] = imuRaw.accel[0];
            podWorker->features.featureDirectionVersor[0][1] = imuRaw.accel[1];
            podWorker->features.featureDirectionVersor[0][2] = imuRaw.accel[2];

            podWorker->features.featureDirectionVersor[1][0] = imuRaw.magn[0];
            podWorker->features.featureDirectionVersor[1][1] = imuRaw.magn[1];
            podWorker->features.featureDirectionVersor[1][2] = imuRaw.magn[2];
            podWorker->lcm.publish ("features", &podWorker->features);
            */
        };

        /* Publishing computation result */
        imuRaw.timestampJetson = GetTimeStamp();

        //	if( vec[0]!=0 && vec[1]!=0 && vec[2]!=0 && vec[3]!=0 && vec[4]!=0 && vec[5]!=0 && vec[6]!=0 && vec[7]!=0 && vec[8]!=0 && vec[9]!=0) //@TODO should we add this?
        {
            podWorker->lcm.publish("imuRaw", &imuRaw);
        }
        /*---------*/
    }
    else
    {
        //nothing read;
    }


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
    imuAcquisition_t* podWorker = reinterpret_cast<imuAcquisition_t*>(data);
    std::lock_guard<std::mutex> guard(podMutex);
    /*---------*/

    /*Computation statusPOD*/

    if((podWorker->computationInterval > MAXPODDELAY_X * podWorker->callInterval * MS2US) && (podWorker->statusCalib == 0))
    {
        printf("imuAcquisition: delay occured of % " PRId64 "us!\n", podWorker->computationInterval);
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
    ;

    // 1) Create the app
    imuAcquisition_t podWorker = imuAcquisition_t("imuAcquisition", CALLINTERVAL_IMUACQUISITION); 	//provide your PODname here!

    if(argc < 2)
    {
        printf("Please provide USB port of IMU-reading arduino (e.g. ttyUSB0)!\n");
        return EXIT_FAILURE;
    };
    std::string tmp = argv[1];
    podWorker.usbPortname = "/dev/" + tmp;
    cout << podWorker.usbPortname << endl;

    // 2) Create LCM
    if(!podWorker.lcm.good())
        return 1;


    /*  POD-specific init procedures  */
    // Update and publish status of imuAcquisition (this POD is the first to start)
    printf("Initializing POD...\n");
    podWorker.publishStatus(POD_INITING);

//cout << "DEBUG (1): imuAcquisition.fd " << imuAcquisition.fd << endl;


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




    // Create mainloop





    printf("Initializing POD... DONE\n");
    /*---------*/

    /*  General Infrastructure: loops (maintain this structure!)  */

    //  POD-wait for watchdogPOD to be up and running (POD_OK)  before continue

    printf("Waiting for watchdog to be POD_INITING...\n");
    while(podWorker.statusWatchdog.status != POD_INITING)
    {
        podWorker.lcm.handle();
    }
    printf("Watchdog POD_INITING! Continuing...\n");
    podWorker.initComputationInterval();

    //Create message listener thead	- watchdog is subscribed!
    std::thread listenerThread(listen, &podWorker);


    //Create loop
    GMainLoop* mainloop = g_main_loop_new(NULL, FALSE);

    guint timerComputations = g_timeout_add(podWorker.callInterval, podBase_t::gtimerfuncComputations, (gpointer)&podWorker);
    guint timerStatusPod    = g_timeout_add(podWorker.callInterval, podBase_t::gtimerfuncStatusPod, (gpointer)&podWorker);
    g_main_loop_run(mainloop);


    /*---------*/

    return 0;
}







