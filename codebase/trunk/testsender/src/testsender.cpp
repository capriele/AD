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



class Testpod_t : public podBase_t {
  
public:
  int value;

  bool isExamplePod, isImuAcquisitionStatus, isImuAcquisitionImuRaw, isDetectorVis, isStateEstimatorOrientV1, isControllerPDPose, isControllerSOCPose, isControllerSOCOrient;

  agile::controlMode_t controlMode;

  //constructor
  Testpod_t (string podName, int64_t callInterval) : podBase_t(podName,callInterval)
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
    
};


gboolean gtimerfunc (gpointer data) {

  Testpod_t* podWorker = (Testpod_t*) data;

 //publish a statusPOD message
  agile::statusPod_t statusPod;

  statusPod.timestampJetson = GetTimeStamp();
  statusPod.status = POD_OK;
  statusPod.podName = "Testsender";

  podWorker->lcm.publish ("statusTestsender", &statusPod);

  
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
if (podWorker->isImuAcquisitionStatus)
{  
  podWorker->lcm.publish ("statusImuAcquisition", &statusPod);
}

if (podWorker->isImuAcquisitionImuRaw)
{
  podWorker->lcm.publish ("imuRaw", &imuRaw);
}


if (podWorker->isDetectorVis)
{
  podWorker->lcm.publish ("statusDetectorVis", &statusPod);
}

if (podWorker->isStateEstimatorOrientV1)
{
 podWorker->lcm.publish ("stateVariancesOrientV1", &stateVariances);
 podWorker->lcm.publish ("statusStateEstimatorOrientV1", &statusPod);
}

if (podWorker->isControllerPDPose)
{
  podWorker->lcm.publish ("statusControllerPDPose", &statusPod);
}

if (podWorker->isControllerSOCOrient)
{
  podWorker->lcm.publish ("statusControllerSOCOrient", &statusPod);
}


if (podWorker->isControllerSOCPose)
{
  podWorker->lcm.publish ("statusControllerSOCPose", &statusPod);
}

 /*for examplePOD: */
  if (podWorker->isExamplePod)
	{
	podWorker->lcm.publish ("statusRemoteController", &statusPod);
	podWorker->lcm.publish ("statusControllerPDOrient", &statusPod);
	podWorker->lcm.publish ("statusMotorCommander", &statusPod);
	podWorker->lcm.publish ("statusWatchdog", &statusPod);
	podWorker->lcm.publish ("controlMode", &(podWorker->controlMode));	

  	agile::pose_t pose_data;

	pose_data.timestampJetson = GetTimeStamp();
	pose_data.position[0] = 0.0;
  	pose_data.position[1] = 0.0;
  	pose_data.position[2] = 0.25*sin((double)(podWorker->value++)/10);

	  pose_data.orient[0] = 1.0;//0.923;//1.0;
	  pose_data.orient[1] = 0.0;
	  pose_data.orient[2] = 0.0;//0.0;
	  pose_data.orient[3] = 0.0;//0.38268;

  	podWorker->lcm.publish ("testdata", &pose_data);
	}

  return TRUE;
}


int main (int argc, char** argv) {


  // Create the app
  Testpod_t podWorker = Testpod_t("testsender",CALLINTERVAL_TESTSENDER); 


  // Create LCM
  if (!podWorker.lcm.good()) 
    return 1;

  podWorker.subscribe("statusWatchdog",  CALLINTERVAL_WATCHDOG, &(podWorker.statusWatchdog),   &podBase_t::handleMessage<agile::statusPod_t>);

  if (argc<2) {printf("Please provide information on what channels to fake!\n"); return -1;};
  
 //printf("argc: %d\n",argc);
  for (int argci = 1; argci< argc; argci++)
	{
	//printf("check arg %d\n",argci);

	if (strcmp(argv[argci], "examplePod") == 0)
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
	else if (strcmp(argv[argci], "imuAcquisitionStatus") == 0)
		{
		printf("Simulate for imuAcquisitionStatus!\n");
		podWorker.isImuAcquisitionStatus = true;
		
		}
	else if (strcmp(argv[argci], "imuAcquisitionImuRaw") == 0)
		{
		printf("Simulate for imuAcquisitionImuRaw!\n");
		podWorker.isImuAcquisitionImuRaw = true;
		
		}
	else if (strcmp(argv[argci], "stateEstimatorOrientV1") == 0)
		{
		printf("Simulate for stateEstimatorOrientV1!\n");
		podWorker.isStateEstimatorOrientV1 = true;
		
		}
	else if (strcmp(argv[argci], "detectorVis") == 0)
		{
		printf("Simulate for detectorVis!\n");
		podWorker.isDetectorVis = true;
		
		}
	else if (strcmp(argv[argci], "controllerPDPose") == 0)
		{
		printf("Simulate for controllerPDPose!\n");
		podWorker.isControllerPDPose = true;
		
		}
	else if (strcmp(argv[argci], "controllerSOCPose") == 0)
		{
		printf("Simulate for controllerSOCPose!\n");
		podWorker.isControllerSOCPose = true;
		
		}
	else if (strcmp(argv[argci], "controllerSOCOrient") == 0)
		{
		printf("Simulate for controllerSOCOrient!\n");
		podWorker.isControllerSOCOrient = true;
		
		}
};


  if (podWorker.isExamplePod == false)
	{
	//dont simulate all channels (e.g. watchdog actually exists and does not have to be faked, so wait for it!)
		printf("Waiting for watchdog to be POD_OK...\n");


  		while (podWorker.statusWatchdog.status!=POD_OK)
		{
		podWorker.lcm.handle();
		}
  		printf("Watchdog POD_OK! Continuing...\n");
	}

  
  
  podWorker.initComputationInterval();
	

  // Create a function call 
  guint timer = g_timeout_add (podWorker.callInterval, gtimerfunc, (gpointer)&podWorker);
  
  // Create the mainloop
  GMainLoop* mainloop = g_main_loop_new (NULL, TRUE);
  g_main_loop_run (mainloop);

  return 0;
}







