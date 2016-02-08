#include "controllerSOCOrient.hpp"

using namespace std;





/*
Implementation of loop function for computations in this specific POD
*/

gboolean podBase_t::gtimerfuncComputations(gpointer data)
{

    /* General Infrastructure (maintain this structure!) */
    controllerSOCOrient_t* podWorker = reinterpret_cast<controllerSOCOrient_t*>(data);
    std::lock_guard<std::mutex> guard(podMutex);
    /*--------*/

    /* Computations */

    //compute euler angle estimate from quaternion estimates
    double yaw_hat, pitch_hat, roll_hat;
    quat2Euler(podWorker->stateVariances.orient, &(yaw_hat), &(pitch_hat), &(roll_hat));

    double torquesRefs[3]; //output of Alex orientation-controller: torques about body-axes x,y,z (see Dropbox (MIT)\AgileDrones\writeups_docs\architecture\controlarchitecture\orientation_model/quadrotordynamics.pdf)

    double orientState[6]; //orientation-related state: euler angles yaw pitch roll + angular rates about body-axes x,y,z
    orientState[0] = yaw_hat; //@TODO replace this by error (angle_hat - angle_ref) if we want to enable the user to control the angles for non-zero references
    orientState[1] = pitch_hat;
    orientState[2] = roll_hat;
    orientState[3] = podWorker->stateVariances.veloOrientBody[0];
    orientState[4] = podWorker->stateVariances.veloOrientBody[1];
    orientState[5] = podWorker->stateVariances.veloOrientBody[2];

    //get optimal torque references from Alex controller
    getResultsControllerSOCOrient(orientState, podWorker.controllerLookup, torquesRefs);

    //compute total thrust as function of user-requested thrust
    double totalThrust = - QUADMASS * GRAVITY - podWorker->powerAdjust.tBiasPDO;

    //convert [totalthrust;body torque references] into the four motor speeds
    podWorker->motorsWsRef.wsRef[0] = sqrt(-THRUST2OMEGA2 * (ATOTALTHRUST * totalThrust + ATAUYAW * torquesRefs[2] - ATAUPR * torquesRefs[1] - ATAUPR * torquesRefs[0));
                                           podWorker->motorsWsRef.wsRef[1] = sqrt(-THRUST2OMEGA2 * (ATOTALTHRUST * totalThrust - ATAUYAW * torquesRefs[2] - ATAUPR * torquesRefs[1] + ATAUPR * torquesRefs[0]));
                                           podWorker->motorsWsRef.wsRef[2] = sqrt(-THRUST2OMEGA2 * (ATOTALTHRUST * totalThrust + ATAUYAW * torquesRefs[2] + ATAUPR * torquesRefs[1] + ATAUPR * torquesRefs[0]));
                                           podWorker->motorsWsRef.wsRef[3] = sqrt(-THRUST2OMEGA2 * (ATOTALTHRUST * totalThrust - ATAUYAW * torquesRefs[2] + ATAUPR * torquesRefs[1] - ATAUPR * torquesRefs[0]));


                                           podWorker->motorsWsRef.timestampJetson = GetTimeStamp();

                                           /*---------*/


                                           /* Publishing */

                                           // statusDrone - publish
                                           podWorker->lcm.publish("motorsWsRefSOCOrient", &podWorker->motorsWsRef); 	//choose channelName here!

                                           /*---------*/



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
    controllerSOCOrient_t* podWorker = reinterpret_cast<controllerSOCOrient_t*>(data);
    /*---------*/

    /*Computation statusPOD*/
    if(podWorker->computationInterval > MAXPODDELAY_X * podWorker->callInterval * MS2US)
    {
        printf("controllerSOCOrient: delay occured of % " PRId64 "us!\n", podWorker->computationInterval);
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

    // 1) Create the app
    controllerSOCOrient_t podWorker = controllerSOCOrient_t("controllerSOCOrient", CALLINTERVAL_controllerSOCOrient); 	//provide your PODname here!
    podWorker.controllerFilePath = "controllerSOCOrient.bin";

    if(argc < 2)
    {
        printf("Please provide estimationChannel that controller will use!\n");
        return EXIT_FAILURE;
    };

    podWorker.stateVariancesChannel = argv[1];

    // 2) Create LCM
    if(!podWorker.lcm.good())
        return 1;

    // 3) Subscribe this POD to channels

    //channel that provides state-estimates
    if(podWorker.stateVariancesChannel == "stateVariancesOrientV1")
        podWorker.subscribe(podWorker.stateVariancesChannel, CALLINTERVAL_STATEESTIMATORORIENTV1, 	 &(podWorker.stateVariances), 	&podBase_t::handleMessage<agile::stateVariances_t>);
    else if(podWorker.stateVariancesChannel == "stateVariancesOrientCF")
        podWorker.subscribe(podWorker.stateVariancesChannel, CALLINTERVAL_STATEESTIMATORORIENTCF, 	 &(podWorker.stateVariances), 	&podBase_t::handleMessage<agile::stateVariances_t>);
    else podWorker.subscribe(podWorker.stateVariancesChannel, CALLINTERVAL_SIMULATOR, 	 &(podWorker.stateVariances), 	&podBase_t::handleMessage<agile::stateVariances_t>);

    //other channels
    podWorker.subscribe("poseRef",  CALLINTERVAL_REMOTECONTROLLER, &(podWorker.poseRef), &podBase_t::handleMessage<agile::poseRef_t>);
    podWorker.subscribe("powerAdjust",  CALLINTERVAL_REMOTECONTROLLER, &(podWorker.powerAdjust), &podBase_t::handleMessage<agile::powerAdjust_t>);

    /*---------*/




    /*  POD-specific init procedures  */
    // Update and publish status of controllerSOCOrient_t (this POD is the first to start)
    podWorker.publishStatus(POD_INITING);

    //Load controller lookup

    if((podWorker.controllerFileHandle = fopen(podWorker.controllerFilePath, "rb")) == NULL)
    {
        printf("ERROR opening controller file!\n");
        return 1;
    }
    else fread(podWorker.controllerLookup, sizeof(controllerLookup_t), 1, podWorker.controllerFileHandle);

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
