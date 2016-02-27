
#include "stateEstimatorOrientCF.hpp"


using namespace std;

void complimentaryfilter(double yaw_old,  double pitch_old, double roll_old,  double dt, double* sensorIMU, double* euler_hat) //this is nasty, should be a member function, but then it cannot be called from within static podBase_t compuation function. See suggestion on github-issues on "can multiple pods be run simulatenously
{



    double gyroAngleUpdate_acc_threshold   = sqrt(3 * pow(0.003, 2)); //@TODO this seems to work for the real IMU, note, for simulated one might use different!
    double gyroAngleUpdate_acc_weight      = 0.0005;


//Rotation of angular velocity vector from Bodyframe to Worldframe, inverted Wronskian (body rates p-q-r to euler rates yaw pitch roll)
    double w_euler[3];

    double p = sensorIMU[3];
    double q = sensorIMU[4];
    double r = sensorIMU[5];

    w_euler[0] = sin(roll_old) / cos(pitch_old) * q + cos(roll_old) / cos(pitch_old) * r;
    w_euler[1] = cos(roll_old) * q - sin(roll_old) * r;
    w_euler[2] = p + sin(roll_old) * tan(pitch_old) * q + cos(roll_old) * tan(pitch_old) * r;

//Integrate gyroscope data
    double roll  = roll_old  + (w_euler[2]) * dt;
    double pitch = pitch_old + (w_euler[1]) * dt;
    double yaw   = yaw_old   + (w_euler[0]) * dt;


//Compensate for drift with accelerometer data if in un-accelerated flight
    double imuaccl_abs = sqrt(pow(sensorIMU[0], 2) + pow(sensorIMU[1], 2) + pow(sensorIMU[2], 2));

    if((imuaccl_abs > (1 - gyroAngleUpdate_acc_threshold) * (GRAVITY)) && (imuaccl_abs < (1 + gyroAngleUpdate_acc_threshold) * (GRAVITY)))
    {
        double roll_hat_acc = atan(sensorIMU[1] / sensorIMU[2]);
        roll         = roll * (1 - gyroAngleUpdate_acc_weight) + roll_hat_acc * gyroAngleUpdate_acc_weight;

        double pitch_hat_acc = asin(sensorIMU[0] / GRAVITY) ;
        pitch        = pitch * (1 - gyroAngleUpdate_acc_weight) + pitch_hat_acc * gyroAngleUpdate_acc_weight;
    }

    euler_hat[0] = yaw;
    euler_hat[1] = pitch;
    euler_hat[2] = roll;


}


/*
Implementation of loop function for computations in this specific POD
*/

