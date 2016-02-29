#include <iostream>
#include <cmath>

#include <glib.h>


#include <lcm/lcm-cpp.hpp>
#include "../../framework/framework.hpp"

#include <lcmtypes/lcmtypes.hpp>

using namespace std;

/*



!!!




THIS SENDER IS NOT INTENDED TO BE AN EXAMPLE/TEMPLATE!!!


!!!




*/



class testSender_t : public podBase_t
{

public:
    int value;

    bool isExamplePod, isImuAcquisitionStatus, isImuAcquisitionImuRaw, isDetectorVis, isStateEstimatorOrientV1, isControllerPDPose, isControllerSOCPose, isControllerSOCOrient;

    agile::controlMode_t controlMode;

    //constructor
    testSender_t (string podName, int64_t callInterval) : podBase_t(podName, callInterval)
    {
        //Pod-specific initialization
        //...
        isExamplePod = false;
        isImuAcquisitionStatus = false;
        isImuAcquisitionImuRaw = false;
        isDetectorVis = false;
        isStateEstimatorOrientV1 = false;
        isControllerPDPose = false;
        isControllerSOCPose = false;
        isControllerSOCOrient = false;
    }

    bool updateStatus() override;
    bool doComputations() override;
};

bool testSender_t::updateStatus() {};

bool testSender_t::doComputations()
{

    testSender_t* podWorker = this;

//publish a statusPOD message
    agile::statusPod_t statusPod;

    statusPod.timestampJetson = GetTimeStamp();
    statusPod.status = POD_OK;
    statusPod.podName = "Testsender";

    lcm.publish("statusTestsender", &statusPod);


    /*FAKE data to create messages for some other PODS*/

    agile::stateVariances_t stateVariances; //@TODO init all values
    agile::imuRaw_t imuRaw;

    stateVariances.position[0] = 0;
    stateVariances.position[1] = 0;
    stateVariances.position[2] = 0;

    stateVariances.orient[0] 	= 1.0;
    stateVariances.orient[1] 	= 0.0;
    stateVariances.orient[2] 	= 0.0;
    stateVariances.orient[3] 	= 0.0;

    stateVariances.veloPositionBody[0] = 0.0;
    stateVariances.veloPositionBody[1] = 0.0;
    stateVariances.veloPositionBody[2] = 0.0;

    stateVariances.veloOrientBody[0] = 0.0;
    stateVariances.veloOrientBody[1] = 0.0;
    stateVariances.veloOrientBody[2] = 0.0;

    stateVariances.imuBiasAccel[0] = 0.0;
    stateVariances.imuBiasAccel[1] = 0.0;
    stateVariances.imuBiasAccel[2] = 0.0;
    stateVariances.imuBiasGyro[0] = 0.0;
    stateVariances.imuBiasGyro[1] = 0.0;
    stateVariances.imuBiasGyro[2] = 0.0;

    stateVariances.timestampJetson = GetTimeStamp();

    imuRaw.timestampArduino = GetTimeStamp();
    imuRaw.timestampJetson = GetTimeStamp();
    imuRaw.accel[0] = 0.0;
    imuRaw.accel[1] = 0.0;
    imuRaw.accel[2] = 0.0;

    imuRaw.gyro[0] = 0.0;
    imuRaw.gyro[1] = 0.0;
    imuRaw.gyro[2] = 0.0;

    imuRaw.magn[0] = 0.0;
    imuRaw.magn[1] = 0.0;
    imuRaw.magn[2] = 0.0;

    imuRaw.baro = 0.0;

    /*PUBLISH  messages for some pods!*/
    if(isImuAcquisitionStatus)
    {
        lcm.publish("statusImuAcquisition", &statusPod);
    }

    if(isImuAcquisitionImuRaw)
    {
        lcm.publish("imuRaw", &imuRaw);
    }


    if(isDetectorVis)
    {
        lcm.publish("statusDetectorVis", &statusPod);
    }

    if(isStateEstimatorOrientV1)
    {
        lcm.publish("stateVariancesOrientV1", &stateVariances);
        lcm.publish("statusStateEstimatorOrientV1", &statusPod);
    }

    if(isControllerPDPose)
    {
        lcm.publish("statusControllerPDPose", &statusPod);
    }

    if(isControllerSOCOrient)
    {
        lcm.publish("statusControllerSOCOrient", &statusPod);
    }


    if(isControllerSOCPose)
    {
        lcm.publish("statusControllerSOCPose", &statusPod);
    }

    /*for examplePOD: */
    if(isExamplePod)
    {
        lcm.publish("statusRemoteController", &statusPod);
        lcm.publish("statusControllerPDOrient", &statusPod);
        lcm.publish("statusMotorCommander", &statusPod);
        lcm.publish("statusWatchdog", &statusPod);
        lcm.publish("controlMode", &(controlMode));

        agile::pose_t pose_data;

        pose_data.timestampJetson = GetTimeStamp();
        pose_data.position[0] = 0.0;
        pose_data.position[1] = 0.0;
        pose_data.position[2] = 0.25 * sin((double)(value++) / 10);

        pose_data.orient[0] = 1.0;//0.923;//1.0;
        pose_data.orient[1] = 0.0;
        pose_data.orient[2] = 0.0;//0.0;
        pose_data.orient[3] = 0.0;//0.38268;

        lcm.publish("testdata", &pose_data);
    }

    return TRUE;
}


