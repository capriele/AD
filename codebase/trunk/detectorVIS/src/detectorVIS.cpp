#include "detectorVIS.hpp"

using namespace std;

/* TODO
*/
/*
Implementation of loop function for computations in this specific POD
*/

gboolean podBase_t::gtimerfuncComputations(gpointer data)
{

    /* General Infrastructure (keep this infrastructure!) */
    detectorVIS_t* podWorker = reinterpret_cast<detectorVIS_t*>(data);
    std::lock_guard<std::mutex> guard(podMutex);

    /*--------*/


    /* Computations */

    // Set time stamps
    podWorker->features.timestampJetson = GetTimeStamp();
    podWorker->features.timestampCamera = 0; // @TODO: figure out how to get timestamp from camera

    // Retrieve image
    // cv::Mat image;
    // cv::Mat image_gray;

    // podWorker->m_cap >> image;
    // cv::cvtColor(image, image_gray, CV_BGR2GRAY);

    // // Detect april tags
    // vector<AprilTags::TagDetection> detections = podWorker->m_tagDetector->extractTags(image_gray);

    // // Get relative positions
    // podWorker->features.numFeat = detections.size();
    // Eigen::Matrix3d relRot;	// @TODO should these be declared here?
    // Eigen::Vector3d relPos;
    // double x,y,z,yaw,pitch,roll;
    // for (int i=0; i<detections.size(); i++) {
    // 	// Get position and location of tags in quadcopter frame, place in features LCM message
    // 	detections[i].getRelativeTranslationRotation(APRIL_TAG_SIZE, M_FX, M_FY, M_PX, M_PY, relPos, relRot);
    // 	wRo_to_euler(relRot, yaw, pitch, roll);
    // 	podWorker->features.featLoc[i][0] = relPos[0];
    // 	podWorker->features.featLoc[i][1] = relPos[1];
    // 	podWorker->features.featLoc[i][2] = relPos[2];
    // 	podWorker->features.featDirEuler[i][0] = yaw;
    // 	podWorker->features.featDirEuler[i][1] = pitch;
    // 	podWorker->features.featDirEuler[i][2] = roll;
    // 	for (int j=0; 9; j++) {
    // 	    // Uncertainty 0 for now
    // 	    podWorker->features.featLocVar[i][j] = 0;
    // 	    podWorker->features.featDirEulerVar[i][j] = 0;
    // 	}
    // }	

    // Get uncertainty
    // @TODO: Uncertainty figures for april tag detections

    /*---------*/


    /* Publishing computation result, in this case the feature locations */

    // statusDrone - publish
    podWorker->lcm.publish("features", &podWorker->features); 	//choose channelName here!

    /*---------*/



    /*General Infrastructure (keep this infrastructure!)*/
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

    /*General Infrastructure (keep this infrastructure!)*/
    detectorVIS_t* podWorker = reinterpret_cast<detectorVIS_t*>(data);
    messageStatus_t messageStatus = podWorker->checkMessagesUptodate();
    std::lock_guard<std::mutex> guard(podMutex);
    /*---------*/

    /*Computation statusPOD*/

    if(podWorker->computationInterval > MAXPODDELAY_X * podWorker->callInterval * MS2US)
    {
        printf("%s: delay in computation, dt=% " PRId64 "us at t=%" PRId64 "!\n", podWorker->podName.c_str(), podWorker->computationInterval,GetTimeStamp());
        podWorker->statusPod.status = POD_FATAL;
    }
    else 
    {

	if(messageStatus == MSGS_LATE)
    	{
		podWorker->statusPod.status = POD_CRITICAL;
	}
	else if(messageStatus == MSGS_DEAD)
	{
		podWorker->statusPod.status = POD_FATAL;
	}
	else
	{
		podWorker->statusPod.status = POD_OK;
	};

    };



    /*---------*/

    /*Publishing statusPOD (keep this infrastructure!)*/
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

    /* General Infrastructure: setup (keep this infrastructure!)  */

    // 1) Create the app
    detectorVIS_t podWorker = detectorVIS_t("detectorVIS", CALLINTERVAL_DETECTORVIS); 	//provide your PODname here!

    // 2) Create LCM
    if(!podWorker.lcm.good())
        return 1;

    // 3) Subscribe this POD to channels
    // No channels to subscribe to for feature detection
    /*---------*/




    /*  POD-specific init procedures  */
    // Update and publish status of detectorVIS (keep this infrastructure!)
    // printf("Initializing POD...\n");
    // podWorker.publishStatus(POD_INITING);

    // //Initialization stuff
    // podWorker.parseOptions(argc, argv); // parse options, i.e. get environment we're in
    // initEnvironment(podWorker.environment, podWorker.m_tagCodes); // Initialize environment parameters (i.e. where are what tags)

    // // Initialize detector and video
    // podWorker.m_tagDetector = new AprilTags::TagDetector(podWorker.m_tagCodes);
    
    // podWorker.m_cap = cv::VideoCapture(podWorker.m_deviceId);
    // if (!podWorker.m_cap.isOpened()) {
    // 	printf("ERROR: Can't find video device...\n");
    // 	return EXIT_FAILURE;
    // }
    // podWorker.m_cap.set(CV_CAP_PROP_FRAME_WIDTH, VIDEO_FRAME_WIDTH);
    // podWorker.m_cap.set(CV_CAP_PROP_FRAME_HEIGHT, VIDEO_FRAME_HEIGHT);
    
    // printf("Initializing POD... DONE\n");
    // /*---------*/



    /*  General Infrastructure: loops (keep this infrastructure!)  */

    //  POD waits for watchdogPOD to be up and running (POD_OK)  before continues

    printf("Waiting for watchdog to be POD_OK...\n");
    while(podWorker.statusWatchdog.status != POD_OK)
    {
        podWorker.lcm.handle();
    }
    printf("Watchdog POD_OK! Continuing...\n");
    podWorker.initComputationInterval();

    //Create message listener thread to listen to subscribed channels and stored received messages
    std::thread listenerThread(listen, &podWorker);

    // Create mainloop, add loop functions, run
    GMainLoop* mainloop = g_main_loop_new(NULL, FALSE);

    guint timerComputations = g_timeout_add(podWorker.callInterval, podBase_t::gtimerfuncComputations, (gpointer)&podWorker);
    guint timerStatusPod =    g_timeout_add(podWorker.callInterval, podBase_t::gtimerfuncStatusPod, (gpointer)&podWorker);

    g_main_loop_run(mainloop);

    /*---------*/

    return 0;
}







