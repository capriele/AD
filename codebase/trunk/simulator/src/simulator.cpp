

#include "simulator.hpp"


static RT_MODEL_SimulatorDrone_T SimulatorDrone_M_;
static RT_MODEL_SimulatorDrone_T *const SimulatorDrone_M = &SimulatorDrone_M_;/* Real-time model */
static P_SimulatorDrone_T SimulatorDrone_P =
{
    {
        2.1959176351571146e-6,
        1750.0,
        0.00026673,
        1009.0,
        -200.2,
        1900.0,
        0.06,

        {
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0165195073635001, 0.0152648883285633,
            0.0215786550496705, 0.000652733165165932, 0.000721701528439517,
            0.000690781425279554
        },

        { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.05, 0.05, 0.05, 1.0, 1.0, 1.0 },
        1.0,

        { 0.0, 0.0, -9.81, 0.0, 0.0, 0.0, 101270.95 },

        { 1.0, 1.0, 1.0 },

        { 1.0, 1.0, 1.0 },
        1.225,
        12.01725,

        { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 },
        0.44,

        {
            { 3.5, 70.0 },

            { -99.0, 0.0, 0.0, -9.0 },
            0.0
        },
        0.05,
        0.005,
        0.1,
        5.5,
        2.5,
        0.0,
        3.0,
        1.0,
        0.1,
        3.0,
        1.0,
        1.0,
        -1.1,
        -99.0,
        -9.0,
        10.0,
        0.05,
        0.0,
        0.0,
        -30.0,
        20.0,
        1.0
    },                                   /* Variable: quadEDT
                                        * Referenced by:
                                        *   '<S1>/sensordataCalib'
                                        *   '<S3>/Constant'
                                        *   '<S3>/Constant1'
                                        *   '<S3>/Constant2'
                                        *   '<S3>/omegaSaturation'
                                        *   '<S3>/pwmSaturation'
                                        *   '<S5>/accel_gain'
                                        *   '<S5>/altToprs_gain'
                                        *   '<S5>/gyro_pq_gain'
                                        *   '<S5>/gyro_r_Gain'
                                        *   '<S5>/Saturation'
                                        *   '<S6>/VelocityToOpticalFlow_Gain'
                                        *   '<S11>/Constant15'
                                        *   '<S7>/noiseWeights'
                                        */

    {
        4.0,
        9.81,
        1.2041,
        1.5e-5,
        0.949,

        { 0.0057846, 0.0, 0.0, 0.0, 0.0058361, 0.0, 0.0, 0.0, 0.010355 },
        -0.012,
        0.165,
        2.0,
        0.076,
        0.016,
        0.0,
        0.0035,
        0.007,
        0.0074,
        4.117715e-6,
        9.583e-8,
        0.0015588090000000002,
        8.42709e-6,
        0.0174,
        0.0016229639552374536,
        0.1340252152352803,
        0.17453292519943295,
        0.52359877559829882,
        -0.34906585039886584,
        0.26179938779914946,
        10000.0,
        5.5,
        0.018145839167134643,
        0.83898068222857458,
        2.1959176351571146e-6,
        1.5566438675879322e-8,
        0.0
    },                                   /* Variable: quad
                                        * Referenced by:
                                        *   '<S11>/Constant'
                                        *   '<S11>/Constant1'
                                        *   '<S11>/Constant10'
                                        *   '<S11>/Constant11'
                                        *   '<S11>/Constant12'
                                        *   '<S11>/Constant13'
                                        *   '<S11>/Constant2'
                                        *   '<S11>/Constant3'
                                        *   '<S11>/Constant4'
                                        *   '<S11>/Constant5'
                                        *   '<S11>/Constant6'
                                        *   '<S11>/Constant7'
                                        *   '<S11>/Constant8'
                                        *   '<S11>/Constant9'
                                        */
    0.0,                                 /* Mask Parameter: CompareToConstant1_const
                                        * Referenced by: '<S14>/Constant'
                                        */
    0.0,                                 /* Mask Parameter: CompareToConstant_const
                                        * Referenced by: '<S13>/Constant'
                                        */
    0.0,                                 /* Mask Parameter: GaussianNoiseGenerator_m
                                        * Referenced by: '<S10>/Constant'
                                        */
    0.0,                                 /* Expression: 0
                                        * Referenced by: '<S11>/Delay3'
                                        */

    /*  Expression: [0 0 0.0 0 0 0 0 0 0 0 0 0]
     * Referenced by: '<S11>/Delay1'
     */
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },

    /*  Expression: [0 0 0.0 0 0 0 0 0 0 0 0 0]
     * Referenced by: '<S11>/Constant14'
     */
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
    0.005,                               /* Computed Parameter: IntPosestates_gainval
                                        * Referenced by: '<S11>/IntPosestates'
                                        */
    0.005,                               /* Computed Parameter: IntMotors_gainval
                                        * Referenced by: '<S11>/IntMotors'
                                        */

    /*  Expression: [10 -10 10 -10]
     * Referenced by: '<S11>/IntMotors'
     */
    { 10.0, -10.0, 10.0, -10.0 },
    0.5,                                 /* Expression: 0.5
                                        * Referenced by: '<S4>/1//2'
                                        */
    0.0,                                 /* Expression: 0
                                        * Referenced by: '<S6>/Delay3'
                                        */
    0.0,                                 /* Expression: 0
                                        * Referenced by: '<S5>/zeroconst'
                                        */

    /*  Expression: [0 0 quad.g 0 0 0 0 0]
     * Referenced by: '<S5>/Assumes takeoff was level'
     */
    { 0.0, 0.0, 9.81, 0.0, 0.0, 0.0, 0.0, 0.0 },

    /*  Expression: [quadEDT.sensordataCalib(1:6),0,quadEDT.sensordataCalib(7)]
     * Referenced by: '<S5>/Delay2'
     */
    { 0.0, 0.0, -9.81, 0.0, 0.0, 0.0, 0.0, 101270.95 },
    0.0,                                 /* Expression: 0
                                        * Referenced by: '<S10>/Random Number'
                                        */
    1.0,                                 /* Computed Parameter: RandomNumber_StdDev
                                        * Referenced by: '<S10>/Random Number'
                                        */

    /*  Expression: s
     * Referenced by: '<S10>/Random Number'
     */
    { 41.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0 },

    /*  Expression: d
     * Referenced by: '<S10>/Matrix Gain'
     */
    {
        1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0165195073635001, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0152648883285633, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0215786550496705, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.000652733165165932, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.000721701528439517, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.000690781425279554
    },
    0.0,                                 /* Expression: 0
                                        * Referenced by: '<S10>/Buffer'
                                        */
    -1.0,                                /* Expression: -1
                                        * Referenced by: '<S5>/alt_Gain'
                                        */
    0.0,                                 /* Expression: inf
                                        * Referenced by: '<S5>/Saturation'
                                        */

    /*  Expression: [0 0 -quad.g]
     * Referenced by: '<S5>/gravity'
     */
    { 0.0, 0.0, -9.81 },

    /*  Expression: 0.8*[-1,1]
     * Referenced by: '<S6>/Gain1'
     */
    { -0.8, 0.8 },
    -1.0,                                /* Expression: -1
                                        * Referenced by: '<S6>/Gain2'
                                        */
    100.0,                               /* Expression: 100
                                        * Referenced by: '<S3>/omegaSaturation'
                                        */

    /*  Expression: [1 -1 1 -1]
     * Referenced by: '<S3>/Directions'
     */
    { 1.0, -1.0, 1.0, -1.0 },
    1U,                                  /* Computed Parameter: Delay3_DelayLength
                                        * Referenced by: '<S11>/Delay3'
                                        */
    1U,                                  /* Computed Parameter: Delay1_DelayLength
                                        * Referenced by: '<S11>/Delay1'
                                        */
    1U,                                  /* Computed Parameter: Delay3_DelayLength_p
                                        * Referenced by: '<S6>/Delay3'
                                        */
    1U                                   /* Expression: quadEDT.sensordelay
                                        * Referenced by: '<S5>/Delay2'
                                        */
};                                     /* Modifiable parameters */

