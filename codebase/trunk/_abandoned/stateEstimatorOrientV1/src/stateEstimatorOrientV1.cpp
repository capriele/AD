
#include "stateEstimatorOrientV1.hpp"
#include <qekf.c> //@TODO remove include of .c! only include headers!
#include <mrdivide.c>
#include <norm.c>


using namespace std;


/*

NOT UPDATED/WORKED ON ANY LONGER

*/


/*
Implementation of loop function for computations in this specific POD
*/

bool stateEstimatorOrientV1_t::doComputations()
{
    stateEstimatorOrientV1_t* podWorker = this;
    std::lock_guard<std::mutex> guard(podMutex);


    /*--------*/

    if(statusPod.status == POD_OK)
    {
        /* Computations */

        imudata.timestampJetson = GetTimeStamp();
        B[0] = imudata.timestampJetson / 1000000.0;


        //@TODO alignment/rotation of sensor measurements with body frame axes!
        B[1] = imudata.gyro[0];
        B[2] = imudata.gyro[1];
        B[3] = imudata.gyro[2];

        /*hacked trafo*/
        /*
        B[1] = imudata.gyro[2];
        B[2] = -imudata.gyro[1];
        B[3] = -imudata.gyro[0];
        */


        length1 = pow(imudata.accel[0], 2);
        length1 += pow(imudata.accel[1], 2);
        length1 += pow(imudata.accel[2], 2);
        length1 = sqrt(length1);

        B[4] = imudata.accel[0] / length1;
        B[5] = imudata.accel[1] / length1;
        B[6] = imudata.accel[2] / length1;

        length2 = pow(imudata.magn[0], 2);
        length2 += pow(imudata.magn[1], 2);
        length2 += pow(imudata.magn[2], 2);
        length2 = sqrt(length2);

        B[7] = imudata.magn[0] / length2;
        B[8] = imudata.magn[1] / length2;
        B[9] = imudata.magn[2] / length2;

        qekf(q, cov_matrixes, w_stats, reference_versors, B, &prev_T, cov_matrixes->P);


        //Note: estimator works with q=[0,0,0,1] quaternion convention, everything else with [1,0,0,0]
        stateVariances.orient[0] = q[3];
        stateVariances.orient[1] = q[0];
        stateVariances.orient[2] = q[1];
        stateVariances.orient[3] = q[2];

        stateVariances.timestampJetson = GetTimeStamp();

        /* Publishing computation result, here status update of drone status */

        // statusDrone - publish
        lcm.publish("stateVariancesOrientV1", &stateVariances);

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

bool stateEstimatorOrientV1_t::updateStatus()
{
    stateEstimatorOrientV1_t* podWorker = this;
    /*---------*/



    /*Computation statusPOD*/
    if(computationInterval > MAXPODDELAY_X * callInterval * MS2US)
    {
        printf("stateEstimatorOrientV1: delay occured; comp interval % " PRId64 "us!\n",
               computationInterval);
        statusPod.status = POD_FATAL;
    }
    else if((checkMessagesUptodate() == MSGS_LATE))
    {
        statusPod.status = POD_CRITICAL;
    }
    else if((checkMessagesUptodate() == MSGS_DEAD))
    {
        statusPod.status = POD_FATAL;
    }
    else
    {
        if(got_initial_features == -1)
        {
            printf("Waiting for IMU Calibration to end...\n");
            got_initial_features = 0;
        }
        else if(got_initial_features == 0)
        {
            if(features.featureDirectionVersor[0][0] != 0 ||
                    features.featureDirectionVersor[0][1] != 0 ||
                    features.featureDirectionVersor[0][2] != 0)
            {
                printf("IMU calibration ok! Continuing...\n");
                got_initial_features == 1;

                //@TODO: generalize for n-versors when using the camera
                reference_versors->r1[0] = features.featureDirectionVersor[0][0];
                reference_versors->r1[1] = features.featureDirectionVersor[0][1];
                reference_versors->r1[2] = features.featureDirectionVersor[0][2];

                reference_versors->r2[0] = features.featureDirectionVersor[1][0];
                reference_versors->r2[1] = features.featureDirectionVersor[1][1];
                reference_versors->r2[2] = features.featureDirectionVersor[1][2];

                //@TODO: this could be removed after uptading the imu calibration
                //(still need to be implemented)
                sigma_b1 = 0.007;
                sigma_b2 = 0.005;


                //@TODO alignment/rotation of sensor measurements with body frame axes!
                w_stats->sigma.x = stateVariances.imuVarianceGyro[0];
                w_stats->sigma.y = stateVariances.imuVarianceGyro[1];
                w_stats->sigma.z = stateVariances.imuVarianceGyro[2];

                w_stats->bias.x = stateVariances.imuBiasGyro[0];
                w_stats->bias.y = stateVariances.imuBiasGyro[1];
                w_stats->bias.z = stateVariances.imuBiasGyro[2]; //@TODO gravity"bias?"


                /*
                //hacked trafo
                w_stats->sigma.x = stateVariances.imuVarianceGyro[2];
                w_stats->sigma.y = stateVariances.imuVarianceGyro[1];
                w_stats->sigma.z = stateVariances.imuVarianceGyro[0];

                w_stats->bias.x = stateVariances.imuBiasGyro[2];
                w_stats->bias.y = -stateVariances.imuBiasGyro[1];
                w_stats->bias.z = -stateVariances.imuBiasGyro[0];
                */



                cov_matrixes->Q[0] = pow(w_stats->sigma.x, 2);
                cov_matrixes->Q[4] = pow(w_stats->sigma.y, 2);
                cov_matrixes->Q[8] = pow(w_stats->sigma.z, 2);

                cov_matrixes->R[0] = pow(sigma_b1, 2);
                cov_matrixes->R[7] = pow(sigma_b1, 2);
                cov_matrixes->R[14] = pow(sigma_b1, 2);
                cov_matrixes->R[21] = pow(sigma_b2, 2);
                cov_matrixes->R[28] = pow(sigma_b2, 2);
                cov_matrixes->R[35] = pow(sigma_b2, 2);

                imudata.timestampJetson = GetTimeStamp();
                B[0] = imudata.timestampJetson;

                //@TODO alignment/rotation of sensor measurements with body frame axes!
                B[1] = imudata.gyro[0];
                B[2] = imudata.gyro[1];
                B[3] = imudata.gyro[2];


                /*
                //hacked trafo
                B[1] = imudata.gyro[2];
                B[2] = -imudata.gyro[1];
                B[3] = -imudata.gyro[0];
                */

                length1 = pow(imudata.accel[0], 2);
                length1 += pow(imudata.accel[1], 2);
                length1 += pow(imudata.accel[2], 2);
                length1 = sqrt(length1);

                B[4] = imudata.accel[0] / length1;
                B[5] = imudata.accel[1] / length1;
                B[6] = imudata.accel[2] / length1;

                length2 = pow(imudata.magn[0], 2);
                length2 += pow(imudata.magn[1], 2);
                length2 += pow(imudata.magn[2], 2);
                length2 = sqrt(length2);

                B[7] = imudata.magn[0] / length2;
                B[8] = imudata.magn[1] / length2;
                B[9] = imudata.magn[2] / length2;

                prev_T = imudata.timestampJetson / 1000000.0;

                //after getting the initial versors, unsubscribe
                unsubscribe("stateVariancesOrientV1");

                got_initial_features = true;

                statusPod.status = POD_OK;

            }
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
    stateEstimatorOrientV1_t podWorker = stateEstimatorOrientV1_t("stateEstimatorOrientV1", CALLINTERVAL_STATEESTIMATORORIENTV1);

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

    podWorker.subscribe("stateVariancesOrientV1", CALLINTERVAL_STATEESTIMATORORIENTV1,
                        &(podWorker.stateVariances), &podBase_t::handleMessage<agile::stateVariances_t>);

    podWorker.subscribe("features", CALLINTERVAL_IMUACQUISITION, &(podWorker.features),
                        &podBase_t::handleMessage<agile::features_t>);


    /*---------*/




    /*  POD-specific init procedures  */
    // Update and publish status of Pod (this POD is the first to start)
    printf("Initializing stateEstimatorOrientV1...\n");
    podWorker.publishStatus(POD_INITING);


    printf("Initializing stateEstimatorOrientV1... DONE\n");
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







