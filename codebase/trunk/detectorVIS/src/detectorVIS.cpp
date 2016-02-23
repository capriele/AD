#include "detectorVIS.hpp"

using namespace std;

/* TODO
- april tag detection and publishing
- camera initialization
- sub function locations??
- parameters file
- update header file with needed info
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
    podWorker->featuers.timestampCamera = 0; // @TODO: figure out how to get timestamp from camera

    // Retrieve image

    // Detect april tags

    // Get relative positions

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
    detectorVIS_t podWorker = detectorVIS_t("detectorVIS", CALLINTERVAL_DETECTORVISPOD); 	//provide your PODname here!

    // 2) Create LCM
    if(!podWorker.lcm.good())
        return 1;

    // 3) Subscribe this POD to channels
    // No channels to subscribe to for feature detection
    /*---------*/




    /*  POD-specific init procedures  */
    // Update and publish status of detectorVIS (keep this infrastructure!)
    printf("Initializing POD...\n");
    podWorker.publishStatus(POD_INITING);

    //Initialization stuff
    podWorker.parseOptions(argc, argv); // parse options, i.e. get environment we're in
    initEnvironment(environment, m_tagCodes); // Initialize environment parameters (i.e. where are what tags)

    // Initialize detector and video
    m_tagDetector = new AprilTags::TagDetector(m_tagCodes);
    
    m_cap = cv::VideoCapture(m_deviceId);
    if (!m_cap.isOpened()) {
      
    }
    printf("Initializing POD... DONE\n");
    /*---------*/



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