static B_SimulatorDrone_T SimulatorDrone_B;/* Observable signals */
static DW_SimulatorDrone_T SimulatorDrone_DW;/* Observable states */

/* '<Root>/motorsPWM_datin' */
static real_T SimulatorDrone_U_motorsPWM_datin[4];

/* '<Root>/X' */
static real_T SimulatorDrone_Y_X;

/* '<Root>/Y' */
static real_T SimulatorDrone_Y_Y;

/* '<Root>/Z' */
static real_T SimulatorDrone_Y_Z;

/* '<Root>/dx' */
static real_T SimulatorDrone_Y_dx;

/* '<Root>/dy' */
static real_T SimulatorDrone_Y_dy;

/* '<Root>/dz' */
static real_T SimulatorDrone_Y_dz;

/* '<Root>/yaw' */
static real_T SimulatorDrone_Y_yaw;

/* '<Root>/pitch' */
static real_T SimulatorDrone_Y_pitch;

/* '<Root>/roll' */
static real_T SimulatorDrone_Y_roll;

/* '<Root>/quaternions_datout' */
static real_T SimulatorDrone_Y_quaternions_datout[4];

/* '<Root>/p' */
static real_T SimulatorDrone_Y_p;

/* '<Root>/q' */
static real_T SimulatorDrone_Y_q;

