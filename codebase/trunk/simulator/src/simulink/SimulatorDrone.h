/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: SimulatorDrone.h
 *
 * Code generated for Simulink model 'SimulatorDrone'.
 *
 * Model version                  : 1.3303
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Fri Feb 26 16:52:15 2016
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: 32-bit Generic
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 *    3. ROM efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_SimulatorDrone_h_
#define RTW_HEADER_SimulatorDrone_h_
#include <math.h>
#include <string.h>
#ifndef SimulatorDrone_COMMON_INCLUDES_
# define SimulatorDrone_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* SimulatorDrone_COMMON_INCLUDES_ */

#include "SimulatorDrone_types.h"
#include "rtGetInf.h"
#include "rt_nonfinite.h"
#include "rt_defines.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetT
# define rtmGetT(rtm)                  (rtmGetTPtr((rtm))[0])
#endif

/* Block signals (auto storage) */
typedef struct {
  real_T IntPosestates[12];            /* '<S11>/IntPosestates' */
  real_T VelocityToOpticalFlow_Gain[3];/* '<S6>/VelocityToOpticalFlow_Gain' */
  real_T dx[16];                       /* '<S11>/MATLAB Function' */
  real_T y[3];                         /* '<S5>/trafo_Body2World_trans' */
} B_SimulatorDrone_T;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real_T Delay3_DSTATE[12];            /* '<S11>/Delay3' */
  real_T Delay1_DSTATE[12];            /* '<S11>/Delay1' */
  real_T IntPosestates_DSTATE[12];     /* '<S11>/IntPosestates' */
  real_T IntMotors_DSTATE[4];          /* '<S11>/IntMotors' */
  real_T simualterandomusbdelays_DSTATE[8];/* '<S5>/simualte random usbdelays' */
  real_T Delay1_DSTATE_j[4];           /* '<S1>/Delay1' */
  real_T simulatedsensorupdateratedelays[8];/* '<S5>/simulated sensorupdaterate delays' */
  real_T NextOutput[12];               /* '<S10>/Random Number' */
  real_T TimeStampA;                   /* '<S5>/Derivative' */
  real_T LastUAtTimeA[3];              /* '<S5>/Derivative' */
  real_T TimeStampB;                   /* '<S5>/Derivative' */
  real_T LastUAtTimeB[3];              /* '<S5>/Derivative' */
  uint32_T RandSeed[12];               /* '<S10>/Random Number' */
  int8_T IntPosestates_PrevResetState; /* '<S11>/IntPosestates' */
} DW_SimulatorDrone_T;