bool stateEstimatorOrientCF_t::doComputations()
{
    stateEstimatorOrientCF_t* podWorker = this;
    std::lock_guard<std::mutex> guard(podMutex);

    /*--------*/

    if(statusPod.status == POD_OK)
    {
        /* Computations */



        // Correct new measurements from bias

        double imuOrig[6];

        imuOrig[0] = imudata.accel[0] - biases.accel[0]; 
        imuOrig[1] = imudata.accel[1] - biases.accel[1];
        imuOrig[2] = imudata.accel[2] - biases.accel[2]; //printf("%f %f\n",imudata.accel[2],biases.accel[2]);
        imuOrig[3] = imudata.gyro[0] - biases.gyro[0];
        imuOrig[4] = imudata.gyro[1] - biases.gyro[1];
        imuOrig[5] = imudata.gyro[2] - biases.gyro[2];


        //Apply IIR filter to Accel
        double imuFiltered[6];
        imuFiltered[0] = IIRIMU * imuFiltered[0] + (1.0 - IIRIMU) * imuOrig[0];
        imuFiltered[1] = IIRIMU * imuFiltered[1] + (1.0 - IIRIMU) * imuOrig[1];
        imuFiltered[2] = IIRIMU * imuFiltered[2] + (1.0 - IIRIMU) * imuOrig[2];
        imuFiltered[3] = imuOrig[3];
        imuFiltered[4] = imuOrig[4];
        imuFiltered[5] = imuOrig[5];


        //Rotate measurements from IMU system to body-frame (assuming it is first 90deg yaw, then 180 to take vectorin IMUsyste to represent in bodyframe sys!

        //accels
        double imuTrafo[6];

        if(imuRawChannel == "imuRaw")
        {
            imuTrafo[0] = -imuFiltered[1];
            imuTrafo[1] = -imuFiltered[0];
            imuTrafo[2] = -imuFiltered[2];

            //rot
            imuTrafo[3] = -imuFiltered[4];
            imuTrafo[4] = -imuFiltered[3];
            imuTrafo[5] = -imuFiltered[5];
        }
        else //imuRawData from simulation
        {
            imuTrafo[0] = imuFiltered[0];
            imuTrafo[1] = imuFiltered[1];
            imuTrafo[2] = imuFiltered[2];

            //rot
            imuTrafo[3] = imuFiltered[3];
            imuTrafo[4] = imuFiltered[4];
            imuTrafo[5] = imuFiltered[5];
        }

	//printf("imu raw-body: %f %f %f %f %f %f ::%f %f %f %f %f %f\n",imuFiltered[0],imuFiltered[1],imuFiltered[2],imuFiltered[3],imuFiltered[4],imuFiltered[5],imuTrafo[0],imuTrafo[1],imuTrafo[2],imuTrafo[3],imuTrafo[4],imuTrafo[5]);



        //prepare current estimates
        double yaw_cur, pitch_cur, roll_cur;
        quat2Euler(stateVariances.orient, &(yaw_cur), &(pitch_cur), &(roll_cur));

        //feed complimentary filter
        double euler_hat[3];
        int64_t nowCompUpdate = GetTimeStamp();		

        double dt = (nowCompUpdate - stateVariances.timestampJetson) / (1000000.0); //printf("dt: %f\n",dt); //@TODO should probably refer to Arduino clock, but then we need some sort of clock sync!


        if(dt > 10) dt = 0.01; //initial timestamp
        complimentaryfilter(yaw_cur, pitch_cur, roll_cur, dt, imuTrafo, euler_hat);

        //transform to quaternions and update stateVariances
	//printf("%f\n",roll_cur/3.14*180);
        Euler2quat(stateVariances.orient, &(euler_hat[0]), &(euler_hat[1]), &(euler_hat[2]));
        stateVariances.veloOrientBody[0] = imuTrafo[3];
        stateVariances.veloOrientBody[1] = imuTrafo[4];
        stateVariances.veloOrientBody[2] = imuTrafo[5];

        stateVariances.timestampJetson = nowCompUpdate;

        /* Publishing computation result*/

	
	//printf("imuRaw-msgage: \t%" PRId64 "\n",messageAdmin["imuRaw"].timestampJetsonLastReceived-imudata.timestampJetson);
	//printf("msg received last to pub now: \t%" PRId64 "\n",GetTimeStamp()-messageAdmin["imuRaw"].timestampJetsonLastReceived);
	//printf("dt imuraw got from serial - estimate being published: \t%" PRId64 "\n",GetTimeStamp()-imudata.timestampJetson);

	stateVariances.deltaSensAcquiToEstimPub = GetTimeStamp()-imudata.timestampJetson;

        // - publish
        lcm.publish("stateVariancesOrientCF", &stateVariances);

        /*---------*/
    }

    updateComputationInterval();
    return TRUE;
    /*---------*/
}
/*--------------------------*/





/*
Implementation of loop function for publishing statusPod
*/