/* '<Root>/r' */
static real_T SimulatorDrone_Y_r;

/* '<Root>/opticalFlow_datout' */
static real_T SimulatorDrone_Y_opticalFlow_datout[3];

/* '<Root>/sensordataCalib_daout' */
static real_T SimulatorDrone_Y_sensordataCalib_daout[7];

/* '<Root>/ddx' */
static real_T SimulatorDrone_Y_ddx;

/* '<Root>/ddy' */
static real_T SimulatorDrone_Y_ddy;

/* '<Root>/ddz' */
static real_T SimulatorDrone_Y_ddz;

/* '<Root>/pb' */
static real_T SimulatorDrone_Y_pb;

/* '<Root>/qb' */
static real_T SimulatorDrone_Y_qb;

/* '<Root>/rb' */
static real_T SimulatorDrone_Y_rb;

/* '<Root>/altitude_sonar' */
static real_T SimulatorDrone_Y_altitude_sonar;

/* '<Root>/prs' */
static real_T SimulatorDrone_Y_prs;
/*
 * Associating rt_OneStep with a real-time clock or interrupt service routine
 * is what makes the generated code "real-time".  The function rt_OneStep is
 * always associated with the base rate of the model.  Subrates are managed
 * by the base rate from inside the generated code.  Enabling/disabling
 * interrupts and floating point context switches are target specific.  This
 * example code indicates where these should take place relative to executing
 * the generated code step function.  Overrun behavior should be tailored to
 * your application needs.  This example simply sets an error status in the
 * real-time model and returns from rt_OneStep.
 */
void rt_OneStep(RT_MODEL_SimulatorDrone_T *const SimulatorDrone_M);
void rt_OneStep(RT_MODEL_SimulatorDrone_T *const SimulatorDrone_M)
{
    static boolean_T OverrunFlag = false;

    /* Disable interrupts here */

    /* Check for overrun */
    if(OverrunFlag)
    {
        rtmSetErrorStatus(SimulatorDrone_M, "Overrun");
        return;
    }

    OverrunFlag = true;

    /* Save FPU context here (if necessary) */
    /* Re-enable timer or interrupt here */
    /* Set model inputs here */

    /* Step the model for base rate */
    SimulatorDrone_step(SimulatorDrone_M, SimulatorDrone_U_motorsPWM_datin,
                        &SimulatorDrone_Y_X, &SimulatorDrone_Y_Y,
                        &SimulatorDrone_Y_Z, &SimulatorDrone_Y_dx,
                        &SimulatorDrone_Y_dy, &SimulatorDrone_Y_dz,
                        &SimulatorDrone_Y_yaw, &SimulatorDrone_Y_pitch,
                        &SimulatorDrone_Y_roll,
                        SimulatorDrone_Y_quaternions_datout, &SimulatorDrone_Y_p,
                        &SimulatorDrone_Y_q, &SimulatorDrone_Y_r,
                        SimulatorDrone_Y_opticalFlow_datout,
                        SimulatorDrone_Y_sensordataCalib_daout,
                        &SimulatorDrone_Y_ddx, &SimulatorDrone_Y_ddy,
                        &SimulatorDrone_Y_ddz, &SimulatorDrone_Y_pb,
                        &SimulatorDrone_Y_qb, &SimulatorDrone_Y_rb,
                        &SimulatorDrone_Y_altitude_sonar, &SimulatorDrone_Y_prs);

    /* Get model outputs here */

    /* Indicate task complete */
    OverrunFlag = false;

    /* Disable interrupts here */
    /* Restore FPU context here (if necessary) */
    /* Enable interrupts here */
}



using namespace std;




/*
Implementation of loop function for computations in this specific POD
*/