/* Parameters (auto storage) */
struct P_SimulatorDrone_T_ {
  struct_95VAVvBkBJA0GNbz7i38n quadEDT;/* Variable: quadEDT
                                        * Referenced by:
                                        *   '<S1>/sensordataCalib'
                                        *   '<S1>/Delay1'
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
  struct_KIBNvBSKeP9vvISqfeQYGC quad;  /* Variable: quad
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
  real_T CompareToConstant1_const;     /* Mask Parameter: CompareToConstant1_const
                                        * Referenced by: '<S14>/Constant'
                                        */
  real_T CompareToConstant_const;      /* Mask Parameter: CompareToConstant_const
                                        * Referenced by: '<S13>/Constant'
                                        */
  real_T GaussianNoiseGenerator_m;     /* Mask Parameter: GaussianNoiseGenerator_m
                                        * Referenced by: '<S10>/Constant'
                                        */
  real_T Delay3_InitialCondition;      /* Expression: 0
                                        * Referenced by: '<S11>/Delay3'
                                        */
  real_T Delay1_InitialCondition[12];  /* Expression: [0 0 0.0 0 0 0 0 0 0 0 0 0]
                                        * Referenced by: '<S11>/Delay1'
                                        */
  real_T Constant14_Value[12];         /* Expression: [0 0 0.0 0 0 0 0 0 0 0 0 0]
                                        * Referenced by: '<S11>/Constant14'
                                        */
  real_T IntPosestates_gainval;        /* Computed Parameter: IntPosestates_gainval
                                        * Referenced by: '<S11>/IntPosestates'
                                        */
  real_T IntMotors_gainval;            /* Computed Parameter: IntMotors_gainval
                                        * Referenced by: '<S11>/IntMotors'
                                        */
  real_T IntMotors_IC[4];              /* Expression: [10 -10 10 -10]
                                        * Referenced by: '<S11>/IntMotors'
                                        */
  real_T u_Gain;                       /* Expression: 0.5
                                        * Referenced by: '<S4>/1//2'
                                        */
  real_T Gain1_Gain[2];                /* Expression: 0.8*[-1,1]
                                        * Referenced by: '<S6>/Gain1'
                                        */
  real_T Gain2_Gain;                   /* Expression: -1
                                        * Referenced by: '<S6>/Gain2'
                                        */
  real_T zeroconst_Value;              /* Expression: 0
                                        * Referenced by: '<S5>/zeroconst'
                                        */
  real_T Assumestakeoffwaslevel_Bias[8];/* Expression: [0 0 quad.g 0 0 0 0 0]
                                         * Referenced by: '<S5>/Assumes takeoff was level'
                                         */
  real_T simualterandomusbdelays_Initial[8];/* Expression: [quadEDT.sensordataCalib(1:6),0,quadEDT.sensordataCalib(7)]
                                             * Referenced by: '<S5>/simualte random usbdelays'
                                             */
  real_T RandomNumber_Mean;            /* Expression: 0
                                        * Referenced by: '<S10>/Random Number'
                                        */
  real_T RandomNumber_StdDev;          /* Computed Parameter: RandomNumber_StdDev
                                        * Referenced by: '<S10>/Random Number'
                                        */
  real_T RandomNumber_Seed[12];        /* Expression: s
                                        * Referenced by: '<S10>/Random Number'
                                        */
  real_T MatrixGain_Gain[144];         /* Expression: d
                                        * Referenced by: '<S10>/Matrix Gain'
                                        */
  real_T Buffer_ic;                    /* Expression: 0
                                        * Referenced by: '<S10>/Buffer'
                                        */
  real_T alt_Gain_Gain;                /* Expression: -1
                                        * Referenced by: '<S5>/alt_Gain'
                                        */
  real_T Saturation_UpperSat;          /* Expression: inf
                                        * Referenced by: '<S5>/Saturation'
                                        */
  real_T gravity_Value[3];             /* Expression: [0 0 -quad.g]
                                        * Referenced by: '<S5>/gravity'
                                        */
  real_T simulatedsensorupdateratedelays[8];/* Expression: [quadEDT.sensordataCalib(1:6),0,quadEDT.sensordataCalib(7)]
                                             * Referenced by: '<S5>/simulated sensorupdaterate delays'
                                             */
  real_T omegaSaturation_LowerSat;     /* Expression: 100
                                        * Referenced by: '<S3>/omegaSaturation'
                                        */
  real_T Directions_Gain[4];           /* Expression: [1 -1 1 -1]
                                        * Referenced by: '<S3>/Directions'
                                        */
  uint32_T Delay3_DelayLength;         /* Computed Parameter: Delay3_DelayLength
                                        * Referenced by: '<S11>/Delay3'
                                        */
  uint32_T Delay1_DelayLength;         /* Computed Parameter: Delay1_DelayLength
                                        * Referenced by: '<S11>/Delay1'
                                        */
  uint32_T simualterandomusbdelays_DelayLe;/* Computed Parameter: simualterandomusbdelays_DelayLe
                                            * Referenced by: '<S5>/simualte random usbdelays'
                                            */
  uint32_T Delay1_DelayLength_n;       /* Computed Parameter: Delay1_DelayLength_n
                                        * Referenced by: '<S1>/Delay1'
                                        */
  uint32_T simulatedsensorupdateratedela_g;/* Computed Parameter: simulatedsensorupdateratedela_g
                                            * Referenced by: '<S5>/simulated sensorupdaterate delays'
                                            */
};

/* Real-time Model Data Structure */
struct tag_RTM_SimulatorDrone_T {
  const char_T * volatile errorStatus;
  RTWSolverInfo solverInfo;