int main(int argc, char** argv)
{


    // Create the app
    testSender_t podWorker = testSender_t("testsender", CALLINTERVAL_TESTSENDER);


    // Create LCM
    if(!podWorker.lcm.good())
        return 1;

    podWorker.subscribe("statusWatchdog",  CALLINTERVAL_WATCHDOG, &(podWorker.statusWatchdog),   &podBase_t::handleMessage<agile::statusPod_t>);

    if(argc < 2)
    {
        printf("Please provide information on what channels to fake!\n");
        return -1;
    };

//printf("argc: %d\n",argc);
    for(int argci = 1; argci < argc; argci++)
    {
        //printf("check arg %d\n",argci);

        if(strcmp(argv[argci], "examplePod") == 0)
        {
            printf("Simulate for examplePOD!\n");
            podWorker.isExamplePod = true;
            podWorker.isImuAcquisitionStatus = true;
            podWorker.isImuAcquisitionImuRaw = true;
            podWorker.isStateEstimatorOrientV1 = true;
            podWorker.isDetectorVis = true;
            podWorker.isControllerPDPose = true;
            podWorker.isControllerSOCPose = true;
            podWorker.isControllerSOCOrient = true;
            break;
        }
        else if(strcmp(argv[argci], "imuAcquisitionStatus") == 0)
        {
            printf("Simulate for imuAcquisitionStatus!\n");
            podWorker.isImuAcquisitionStatus = true;

        }
        else if(strcmp(argv[argci], "imuAcquisitionImuRaw") == 0)
        {
            printf("Simulate for imuAcquisitionImuRaw!\n");
            podWorker.isImuAcquisitionImuRaw = true;

        }
        else if(strcmp(argv[argci], "stateEstimatorOrientV1") == 0)
        {
            printf("Simulate for stateEstimatorOrientV1!\n");
            podWorker.isStateEstimatorOrientV1 = true;

        }
        else if(strcmp(argv[argci], "detectorVis") == 0)
        {
            printf("Simulate for detectorVis!\n");
            podWorker.isDetectorVis = true;

        }
        else if(strcmp(argv[argci], "controllerPDPose") == 0)
        {
            printf("Simulate for controllerPDPose!\n");
            podWorker.isControllerPDPose = true;

        }
        else if(strcmp(argv[argci], "controllerSOCPose") == 0)
        {
            printf("Simulate for controllerSOCPose!\n");
            podWorker.isControllerSOCPose = true;

        }
        else if(strcmp(argv[argci], "controllerSOCOrient") == 0)
        {
            printf("Simulate for controllerSOCOrient!\n");
            podWorker.isControllerSOCOrient = true;

        }
    };


    if(podWorker.isExamplePod == false)
    {
        //dont simulate all channels (e.g. watchdog actually exists and does not have to be faked, so wait for it!)
        printf("Waiting for watchdog to be POD_OK...\n");


        while(podWorker.statusWatchdog.status != POD_OK)
        {
            podWorker.lcm.handle();
        }
        printf("Watchdog POD_OK! Continuing...\n");
    }



    podWorker.initComputationInterval();


    // Create a function call
    guint timer = g_timeout_add(podWorker.callInterval, podBase_t::gtimerfuncComputations, (gpointer)&podWorker);

    // Create the mainloop
    GMainLoop* mainloop = g_main_loop_new(NULL, TRUE);
    g_main_loop_run(mainloop);

    return 0;
}







