
#include "imuAcquisition.hpp"
#include <sys/ioctl.h>


using namespace std;

int setNonblocking(int fd)
{
    int flags;

    /* If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
    /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    /* Otherwise, use the old way of doing it */
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif
} 

int serialport_read_until(int fd, char * buf)
{	
long now0 = GetTimeStamp();
long now1,now2,now3,now_do,now_aw,delta;

    int timeouts_max = 1;
    int bufferlen_max = 70;
    int timeouts = timeouts_max;

    char until = '\n';
    char b[1];
    int i = 0;int count=0;
    int n=0;


    setNonblocking(fd); //TODO does not seem to work

    do
    {
	now1 = GetTimeStamp();
	
	n = read(fd, b, 1);  // read a char at a time
		 now2 = GetTimeStamp();
		//delta = now2-now1; if (delta>1000) cout<<"loop_1 "<<delta<<endl;

        if(n == -1) return -1;   // couldn't read
	

        if((n == 0)&&(timeouts>0)) //is this aligned with the socket settings?
        {
            usleep(10);   // wait 1 usec try again
	    timeouts--;
            continue;
        }
        buf[i] = b[0]; i++;
		now3 = GetTimeStamp();	
		delta = now3-now2; if (delta>1000) cout<<" loop_2 "<<delta<<endl;
	
    }
    while((b[0] != until)&&(timeouts>0)&&(i<bufferlen_max));

	now_aw = GetTimeStamp();

    if (i>5)
	{
    	buf[i] = 0;  // null terminate the string
	
	n = read(fd, b, 1);  // read lf? without this, second reading occurs in computations()
	}

	now_do = GetTimeStamp();

	long delta0 = now_aw-now0;
	long delta1 = now_do-now_aw;
	//cout<<"delta spru: "<<delta0<<" "<<delta1<<" "<<i<<endl;

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



bool imuAcquisition_t::doComputations()
{
    imuAcquisition_t* podWorker = this;
    messageStatus_t messageStatus = checkMessagesUptodate();
    std::lock_guard<std::mutex> guard(podMutex);

    /*--------*/


    /* imuAcquisition */

    agile::imuRaw_t imuRaw;

    long tsnow1 = GetTimeStamp();

    //"reset"
    buf[0] = 10;
    //read
    int readFromArdStatus = serialport_read_until(fd, buf);
    long tsnow2 = GetTimeStamp();

    if((readFromArdStatus != 0) || (buf[0] == 10))
	{
        //@TODO without this, "publish imuRaw" is always half the imuAcuqition-callrate.
        //I.e., every other arduino reading is missed
        readFromArdStatus = serialport_read_until(fd, buf);
	//cout<<"second reading of serial occured!"<<endl;
	}
   else
	{//cout<<"no second reading"<<endl;
	};

    long tsnow3 = GetTimeStamp();
    //printf("reading took %d %d \n",tsnow2-tsnow1,tsnow3-tsnow2);
    imuRaw.timestampJetson = GetTimeStamp();
    double k = 1000000.0;

    vector<int> vec(10);

    if((readFromArdStatus == 0) && (buf != NULL) && (buf[0] != 10))	//check if buf has imu-data
    {
      /*  vec = char2vector(buf);
        //if((vec[9] == 0 && vec[8] == 0 && vec[7] == 0) || (vec[6] == 0 && vec[5] == 0 && vec[4] == 0) || (vec[3] == 0 && vec[2] == 0 && vec[1] == 0)) //TODO add magnetometer reading?
	if((vec[6] == 0 && vec[5] == 0 && vec[4] == 0) || (vec[3] == 0 && vec[2] == 0 && vec[1] == 0)) //TODO add magnetometer reading, was removed in arduino to reduce data
	    {
	    printf("third reading of serial occured! %d %d %d %d %d %d\n ",vec[1],vec[2],vec[3],vec[4],vec[5],vec[6]);
            readFromArdStatus = serialport_read_until(fd, buf);
	    };*/
		
    }

    //set stage of calibration procedure to wait for data
    if(statusCalib == -2)
        printf("Waiting for data to calibrate...\n");



    //process data if existent
    if((readFromArdStatus == 0) && (buf != NULL) && (buf[0] != 10))	//check if buf has imu-data
    {	

	//read data
        vec = char2vector(buf); //@TODO warning, using th strtok routine empties the buffer!

        imuRaw.timestampArduino = vec[0]; // microseconds
	
        imuRaw.gyro[0]  = vec[1] / k;
        imuRaw.gyro[1]  = vec[2] / k;
        imuRaw.gyro[2]  = vec[3] / k;
        imuRaw.accel[0] = vec[4] / k*100; //@TODO arduino code truncates long-int by 100 to reduce bytes
        imuRaw.accel[1] = vec[5] / k*100; //@TODO arduino code truncates long-int by 100 to reduce bytes
        imuRaw.accel[2] = vec[6] / k*100; //@TODO arduino code truncates long-int by 100 to reduce bytes
        if(vec[7] != 0)
        {
            imuRaw.magn[0]  = vec[7] / k;
            imuRaw.magn[1]  = vec[8] / k;
            imuRaw.magn[2]  = vec[9] / k;
        }
        imuRaw.baro = 0;          //not using barometer with the current imu because it drops imu rate to 24Hz



        //finalize "calibration procedure" in case no calibration requested
        if (noCalib)
	{
	    if (statusCalib<0 )
            {
            stateVariances.imuBiasGyro[0] = defaultBias[0];
            stateVariances.imuBiasGyro[1] = defaultBias[1];
            stateVariances.imuBiasGyro[2] = defaultBias[2];

            stateVariances.imuBiasAccel[0] = defaultBias[3];
            stateVariances.imuBiasAccel[1] = defaultBias[4];
            stateVariances.imuBiasAccel[2] = defaultBias[5] - GRAVITY; //@TODO warning: this assumes planar calibration!

            stateVariances.imuVarianceGyro[0] = defaultBias[6];
            stateVariances.imuVarianceGyro[1] = defaultBias[7];
            stateVariances.imuVarianceGyro[2] = defaultBias[8];

            stateVariances.imuVarianceAccel[0] = defaultBias[9];
            stateVariances.imuVarianceAccel[1] = defaultBias[10];
            stateVariances.imuVarianceAccel[2] = defaultBias[11];
		
            printf("Calibration... DONE\n");
            lcm.publish("stateVariancesOrientCF", &stateVariances);
            statusCalib = 0; 
            }   
	}

	else //do static calib procedure
	{
		//set stage of calibration procedure to calibrting and init values
		if(statusCalib == -2)
		{
		    printf("Starting calibration...\n");
		    starttimeCalib = GetTimeStamp();
		    statusCalib = -1;

		    imuCalib.gyro[0]  = imuRaw.gyro[0];
		    imuCalib.gyro[1]  = imuRaw.gyro[1];
		    imuCalib.gyro[2]  = imuRaw.gyro[2];
		    imuCalib.accel[0] = imuRaw.accel[0];
		    imuCalib.accel[1] = imuRaw.accel[1];
		    imuCalib.accel[2] = imuRaw.accel[2];
		    imuCalib.magn[0]  = imuRaw.magn[0];
		    imuCalib.magn[1]  = imuRaw.magn[1];
		    imuCalib.magn[2]  = imuRaw.magn[2];
		    imuCalib.baro = imuRaw.baro;

		    stateVariances.imuVarianceGyro[0] = 0.0;
		    stateVariances.imuVarianceGyro[1] = 0.0;
		    stateVariances.imuVarianceGyro[2] = 0.0;

		    stateVariances.imuVarianceAccel[0] = 0.0;
		    stateVariances.imuVarianceAccel[1] = 0.0;
		    stateVariances.imuVarianceAccel[2] = 0.0;

		    deltaGyro[0] = imuRaw.gyro[0];
		    deltaGyro[1] = imuRaw.gyro[1];
		    deltaGyro[2] = imuRaw.gyro[2];
		    meanGyro[0] = imuRaw.gyro[0];
		    meanGyro[1] = imuRaw.gyro[1];
		    meanGyro[2] = imuRaw.gyro[2];

		    deltaAccel[0] = imuRaw.accel[0];
		    deltaAccel[1] = imuRaw.accel[1];
		    deltaAccel[2] = imuRaw.accel[2];
		    meanAccel[0] = imuRaw.accel[0];
		    meanAccel[1] = imuRaw.accel[1];
		    meanAccel[2] = imuRaw.accel[2];

		    nMeasurements = 1;

		}
		//collecting data over CALIBINTERVAL time
		else if((statusCalib == -1) && ((GetTimeStamp() - starttimeCalib) < CALIBINTERVAL_IMU * MS2US))
		{
		    int n = nMeasurements;

		    imuCalib.gyro[0]  = imuCalib.gyro[0] * n / (n + 1) + imuRaw.gyro[0] / (n + 1);
		    imuCalib.gyro[1]  = imuCalib.gyro[1] * n / (n + 1) + imuRaw.gyro[1] / (n + 1);
		    imuCalib.gyro[2]  = imuCalib.gyro[2] * n / (n + 1) + imuRaw.gyro[2] / (n + 1);
		    imuCalib.accel[0] = imuCalib.accel[0] * n / (n + 1) + imuRaw.accel[0] / (n + 1);
		    imuCalib.accel[1] = imuCalib.accel[1] * n / (n + 1) + imuRaw.accel[1] / (n + 1);
		    imuCalib.accel[2] = imuCalib.accel[2] * n / (n + 1) + imuRaw.accel[2] / (n + 1);
		    imuCalib.magn[0]  = imuCalib.magn[0] * n / (n + 1) + imuRaw.magn[0] / (n + 1);
		    imuCalib.magn[1]  = imuCalib.magn[1] * n / (n + 1) + imuRaw.magn[1] / (n + 1);
		    imuCalib.magn[2]  = imuCalib.magn[2] * n / (n + 1) + imuRaw.magn[2] / (n + 1);
		    imuCalib.baro = imuCalib.baro * n / (n + 1) + imuRaw.baro / (n + 1);

		    //gyro variance
		    deltaGyro[0] = imuRaw.gyro[0] - meanGyro[0];
		    deltaGyro[1] = imuRaw.gyro[1] - meanGyro[1];
		    deltaGyro[2] = imuRaw.gyro[2] - meanGyro[2];
		    meanGyro[0] = meanGyro[0] + deltaGyro[0] / n; //@TODO why not n+1? or above n instead of n+1? link/reference would be awesome. check where to put n=n+1
		    meanGyro[1] = meanGyro[1] + deltaGyro[1] / n;
		    meanGyro[2] = meanGyro[2] + deltaGyro[2] / n;

		    stateVariances.imuVarianceGyro[0] = stateVariances.imuVarianceGyro[0] + deltaGyro[0] * (imuRaw.gyro[0] - meanGyro[0]);
		    stateVariances.imuVarianceGyro[1] = stateVariances.imuVarianceGyro[1] + deltaGyro[1] * (imuRaw.gyro[1] - meanGyro[1]);
		    stateVariances.imuVarianceGyro[2] = stateVariances.imuVarianceGyro[2] + deltaGyro[2] * (imuRaw.gyro[2] - meanGyro[2]);

		    //accel variance
		    deltaAccel[0] = imuRaw.accel[0] - meanAccel[0];
		    deltaAccel[1] = imuRaw.accel[1] - meanAccel[1];
		    deltaAccel[2] = imuRaw.accel[2] - meanAccel[2];
		    meanAccel[0] = meanAccel[0] + deltaAccel[0] / n; //@TODO why not n+1? or above n instead of n+1? link/reference would be awesome. check where to put n=n+1
		    meanAccel[1] = meanAccel[1] + deltaAccel[1] / n;
		    meanAccel[2] = meanAccel[2] + deltaAccel[2] / n;

		    stateVariances.imuVarianceAccel[0] = stateVariances.imuVarianceAccel[0] + deltaAccel[0] * (imuRaw.accel[0] - meanAccel[0]);
		    stateVariances.imuVarianceAccel[1] = stateVariances.imuVarianceAccel[1] + deltaAccel[1] * (imuRaw.accel[1] - meanAccel[1]);
		    stateVariances.imuVarianceAccel[2] = stateVariances.imuVarianceAccel[2] + deltaAccel[2] * (imuRaw.accel[2] - meanAccel[2]);



		    nMeasurements += 1;
		}
		else if(statusCalib == -1)
		{


		    //finishing up calib
		    stateVariances.imuBiasGyro[0] = imuCalib.gyro[0];
		    stateVariances.imuBiasGyro[1] = imuCalib.gyro[1];
		    stateVariances.imuBiasGyro[2] = imuCalib.gyro[2];

		    //@TODO ok to also store this here?
		    stateVariances.imuBiasAccel[0] = imuCalib.accel[0];
		    stateVariances.imuBiasAccel[1] = imuCalib.accel[1];
		    stateVariances.imuBiasAccel[2] = imuCalib.accel[2] - GRAVITY; //@TODO warning: this assumes planar calibration!


		    stateVariances.imuVarianceGyro[0] = stateVariances.imuVarianceGyro[0] / (nMeasurements - 2);
		    stateVariances.imuVarianceGyro[1] = stateVariances.imuVarianceGyro[1] / (nMeasurements - 2);
		    stateVariances.imuVarianceGyro[2] = stateVariances.imuVarianceGyro[2] / (nMeasurements - 2);

		    stateVariances.imuVarianceAccel[0] = stateVariances.imuVarianceAccel[0] / (nMeasurements - 2);
		    stateVariances.imuVarianceAccel[1] = stateVariances.imuVarianceAccel[1] / (nMeasurements - 2);
		    stateVariances.imuVarianceAccel[2] = stateVariances.imuVarianceAccel[2] / (nMeasurements - 2);

		    /*//stateEstimatorOrientV1 uses calib reslts as 0-reference vector
		    features.featureDirectionVersor[0][0] = imuCalib.accel[0];
		    features.featureDirectionVersor[0][1] = imuCalib.accel[1];
		    features.featureDirectionVersor[0][2] = imuCalib.accel[2];

		    features.featureDirectionVersor[1][0] = imuCalib.magn[0];
		    features.featureDirectionVersor[1][1] = imuCalib.magn[1];
		    features.featureDirectionVersor[1][2] = imuCalib.magn[2];
		    */

		    //ATTENTION: we publish the calibration data over estimation channels! -> estimator waits for this first message on this channel!
		    //lcm.publish ("features", &features);
		    //lcm.publish ("stateVariancesOrientV1", &stateVariances);

		    printf("Calibration DONE...\n >> %f %f %f %f %f %f %f %f %f %f :: %f %f %f %f %f %f <<\n",
		           imuCalib.gyro[0], imuCalib.gyro[1],
		           imuCalib.gyro[2], imuCalib.accel[0],
		           imuCalib.accel[1], imuCalib.accel[2],
		           imuCalib.magn[0], imuCalib.magn[1],
		           imuCalib.magn[2], imuCalib.baro,
			   stateVariances.imuVarianceGyro[0],
				stateVariances.imuVarianceGyro[1],
				stateVariances.imuVarianceGyro[2],
				stateVariances.imuVarianceAccel[0],
				stateVariances.imuVarianceAccel[1],
				stateVariances.imuVarianceAccel[2]);

		    lcm.publish("stateVariancesOrientCF", &stateVariances);

		    statusCalib = 0;

		}
		else if(statusCalib == 0)
		{
		    /*
		    //This publishes accelerometer and magnetometervectors as featureDirectionVersors; later this job will be done by vectors pointing to the visual markers!
		    features.featureDirectionVersor[0][0] = imuRaw.accel[0];
		    features.featureDirectionVersor[0][1] = imuRaw.accel[1];
		    features.featureDirectionVersor[0][2] = imuRaw.accel[2];

		    features.featureDirectionVersor[1][0] = imuRaw.magn[0];
		    features.featureDirectionVersor[1][1] = imuRaw.magn[1];
		    features.featureDirectionVersor[1][2] = imuRaw.magn[2];
		    lcm.publish ("features", &features);
		    */
		};
	};
        /* Publishing computation result */
        //imuRaw.timestampJetson = GetTimeStamp(); //moved to closer when actually read from USB @TODO estimators should work with arduino clock when figuring out dt to last measurement, however: pay attention to clocksync!

        //	if( vec[0]!=0 && vec[1]!=0 && vec[2]!=0 && vec[3]!=0 && vec[4]!=0 && vec[5]!=0 && vec[6]!=0 && vec[7]!=0 && vec[8]!=0 && vec[9]!=0) //@TODO should we add this?
        {
            lcm.publish("imuRaw", &imuRaw);
        }
        /*---------*/

    }
    else
    {
        //nothing read;
    }


    /*General Infrastructure (maintain this structure!)*/    
    updateComputationInterval();
    return TRUE;
    /*---------*/
}
/*--------------------------*/





/*
Implementation of loop function for publishing statusPod
*/

bool imuAcquisition_t::updateStatus()
{
    imuAcquisition_t* podWorker = this;
    std::lock_guard<std::mutex> guard(podMutex);
    messageStatus_t messageStatus = checkMessagesUptodate();
    /*---------*/

    /*Computation statusPOD*/

    if((computationInterval > MAXPODDELAY_X * callInterval * MS2US) && (statusCalib == 0))
    {
	printf("%s: delay in computation, dt=% " PRId64 "us at t=%" PRId64 "! (POD only set to critical instead of usual fatal!)\n", podName.c_str(), computationInterval,GetTimeStamp());
        statusPod.status = POD_CRITICAL;
    }

    else if((computationInterval > MAXPODDELAY_NOTIFY_X * callInterval * MS2US) && (statusCalib == 0))
    {
        printf("\t %s (noncritical notification): delay in computation, dt=% " PRId64 "us!\n", podName.c_str(), computationInterval);        
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
    ;

    // 1) Create the app
    imuAcquisition_t podWorker = imuAcquisition_t("imuAcquisition", CALLINTERVAL_IMUACQUISITION); 	//provide your PODname here!

    if(argc < 2)
    {
        printf("Please provide USB port of IMU-reading arduino (e.g. ttyUSB0)!\n");
        return EXIT_FAILURE;
    };

    if((argc == 3) &&  (strcmp(argv[2], "noCalib") == 0))
    {
	podWorker.noCalib = true;
        printf("No calibration, using preset values!\n");
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


    /* Open the file descriptor in non-blocking mode */ //how does that happen with these flags?
    podWorker.fd = open(podWorker.usbPortname.c_str(), O_RDWR | O_NOCTTY); //@TODO c_cc[VMIN] and c_cc[VTIME]=0 should create nonblocking serial read, didnt work?
    //or //podWorker.fd = open(podWorker.usbPortname.c_str(), O_RDWR | O_NOCTTY | FNDELAY | O_NONBLOCK); //will ignroe c_cc[VMIN], but should be nonblocking anyway..ditnd work?



    /* Set up the control structure */
    struct termios toptions;

    /* Get currently set options for the tty */
    tcgetattr(podWorker.fd, &toptions);

    /* Set custom options */


    /* 115200 baud */
    cfsetispeed(&toptions, B115200);
    cfsetospeed(&toptions, B115200);
    /* 57600 baud */
    /*cfsetispeed(&toptions, B57600);
    cfsetospeed(&toptions, B57600);*/
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
    toptions.c_cc[VMIN] = 0;
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