gboolean podBase_t::gtimerfuncComputations(gpointer data)
{

    /* General Infrastructure (keep this infrastructure!) */
    simulator_t* podWorker = (simulator_t*) data;
    std::lock_guard<std::mutex> guard(podMutex);

    /*--------*/


    /* Computations */

    //Update inputs to simulink model

    SimulatorDrone_U_motorsPWM_datin[0] = (int)((podWorker->motorsPwms.pwms[0]) / PWMDISCRESC + 0.5) * PWMDISCRESC; //NOTE: +0.5 to make cast as semi-rounding procedure
    SimulatorDrone_U_motorsPWM_datin[1] = (int)((podWorker->motorsPwms.pwms[1]) / PWMDISCRESC + 0.5) * PWMDISCRESC;
    SimulatorDrone_U_motorsPWM_datin[2] = (int)((podWorker->motorsPwms.pwms[2]) / PWMDISCRESC + 0.5) * PWMDISCRESC;
    SimulatorDrone_U_motorsPWM_datin[3] = (int)((podWorker->motorsPwms.pwms[3]) / PWMDISCRESC + 0.5) * PWMDISCRESC;

    //printf("in %d, act int %f\n",podWorker->motorsPwms.pwms[0],SimulatorDrone_U_motorsPWM_datin[0]);


    if((rtmGetErrorStatus(SimulatorDrone_M) == (NULL)))
    {
        rt_OneStep(SimulatorDrone_M);
    }
    else
    {
        printf("ERROR: Error from simulink model!\n");
    }

    //Pack outputs from simulink model

    podWorker->stateVariancesSim.position[0] = SimulatorDrone_Y_X;
    podWorker->stateVariancesSim.position[1] = SimulatorDrone_Y_Y;
    podWorker->stateVariancesSim.position[2] = SimulatorDrone_Y_Z;

    podWorker->stateVariancesSim.veloPositionBody[0] = SimulatorDrone_Y_dx;
    podWorker->stateVariancesSim.veloPositionBody[1] = SimulatorDrone_Y_dy;
    podWorker->stateVariancesSim.veloPositionBody[2] = SimulatorDrone_Y_dz;

    podWorker->stateVariancesSim.orient[0] = SimulatorDrone_Y_quaternions_datout[0];
    podWorker->stateVariancesSim.orient[1] = SimulatorDrone_Y_quaternions_datout[1];
    podWorker->stateVariancesSim.orient[2] = SimulatorDrone_Y_quaternions_datout[2];
    podWorker->stateVariancesSim.orient[3] = SimulatorDrone_Y_quaternions_datout[3];

    podWorker->stateVariancesSim.veloOrientBody[0] = SimulatorDrone_Y_p;
    podWorker->stateVariancesSim.veloOrientBody[1] = SimulatorDrone_Y_q;
    podWorker->stateVariancesSim.veloOrientBody[2] = SimulatorDrone_Y_r;

    podWorker->stateVariancesSim.timestampJetson = GetTimeStamp();

    podWorker->imuRawSim.accel[0] = SimulatorDrone_Y_ddx;
    podWorker->imuRawSim.accel[1] = SimulatorDrone_Y_ddy;
    podWorker->imuRawSim.accel[2] = SimulatorDrone_Y_ddz;
    podWorker->imuRawSim.gyro[0] = SimulatorDrone_Y_pb;
    podWorker->imuRawSim.gyro[1] = SimulatorDrone_Y_qb;
    podWorker->imuRawSim.gyro[2] = SimulatorDrone_Y_rb;

    podWorker->imuRawSim.timestampJetson = GetTimeStamp();	//@TODO timestamp arduino

    //printf("%f %f %f\n",SimulatorDrone_Y_eulerangles_global_RS_datout[0],SimulatorDrone_Y_eulerangles_global_RS_datout[1],SimulatorDrone_Y_eulerangles_global_RS_datout[2]);
    podWorker->stateEulerSim.orientEuler[0] = SimulatorDrone_Y_yaw;
    podWorker->stateEulerSim.orientEuler[1] = SimulatorDrone_Y_pitch;
    podWorker->stateEulerSim.orientEuler[2] = SimulatorDrone_Y_roll;
    podWorker->stateEulerSim.timestampJetson = GetTimeStamp();




    /*---------*/


    /* Publishing computation result, in this case, the status update of drone status */

    // statusDrone - publish
    podWorker->lcm.publish("imuRawSim", &podWorker->imuRawSim);
    podWorker->lcm.publish("stateVariancesSim", &podWorker->stateVariancesSim);

    podWorker->lcm.publish("stateEulerSim", &podWorker->stateEulerSim);

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
    simulator_t* podWorker = (simulator_t*) data;
    /*---------*/

    /*Computation statusPOD*/
    /*--compute updated pod status--*/
    if(podWorker->computationInterval > MAXPODDELAY_X * podWorker->callInterval * MS2US)
    {
        printf("simulator: delay occured of % " PRId64 "us!\n", podWorker->computationInterval);
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
    simulator_t podWorker = simulator_t("simulator", CALLINTERVAL_SIMULATOR);

    // 2) Create LCM
    if(!podWorker.lcm.good())
        return 1;

    // 3) Subscribe this POD to channels
    podWorker.subscribe("motorsPwms", CALLINTERVAL_MOTORCOMMANDER, 	 &(podWorker.motorsPwms), 	&podBase_t::handleMessage<agile::motorsPwms_t>);
    //podWorker.subscribe("statusWatchdog",  CALLINTERVAL_WATCHDOG, &(podWorker.statusWatchdog),   &podBase_t::handleMessage<agile::statusPod_t>); // IMPORTANT -> POD is autosubscribed to this via constructor of Pod-base-class!






    /*---------*/




    /*  POD-specific init procedures  */
    // Update and publish status of simulator (keep this infrastructure!)
    printf("Initializing POD...\n");
    podWorker.publishStatus(POD_INITING);

    //Initialization stuff
    //...

    //@TODO can remove this as set from simulink?
    SimulatorDrone_U_motorsPWM_datin[0] = ZERORPMPWM;
    SimulatorDrone_U_motorsPWM_datin[1] = ZERORPMPWM;
    SimulatorDrone_U_motorsPWM_datin[2] = ZERORPMPWM;
    SimulatorDrone_U_motorsPWM_datin[3] = ZERORPMPWM;
    SimulatorDrone_Y_X = 0.0;
    SimulatorDrone_Y_Y = 0.0;
    SimulatorDrone_Y_Z = 0.0;
    SimulatorDrone_Y_dx = 0.0;
    SimulatorDrone_Y_dy = 0.0;
    SimulatorDrone_Y_dz = 0.0;
    SimulatorDrone_Y_yaw = 0.0;
    SimulatorDrone_Y_pitch = 0.0;
    SimulatorDrone_Y_roll = 0.0;
    SimulatorDrone_Y_quaternions_datout[0] = 1.0;
    SimulatorDrone_Y_quaternions_datout[1] = 0.0;
    SimulatorDrone_Y_quaternions_datout[2] = 0.0;
    SimulatorDrone_Y_quaternions_datout[3] = 0.0;

    SimulatorDrone_Y_p = 0.0;
    SimulatorDrone_Y_q = 0.0;
    SimulatorDrone_Y_r = 0.0;

    SimulatorDrone_Y_ddx = 0.0;
    SimulatorDrone_Y_ddy = 0.0;
    SimulatorDrone_Y_ddz = 0.0;

    SimulatorDrone_Y_pb = 0.0;
    SimulatorDrone_Y_qb = 0.0;
    SimulatorDrone_Y_rb = 0.0;



    /* Pack model data into RTM */
    SimulatorDrone_M->ModelData.defaultParam = &SimulatorDrone_P;
    SimulatorDrone_M->ModelData.blockIO = &SimulatorDrone_B;
    SimulatorDrone_M->ModelData.dwork = &SimulatorDrone_DW;

    /* Initialize model */
    SimulatorDrone_initialize(SimulatorDrone_M, SimulatorDrone_U_motorsPWM_datin,
                              &SimulatorDrone_Y_X, &SimulatorDrone_Y_Y, &SimulatorDrone_Y_Z,
                              &SimulatorDrone_Y_dx, &SimulatorDrone_Y_dy, &SimulatorDrone_Y_dz,
                              &SimulatorDrone_Y_yaw, &SimulatorDrone_Y_pitch, &SimulatorDrone_Y_roll,
                              SimulatorDrone_Y_quaternions_datout, &SimulatorDrone_Y_p,
                              &SimulatorDrone_Y_q, &SimulatorDrone_Y_r,
                              SimulatorDrone_Y_opticalFlow_datout, SimulatorDrone_Y_sensordataCalib_daout,
                              &SimulatorDrone_Y_ddx, &SimulatorDrone_Y_ddy, &SimulatorDrone_Y_ddz,
                              &SimulatorDrone_Y_pb, &SimulatorDrone_Y_qb, &SimulatorDrone_Y_rb,
                              &SimulatorDrone_Y_altitude_sonar, &SimulatorDrone_Y_prs);


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







