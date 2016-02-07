
#include "stateEstimatorOrientV1.hpp"
#include <qekf.c> //@TODO remove include of .c! only include headers!
#include <mrdivide.c>
#include <norm.c>


using namespace std;


/*
Implementation of loop function for computations in this specific POD
*/

gboolean podBase_t::gtimerfuncComputations(gpointer data)
{

    /* General Infrastructure (maintain this structure!) */
    stateEstimatorOrientV1_t* podWorker = (stateEstimatorOrientV1_t*) data;
    std::lock_guard<std::mutex> guard(podMutex);


    /*--------*/

    if(podWorker->statusPod.status == POD_OK)
    {
        /* Computations */

        podWorker->imudata.timestampJetson = GetTimeStamp();
        podWorker->B[0] = podWorker->imudata.timestampJetson / 1000000.0;


        //@TODO alignment/rotation of sensor measurements with body frame axes!
        podWorker->B[1] = podWorker->imudata.gyro[0];
        podWorker->B[2] = podWorker->imudata.gyro[1];
        podWorker->B[3] = podWorker->imudata.gyro[2];

        /*hacked trafo*/
        /*
        podWorker->B[1] = podWorker->imudata.gyro[2];
        podWorker->B[2] = -podWorker->imudata.gyro[1];
        podWorker->B[3] = -podWorker->imudata.gyro[0];
        */


        podWorker->length1 = pow(podWorker->imudata.accel[0], 2);
        podWorker->length1 += pow(podWorker->imudata.accel[1], 2);
        podWorker->length1 += pow(podWorker->imudata.accel[2], 2);
        podWorker->length1 = sqrt(podWorker->length1);

        podWorker->B[4] = podWorker->imudata.accel[0] / podWorker->length1;
        podWorker->B[5] = podWorker->imudata.accel[1] / podWorker->length1;
        podWorker->B[6] = podWorker->imudata.accel[2] / podWorker->length1;

        podWorker->length2 = pow(podWorker->imudata.magn[0], 2);
        podWorker->length2 += pow(podWorker->imudata.magn[1], 2);
        podWorker->length2 += pow(podWorker->imudata.magn[2], 2);
        podWorker->length2 = sqrt(podWorker->length2);

        podWorker->B[7] = podWorker->imudata.magn[0] / podWorker->length2;
        podWorker->B[8] = podWorker->imudata.magn[1] / podWorker->length2;
        podWorker->B[9] = podWorker->imudata.magn[2] / podWorker->length2;

        qekf(podWorker->q, podWorker->cov_matrixes, podWorker->w_stats, podWorker->reference_versors, podWorker->B, &podWorker->prev_T, podWorker->cov_matrixes->P);


        //Note: estimator works with q=[0,0,0,1] quaternion convention, everything else with [1,0,0,0]
        podWorker->stateVariances.orient[0] = podWorker->q[3];
        podWorker->stateVariances.orient[1] = podWorker->q[0];
        podWorker->stateVariances.orient[2] = podWorker->q[1];
        podWorker->stateVariances.orient[3] = podWorker->q[2];

        podWorker->stateVariances.timestampJetson = GetTimeStamp();

        /* Publishing computation result, here status update of drone status */

        // statusDrone - publish
        podWorker->lcm.publish("stateVariancesOrientV1", &podWorker->stateVariances);

        /*---------*/
    }

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
    stateEstimatorOrientV1_t* podWorker = (stateEstimatorOrientV1_t*) data;
    /*---------*/



    /*Computation statusPOD*/
    if(podWorker->computationInterval > MAXPODDELAY_X * podWorker->callInterval * MS2US)
    {
        printf("stateEstimatorOrientV1: delay occured of % " PRId64 "us!\n",
               podWorker->computationInterval);
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
        if(podWorker->got_initial_features == -1)
        {
            printf("Waiting for IMU Calibration to end...\n");
            podWorker->got_initial_features = 0;
        }
        else if(podWorker->got_initial_features == 0)
        {
            if(podWorker->features.featureDirectionVersor[0][0] != 0 ||
                    podWorker->features.featureDirectionVersor[0][1] != 0 ||
                    podWorker->features.featureDirectionVersor[0][2] != 0)
            {
                printf("IMU calibration ok! Continuing...\n");
                podWorker->got_initial_features == 1;

                //@TODO: generalize for n-versors when using the camera
                podWorker->reference_versors->r1[0] = podWorker->features.featureDirectionVersor[0][0];
                podWorker->reference_versors->r1[1] = podWorker->features.featureDirectionVersor[0][1];
                podWorker->reference_versors->r1[2] = podWorker->features.featureDirectionVersor[0][2];

                podWorker->reference_versors->r2[0] = podWorker->features.featureDirectionVersor[1][0];
                podWorker->reference_versors->r2[1] = podWorker->features.featureDirectionVersor[1][1];
                podWorker->reference_versors->r2[2] = podWorker->features.featureDirectionVersor[1][2];

                //@TODO: this could be removed after uptading the imu calibration
                //(still need to be implemented)
                podWorker->sigma_b1 = 0.007;
                podWorker->sigma_b2 = 0.005;


                //@TODO alignment/rotation of sensor measurements with body frame axes!
                podWorker->w_stats->sigma.x = podWorker->stateVariances.imuVarianceGyro[0];
                podWorker->w_stats->sigma.y = podWorker->stateVariances.imuVarianceGyro[1];
                podWorker->w_stats->sigma.z = podWorker->stateVariances.imuVarianceGyro[2];

                podWorker->w_stats->bias.x = podWorker->stateVariances.imuBiasGyro[0];
                podWorker->w_stats->bias.y = podWorker->stateVariances.imuBiasGyro[1];
                podWorker->w_stats->bias.z = podWorker->stateVariances.imuBiasGyro[2]; //@TODO gravity"bias?"


                /*
                //hacked trafo
                podWorker->w_stats->sigma.x = podWorker->stateVariances.imuVarianceGyro[2];
                podWorker->w_stats->sigma.y = podWorker->stateVariances.imuVarianceGyro[1];
                podWorker->w_stats->sigma.z = podWorker->stateVariances.imuVarianceGyro[0];

                podWorker->w_stats->bias.x = podWorker->stateVariances.imuBiasGyro[2];
                podWorker->w_stats->bias.y = -podWorker->stateVariances.imuBiasGyro[1];
                podWorker->w_stats->bias.z = -podWorker->stateVariances.imuBiasGyro[0];
                */



                podWorker->cov_matrixes->Q[0] = pow(podWorker->w_stats->sigma.x, 2);
                podWorker->cov_matrixes->Q[4] = pow(podWorker->w_stats->sigma.y, 2);
                podWorker->cov_matrixes->Q[8] = pow(podWorker->w_stats->sigma.z, 2);

                podWorker->cov_matrixes->R[0] = pow(podWorker->sigma_b1, 2);
                podWorker->cov_matrixes->R[7] = pow(podWorker->sigma_b1, 2);
                podWorker->cov_matrixes->R[14] = pow(podWorker->sigma_b1, 2);
                podWorker->cov_matrixes->R[21] = pow(podWorker->sigma_b2, 2);
                podWorker->cov_matrixes->R[28] = pow(podWorker->sigma_b2, 2);
                podWorker->cov_matrixes->R[35] = pow(podWorker->sigma_b2, 2);

                podWorker->imudata.timestampJetson = GetTimeStamp();
                podWorker->B[0] = podWorker->imudata.timestampJetson;

                //@TODO alignment/rotation of sensor measurements with body frame axes!
                podWorker->B[1] = podWorker->imudata.gyro[0];
                podWorker->B[2] = podWorker->imudata.gyro[1];
                podWorker->B[3] = podWorker->imudata.gyro[2];


                /*
                //hacked trafo
                podWorker->B[1] = podWorker->imudata.gyro[2];
                podWorker->B[2] = -podWorker->imudata.gyro[1];
                podWorker->B[3] = -podWorker->imudata.gyro[0];
                */

                podWorker->length1 = pow(podWorker->imudata.accel[0], 2);
                podWorker->length1 += pow(podWorker->imudata.accel[1], 2);
                podWorker->length1 += pow(podWorker->imudata.accel[2], 2);
                podWorker->length1 = sqrt(podWorker->length1);

                podWorker->B[4] = podWorker->imudata.accel[0] / podWorker->length1;
                podWorker->B[5] = podWorker->imudata.accel[1] / podWorker->length1;
                podWorker->B[6] = podWorker->imudata.accel[2] / podWorker->length1;

                podWorker->length2 = pow(podWorker->imudata.magn[0], 2);
                podWorker->length2 += pow(podWorker->imudata.magn[1], 2);
                podWorker->length2 += pow(podWorker->imudata.magn[2], 2);
                podWorker->length2 = sqrt(podWorker->length2);

                podWorker->B[7] = podWorker->imudata.magn[0] / podWorker->length2;
                podWorker->B[8] = podWorker->imudata.magn[1] / podWorker->length2;
                podWorker->B[9] = podWorker->imudata.magn[2] / podWorker->length2;

                podWorker->prev_T = podWorker->imudata.timestampJetson / 1000000.0;

                //after getting the initial versors, unsubscribe
                podWorker->unsubscribe("stateVariancesOrientV1");

                podWorker->got_initial_features = true;

                podWorker->statusPod.status = POD_OK;

            }
        };




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
    podWorker.subscribe(podWorker.imuRawChannel.c_str(), CALLINTERVAL_IMUACQUISITION, &(podWorker.imudata),
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