bool stateEstimatorOrientCF_t::updateStatus()
{
    stateEstimatorOrientCF_t* podWorker = this;
    messageStatus_t messageStatus = checkMessagesUptodate();
    std::lock_guard<std::mutex> guard(podMutex);
    /*---------*/

    /*Computation statusPOD*/
    if(computationInterval > MAXPODDELAY_X * callInterval * MS2US)
    {
      printf("%s: delay in computation, dt=% " PRId64 "us at t=%" PRId64 "!\n", podName.c_str(), computationInterval,GetTimeStamp());
        statusPod.status = POD_FATAL;
    }
    else if((messageStatus == MSGS_LATE))
    {
        statusPod.status = POD_CRITICAL;
    }
    else if((messageStatus == MSGS_DEAD))
    {
        statusPod.status = POD_FATAL;
    }
    else
    {
        if(isGotBiases == -1)
        {
            printf("Waiting for IMU Calibration to end...\n");
            isGotBiases = 0;
        }
        else if(isGotBiases == 0)
        {
            if(stateVariances.imuBiasAccel[0] != 0 ||
                    stateVariances.imuBiasAccel[1] != 0 ||
                    stateVariances.imuBiasAccel[2] != 0)
            {
                printf("IMU calibration ok! Continuing...\n");                

                biases.accel[0] = stateVariances.imuBiasAccel[0];
                biases.accel[1] = stateVariances.imuBiasAccel[1];
                biases.accel[2] = stateVariances.imuBiasAccel[2];

                biases.gyro[0] = stateVariances.imuBiasGyro[0];
                biases.gyro[1] = stateVariances.imuBiasGyro[1];
                biases.gyro[2] = stateVariances.imuBiasGyro[2];

                //initial values for IIR-IMU filter
                imuFiltered[0] = biases.accel[0];
                imuFiltered[1] = biases.accel[1];
                imuFiltered[2] = biases.accel[2] - GRAVITY;
                imuFiltered[0] = biases.gyro[0];
                imuFiltered[1] = biases.gyro[1];
                imuFiltered[2] = biases.gyro[2];
                /*
                imuRawLast[0] = biases.accel[0];
                imuRawLast[1] = biases.accel[1];
                imuRawLast[2] = biases.accel[2]-GRAVITY;
                imuRawLast[0] = biases.gyro[0];
                imuRawLast[1] = biases.gyro[1];
                imuRawLast[2] = biases.gyro[2];
                */
                //set estimates to intial value
                stateVariances.orient[0] = 1.0;
                stateVariances.orient[1] = 0.0;
                stateVariances.orient[2] = 0.0;
                stateVariances.orient[3] = 0.0;

                stateVariances.position[0] = 0.0;
                stateVariances.position[1] = 0.0;
                stateVariances.position[2] = 0.0;

                stateVariances.veloPositionBody[0] = 0.0;
                stateVariances.veloPositionBody[1] = 0.0;
                stateVariances.veloPositionBody[2] = 0.0;

                stateVariances.veloOrientBody[0] = 0.0;
                stateVariances.veloOrientBody[1] = 0.0;
                stateVariances.veloOrientBody[2] = 0.0;


                //after getting the initial versors, unsubscribe
                unsubscribe("stateVariancesOrientCF");

                isGotBiases = 1;

                //statusPod.status = POD_OK;

            }
            else if(imuRawChannel == "imuRawSim")
            {
                printf("IMU calibration ok! Continuing...it's only simulated anyway!\n");
                isGotBiases == 1;

                biases.accel[0] = 0.0;
                biases.accel[1] = 0.0;
                biases.accel[2] = 0.0;

                biases.gyro[0] = 0.0;
                biases.gyro[1] = 0.0;
                biases.gyro[2] = 0.0;


                //initial values for IIR-IMU filter
                imuFiltered[0] = 0.0;
                imuFiltered[1] = 0.0;
                imuFiltered[2] = -GRAVITY;
                imuFiltered[0] = 0.0;
                imuFiltered[1] = 0.0;
                imuFiltered[2] = 0.0;
                /*
                imuRawLast[0] = 0.0;
                imuRawLast[1] = 0.0;
                imuRawLast[2] = -GRAVITY;
                imuRawLast[0] = 0.0;
                imuRawLast[1] = 0.0;
                imuRawLast[2] = 0.0;
                */
                //set estimates to intial value
                stateVariances.orient[0] = 1.0;
                stateVariances.orient[1] = 0.0;
                stateVariances.orient[2] = 0.0;
                stateVariances.orient[3] = 0.0;

                stateVariances.position[0] = 0.0;
                stateVariances.position[1] = 0.0;
                stateVariances.position[2] = 0.0;

                stateVariances.veloPositionBody[0] = 0.0;
                stateVariances.veloPositionBody[1] = 0.0;
                stateVariances.veloPositionBody[2] = 0.0;

                stateVariances.veloOrientBody[0] = 0.0;
                stateVariances.veloOrientBody[1] = 0.0;
                stateVariances.veloOrientBody[2] = 0.0;


                //after getting the initial versors, unsubscribe
                unsubscribe("stateVariancesOrientCF");
                //unsubscribe("features");

                isGotBiases = true;
		//statusPod.status = POD_OK;
                
            };	
        }

    else if(isGotBiases == 1) statusPod.status = POD_OK;	
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
    stateEstimatorOrientCF_t podWorker = stateEstimatorOrientCF_t("stateEstimatorOrientCF", CALLINTERVAL_STATEESTIMATORORIENTCF);

    if(argc < 2)
    {
        printf("Please provide imuRaw-Channel used for estimation!\n");
        return EXIT_FAILURE;
    };

    podWorker.imuRawChannel = argv[1];

    // 2) Create LCM
    if(!podWorker.lcm.good())
        return 1;

    // 3) Subscribe this POD to channels
    podWorker.subscribe(podWorker.imuRawChannel, CALLINTERVAL_IMUACQUISITION, &(podWorker.imudata),
                        &podBase_t::handleMessage<agile::imuRaw_t>);

    podWorker.subscribe("stateVariancesOrientCF", CALLINTERVAL_IMUACQUISITION,
                        &(podWorker.stateVariances), &podBase_t::handleMessage<agile::stateVariances_t>);

    // podWorker.subscribe("features", CALLINTERVAL_IMUACQUISITION, &(podWorker.features),&podBase_t::handleMessage<agile::features_t>);
    /*---------*/




    /*  POD-specific init procedures  */
    // Update and publish status of Pod (this POD is the first to start)
    printf("Initializing stateEstimatorOrientCF...\n");
    podWorker.publishStatus(POD_INITING);


    printf("Initializing stateEstimatorOrientCF... DONE\n");
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