  /*
   * ModelData:
   * The following substructure contains information regarding
   * the data used in the model.
   */
  struct {
    B_SimulatorDrone_T *blockIO;
    P_SimulatorDrone_T *defaultParam;
    DW_SimulatorDrone_T *dwork;
  } ModelData;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    time_T stepSize0;
    uint32_T clockTick1;
    SimTimeStep simTimeStep;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Model entry point functions */
extern void SimulatorDrone_initialize(RT_MODEL_SimulatorDrone_T *const
  SimulatorDrone_M, real_T SimulatorDrone_U_y[4], real_T *SimulatorDrone_Y_X,
  real_T *SimulatorDrone_Y_Y, real_T *SimulatorDrone_Y_Z, real_T
  *SimulatorDrone_Y_dx, real_T *SimulatorDrone_Y_dy, real_T *SimulatorDrone_Y_dz,
  real_T *SimulatorDrone_Y_yaw, real_T *SimulatorDrone_Y_pitch, real_T
  *SimulatorDrone_Y_roll, real_T SimulatorDrone_Y_quaternions_datout[4], real_T *
  SimulatorDrone_Y_p, real_T *SimulatorDrone_Y_q, real_T *SimulatorDrone_Y_r,
  real_T SimulatorDrone_Y_opticalFlow_datout[3], real_T
  SimulatorDrone_Y_sensordataCalib_daout[7], real_T *SimulatorDrone_Y_ddx,
  real_T *SimulatorDrone_Y_ddy, real_T *SimulatorDrone_Y_ddz, real_T
  *SimulatorDrone_Y_pb, real_T *SimulatorDrone_Y_qb, real_T *SimulatorDrone_Y_rb,
  real_T *SimulatorDrone_Y_altitude_sonar, real_T *SimulatorDrone_Y_prs);
extern void SimulatorDrone_step(RT_MODEL_SimulatorDrone_T *const
  SimulatorDrone_M, real_T SimulatorDrone_U_y[4], real_T *SimulatorDrone_Y_X,
  real_T *SimulatorDrone_Y_Y, real_T *SimulatorDrone_Y_Z, real_T
  *SimulatorDrone_Y_dx, real_T *SimulatorDrone_Y_dy, real_T *SimulatorDrone_Y_dz,
  real_T *SimulatorDrone_Y_yaw, real_T *SimulatorDrone_Y_pitch, real_T
  *SimulatorDrone_Y_roll, real_T SimulatorDrone_Y_quaternions_datout[4], real_T *
  SimulatorDrone_Y_p, real_T *SimulatorDrone_Y_q, real_T *SimulatorDrone_Y_r,
  real_T SimulatorDrone_Y_opticalFlow_datout[3], real_T
  SimulatorDrone_Y_sensordataCalib_daout[7], real_T *SimulatorDrone_Y_ddx,
  real_T *SimulatorDrone_Y_ddy, real_T *SimulatorDrone_Y_ddz, real_T
  *SimulatorDrone_Y_pb, real_T *SimulatorDrone_Y_qb, real_T *SimulatorDrone_Y_rb,
  real_T *SimulatorDrone_Y_altitude_sonar, real_T *SimulatorDrone_Y_prs);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('sim_quadrotor/SimulatorDrone')    - opens subsystem sim_quadrotor/SimulatorDrone
 * hilite_system('sim_quadrotor/SimulatorDrone/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'sim_quadrotor'
 * '<S1>'   : 'sim_quadrotor/SimulatorDrone'
 * '<S2>'   : 'sim_quadrotor/SimulatorDrone/DroneRS_Sensorsystem'
 * '<S3>'   : 'sim_quadrotor/SimulatorDrone/Drone_Dynamics'
 * '<S4>'   : 'sim_quadrotor/SimulatorDrone/Rotation Angles to Quaternions'
 * '<S5>'   : 'sim_quadrotor/SimulatorDrone/DroneRS_Sensorsystem/IMU_Pressure'
 * '<S6>'   : 'sim_quadrotor/SimulatorDrone/DroneRS_Sensorsystem/camera'
 * '<S7>'   : 'sim_quadrotor/SimulatorDrone/DroneRS_Sensorsystem/IMU_Pressure/ Noise'
 * '<S8>'   : 'sim_quadrotor/SimulatorDrone/DroneRS_Sensorsystem/IMU_Pressure/trafo_Body2World_trans'
 * '<S9>'   : 'sim_quadrotor/SimulatorDrone/DroneRS_Sensorsystem/IMU_Pressure/trafo_World2Body_trans'
 * '<S10>'  : 'sim_quadrotor/SimulatorDrone/DroneRS_Sensorsystem/IMU_Pressure/ Noise/Gaussian Noise Generator'
 * '<S11>'  : 'sim_quadrotor/SimulatorDrone/Drone_Dynamics/Dynamics'
 * '<S12>'  : 'sim_quadrotor/SimulatorDrone/Drone_Dynamics/MATLAB Function'
 * '<S13>'  : 'sim_quadrotor/SimulatorDrone/Drone_Dynamics/Dynamics/Compare To Constant'
 * '<S14>'  : 'sim_quadrotor/SimulatorDrone/Drone_Dynamics/Dynamics/Compare To Constant1'
 * '<S15>'  : 'sim_quadrotor/SimulatorDrone/Drone_Dynamics/Dynamics/MATLAB Function'
 * '<S16>'  : 'sim_quadrotor/SimulatorDrone/Drone_Dynamics/Dynamics/statetoout'
 */
#endif                                 /* RTW_HEADER_SimulatorDrone_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
