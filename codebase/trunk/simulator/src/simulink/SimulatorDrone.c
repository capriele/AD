/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: SimulatorDrone.c
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

#include "SimulatorDrone.h"
#include "SimulatorDrone_private.h"

/* Forward declaration for local functions */
static void SimulatorDrone_inv_a(const real_T x[9], real_T y[9]);
static void SimulatorDrone_sum(const real_T x[12], real_T y[3]);
static void SimulatorDrone_inv(const real_T x[9], real_T y[9]);

/* Function for MATLAB Function: '<S11>/statetoout' */
static void SimulatorDrone_inv_a(const real_T x[9], real_T y[9])
{
  real_T b_x[9];
  int32_T p1;
  int32_T p2;
  int32_T p3;
  real_T absx11;
  real_T absx21;
  real_T absx31;
  int32_T itmp;
  real_T b_y;
  memcpy(&b_x[0], &x[0], 9U * sizeof(real_T));
  p1 = 0;
  p2 = 3;
  p3 = 6;
  absx11 = fabs(x[0]);
  absx21 = fabs(x[1]);
  absx31 = fabs(x[2]);
  if ((absx21 > absx11) && (absx21 > absx31)) {
    p1 = 3;
    p2 = 0;
    b_x[0] = x[1];
    b_x[1] = x[0];
    b_x[3] = x[4];
    b_x[4] = x[3];
    b_x[6] = x[7];
    b_x[7] = x[6];
  } else {
    if (absx31 > absx11) {
      p1 = 6;
      p3 = 0;
      b_x[0] = x[2];
      b_x[2] = x[0];
      b_x[3] = x[5];
      b_x[5] = x[3];
      b_x[6] = x[8];
      b_x[8] = x[6];
    }
  }

  absx11 = b_x[1] / b_x[0];
  b_x[1] /= b_x[0];
  absx21 = b_x[2] / b_x[0];
  b_x[2] /= b_x[0];
  b_x[4] -= absx11 * b_x[3];
  b_x[5] -= absx21 * b_x[3];
  b_x[7] -= absx11 * b_x[6];
  b_x[8] -= absx21 * b_x[6];
  if (fabs(b_x[5]) > fabs(b_x[4])) {
    itmp = p2;
    p2 = p3;
    p3 = itmp;
    b_x[1] = absx21;
    b_x[2] = absx11;
    absx11 = b_x[4];
    b_x[4] = b_x[5];
    b_x[5] = absx11;
    absx11 = b_x[7];
    b_x[7] = b_x[8];
    b_x[8] = absx11;
  }

  absx31 = b_x[5];
  b_y = b_x[4];
  absx11 = b_x[5] / b_x[4];
  b_x[8] -= absx11 * b_x[7];
  absx11 = (absx11 * b_x[1] - b_x[2]) / b_x[8];
  absx21 = -(b_x[7] * absx11 + b_x[1]) / b_x[4];
  y[p1] = ((1.0 - b_x[3] * absx21) - b_x[6] * absx11) / b_x[0];
  y[p1 + 1] = absx21;
  y[p1 + 2] = absx11;
  absx11 = -(absx31 / b_y) / b_x[8];
  absx21 = (1.0 - b_x[7] * absx11) / b_x[4];
  y[p2] = -(b_x[3] * absx21 + b_x[6] * absx11) / b_x[0];
  y[p2 + 1] = absx21;
  y[p2 + 2] = absx11;
  absx11 = 1.0 / b_x[8];
  absx21 = -b_x[7] * absx11 / b_x[4];
  y[p3] = -(b_x[3] * absx21 + b_x[6] * absx11) / b_x[0];
  y[p3 + 1] = absx21;
  y[p3 + 2] = absx11;
}

/* Function for MATLAB Function: '<S11>/MATLAB Function' */
static void SimulatorDrone_sum(const real_T x[12], real_T y[3])
{
  int32_T ix;
  int32_T iy;
  int32_T ixstart;
  real_T s;
  int32_T j;
  iy = -1;
  ixstart = -1;
  for (j = 0; j < 3; j++) {
    ixstart++;
    ix = ixstart + 3;
    s = x[ixstart] + x[ix];
    ix += 3;
    s += x[ix];
    ix += 3;
    s += x[ix];
    iy++;
    y[iy] = s;
  }
}

/* Function for MATLAB Function: '<S11>/MATLAB Function' */
static void SimulatorDrone_inv(const real_T x[9], real_T y[9])
{
  real_T b_x[9];
  int32_T p1;
  int32_T p2;
  int32_T p3;
  real_T absx11;
  real_T absx21;
  real_T absx31;
  int32_T itmp;
  real_T b_y;
  memcpy(&b_x[0], &x[0], 9U * sizeof(real_T));
  p1 = 0;
  p2 = 3;
  p3 = 6;
  absx11 = fabs(x[0]);
  absx21 = fabs(x[1]);
  absx31 = fabs(x[2]);
  if ((absx21 > absx11) && (absx21 > absx31)) {
    p1 = 3;
    p2 = 0;
    b_x[0] = x[1];
    b_x[1] = x[0];
    b_x[3] = x[4];
    b_x[4] = x[3];
    b_x[6] = x[7];
    b_x[7] = x[6];
  } else {
    if (absx31 > absx11) {
      p1 = 6;
      p3 = 0;
      b_x[0] = x[2];
      b_x[2] = x[0];
      b_x[3] = x[5];
      b_x[5] = x[3];
      b_x[6] = x[8];
      b_x[8] = x[6];
    }
  }

  absx11 = b_x[1] / b_x[0];
  b_x[1] /= b_x[0];
  absx21 = b_x[2] / b_x[0];
  b_x[2] /= b_x[0];
  b_x[4] -= absx11 * b_x[3];
  b_x[5] -= absx21 * b_x[3];
  b_x[7] -= absx11 * b_x[6];
  b_x[8] -= absx21 * b_x[6];
  if (fabs(b_x[5]) > fabs(b_x[4])) {
    itmp = p2;
    p2 = p3;
    p3 = itmp;
    b_x[1] = absx21;
    b_x[2] = absx11;
    absx11 = b_x[4];
    b_x[4] = b_x[5];
    b_x[5] = absx11;
    absx11 = b_x[7];
    b_x[7] = b_x[8];
    b_x[8] = absx11;
  }

  absx31 = b_x[5];
  b_y = b_x[4];
  absx11 = b_x[5] / b_x[4];
  b_x[8] -= absx11 * b_x[7];
  absx11 = (absx11 * b_x[1] - b_x[2]) / b_x[8];
  absx21 = -(b_x[7] * absx11 + b_x[1]) / b_x[4];
  y[p1] = ((1.0 - b_x[3] * absx21) - b_x[6] * absx11) / b_x[0];
  y[p1 + 1] = absx21;
  y[p1 + 2] = absx11;
  absx11 = -(absx31 / b_y) / b_x[8];
  absx21 = (1.0 - b_x[7] * absx11) / b_x[4];
  y[p2] = -(b_x[3] * absx21 + b_x[6] * absx11) / b_x[0];
  y[p2 + 1] = absx21;
  y[p2 + 2] = absx11;
  absx11 = 1.0 / b_x[8];
  absx21 = -b_x[7] * absx11 / b_x[4];
  y[p3] = -(b_x[3] * absx21 + b_x[6] * absx11) / b_x[0];
  y[p3 + 1] = absx21;
  y[p3 + 2] = absx11;
}

real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  int32_T u0_0;
  int32_T u1_0;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    if (u0 > 0.0) {
      u0_0 = 1;
    } else {
      u0_0 = -1;
    }

    if (u1 > 0.0) {
      u1_0 = 1;
    } else {
      u1_0 = -1;
    }

    y = atan2(u0_0, u1_0);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

real_T rt_powd_snf(real_T u0, real_T u1)
{
  real_T y;
  real_T tmp;
  real_T tmp_0;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else {
    tmp = fabs(u0);
    tmp_0 = fabs(u1);
    if (rtIsInf(u1)) {
      if (tmp == 1.0) {
        y = (rtNaN);
      } else if (tmp > 1.0) {
        if (u1 > 0.0) {
          y = (rtInf);
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = (rtInf);
      }
    } else if (tmp_0 == 0.0) {
      y = 1.0;
    } else if (tmp_0 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = (rtNaN);
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

real_T rt_urand_Upu32_Yd_f_pw_snf(uint32_T *u)
{
  uint32_T lo;
  uint32_T hi;

  /* Uniform random number generator (random number between 0 and 1)

     #define IA      16807                      magic multiplier = 7^5
     #define IM      2147483647                 modulus = 2^31-1
     #define IQ      127773                     IM div IA
     #define IR      2836                       IM modulo IA
     #define S       4.656612875245797e-10      reciprocal of 2^31-1
     test = IA * (seed % IQ) - IR * (seed/IQ)
     seed = test < 0 ? (test + IM) : test
     return (seed*S)
   */
  lo = *u % 127773U * 16807U;
  hi = *u / 127773U * 2836U;
  if (lo < hi) {
    *u = 2147483647U - (hi - lo);
  } else {
    *u = lo - hi;
  }

  return (real_T)*u * 4.6566128752457969E-10;
}

real_T rt_nrand_Upu32_Yd_f_pw_snf(uint32_T *u)
{
  real_T y;
  real_T sr;
  real_T si;

  /* Normal (Gaussian) random number generator */
  do {
    sr = 2.0 * rt_urand_Upu32_Yd_f_pw_snf(u) - 1.0;
    si = 2.0 * rt_urand_Upu32_Yd_f_pw_snf(u) - 1.0;
    si = sr * sr + si * si;
  } while (si > 1.0);

  y = sqrt(-2.0 * log(si) / si) * sr;
  return y;
}

/* Model step function */
void SimulatorDrone_step(RT_MODEL_SimulatorDrone_T *const SimulatorDrone_M,
  real_T SimulatorDrone_U_y[4], real_T *SimulatorDrone_Y_X, real_T
  *SimulatorDrone_Y_Y, real_T *SimulatorDrone_Y_Z, real_T *SimulatorDrone_Y_dx,
  real_T *SimulatorDrone_Y_dy, real_T *SimulatorDrone_Y_dz, real_T
  *SimulatorDrone_Y_yaw, real_T *SimulatorDrone_Y_pitch, real_T
  *SimulatorDrone_Y_roll, real_T SimulatorDrone_Y_quaternions_datout[4], real_T *
  SimulatorDrone_Y_p, real_T *SimulatorDrone_Y_q, real_T *SimulatorDrone_Y_r,
  real_T SimulatorDrone_Y_opticalFlow_datout[3], real_T
  SimulatorDrone_Y_sensordataCalib_daout[7], real_T *SimulatorDrone_Y_ddx,
  real_T *SimulatorDrone_Y_ddy, real_T *SimulatorDrone_Y_ddz, real_T
  *SimulatorDrone_Y_pb, real_T *SimulatorDrone_Y_qb, real_T *SimulatorDrone_Y_rb,
  real_T *SimulatorDrone_Y_altitude_sonar, real_T *SimulatorDrone_Y_prs)
{
  P_SimulatorDrone_T *SimulatorDrone_P = ((P_SimulatorDrone_T *)
    SimulatorDrone_M->ModelData.defaultParam);
  B_SimulatorDrone_T *SimulatorDrone_B = ((B_SimulatorDrone_T *)
    SimulatorDrone_M->ModelData.blockIO);
  DW_SimulatorDrone_T *SimulatorDrone_DW = ((DW_SimulatorDrone_T *)
    SimulatorDrone_M->ModelData.dwork);

  /* local block i/o variables */
  real_T rtb_Clock;
  real_T rtb_TmpSignalConversionAtsimula[8];
  real_T rtb_simulatedsensorupdateratede[8];
  boolean_T rtb_LogicalOperator;

  {
    real_T (*lastU)[3];
    real_T a1s[4];
    real_T b1s[4];
    real_T Q[12];
    real_T tau[12];
    real_T D[12];
    real_T R[9];
    static const int8_T b[3] = { 0, 0, 1 };

    real_T rtb_RandomNumber[12];
    real_T rtb_sys[16];
    real_T rtb_u[3];
    real_T rtb_alt_Gain;
    real_T rtb_Add_p[8];
    real_T rtb_TmpSignalConversionAtSFunct[16];
    int32_T i;
    real_T tmp[3];
    real_T tmp_0[3];
    real_T tmp_1[9];
    real_T tmp_2[9];
    real_T tmp_3[3];
    int32_T i_0;
    real_T tmp_4[9];
    real_T tmp_5[9];
    real_T tmp_6[9];
    real_T rtb_y;
    real_T rtb_sincos_o1_idx_0;
    real_T rtb_acc_Word_idx_0;
    real_T rtb_sincos_o1_idx_1;
    real_T rtb_acc_Word_idx_1;
    real_T rtb_sincos_o1_idx_2;
    real_T rtb_acc_Word_idx_2;
    real_T rtb_y_idx_0;
    real_T rtb_y_idx_1;
    real_T rtb_y_idx_2;

    /* Clock: '<S11>/Clock' */
    rtb_Clock = SimulatorDrone_M->Timing.t[0];

    /* Delay: '<S11>/Delay1' */
    memcpy(&rtb_RandomNumber[0], &SimulatorDrone_DW->Delay1_DSTATE[0], 12U *
           sizeof(real_T));

    /* Logic: '<S11>/Logical Operator' incorporates:
     *  Constant: '<S13>/Constant'
     *  Constant: '<S14>/Constant'
     *  Delay: '<S11>/Delay3'
     *  RelationalOperator: '<S13>/Compare'
     *  RelationalOperator: '<S14>/Compare'
     */
    rtb_LogicalOperator = ((SimulatorDrone_DW->Delay3_DSTATE[8] >
      SimulatorDrone_P->CompareToConstant1_const) && (rtb_RandomNumber[2] >=
      SimulatorDrone_P->CompareToConstant_const));
    for (i = 0; i < 12; i++) {
      /* DiscreteIntegrator: '<S11>/IntPosestates' incorporates:
       *  Constant: '<S11>/Constant14'
       */
      if (rtb_LogicalOperator ||
          (SimulatorDrone_DW->IntPosestates_PrevResetState != 0)) {
        SimulatorDrone_DW->IntPosestates_DSTATE[i] =
          SimulatorDrone_P->Constant14_Value[i];
      }

      SimulatorDrone_B->IntPosestates[i] =
        SimulatorDrone_DW->IntPosestates_DSTATE[i];

      /* End of DiscreteIntegrator: '<S11>/IntPosestates' */

      /* SignalConversion: '<S16>/TmpSignal ConversionAt SFunction Inport2' incorporates:
       *  MATLAB Function: '<S11>/statetoout'
       */
      rtb_TmpSignalConversionAtSFunct[i] = SimulatorDrone_B->IntPosestates[i];
    }

    /* SignalConversion: '<S16>/TmpSignal ConversionAt SFunction Inport2' incorporates:
     *  DiscreteIntegrator: '<S11>/IntMotors'
     *  MATLAB Function: '<S11>/statetoout'
     */
    rtb_TmpSignalConversionAtSFunct[12] = SimulatorDrone_DW->IntMotors_DSTATE[0];
    rtb_TmpSignalConversionAtSFunct[13] = SimulatorDrone_DW->IntMotors_DSTATE[1];
    rtb_TmpSignalConversionAtSFunct[14] = SimulatorDrone_DW->IntMotors_DSTATE[2];
    rtb_TmpSignalConversionAtSFunct[15] = SimulatorDrone_DW->IntMotors_DSTATE[3];

    /* MATLAB Function: '<S11>/statetoout' incorporates:
     *  DiscreteIntegrator: '<S11>/IntMotors'
     */
    /* MATLAB Function 'SimulatorDrone/Drone_Dynamics/Dynamics/statetoout': '<S16>:1' */
    /* ============================================================== */
    /*  mdlOutputs */
    /*  Calculate the output vector for this timestep */
    /* ============================================================== */
    /*  */
    /* rebuilt quad. */
    /* CRASH DETECTION */
    /*      if x(3)>0 */
    /*          x(3) = 0; */
    /*          if x(6) > 0 */
    /*              x(6) = 0; */
    /*          end */
    /*      end */
    /* if (x(3)>0)&(crash) */
    /*     error('CRASH!') */
    /* end */
    /* TELEMETRY */
    /*  compute output vector as a function of state vector */
    /*    z      Position                         3x1   (x,y,z) */
    /*    v      Velocity                         3x1   (xd,yd,zd) */
    /*    n      Attitude                         3x1   (Y,P,R) */
    /*    o      Angular velocity                 3x1   (Yd,Pd,Rd) */
    /* '<S16>:1:46' */
    /*  RPY angles */
    /* '<S16>:1:47' */
    /*  yaw */
    /* '<S16>:1:48' */
    /*  pitch */
    /* '<S16>:1:49' */
    /*  roll */
    /*  rotz(phi)*roty(the)*rotx(psi) */
    /* BBF > Inertial rotation matrix */
    /* '<S16>:1:53' */
    /* inverted Wronskian */
    /*  return velocity in the body frame */
    /* output global pos and euler angles */
    /* translational velocity mapped to body frame -> i.e. output v in bodyframe! (F) */
    /* iW*x(10:12)];    % RPY rates mapped to body frame ->     i.e. : this outputed euler rates!, now in bodyrates pqr (F) */
    /* '<S16>:1:62' */
    /* motor speed */
    /* sys = [x(1:6); iW*x(7:9);  iW*x(10:12)]; */
    /* sys = x; */
    /*  End of mdlOutputs. */
    tmp_1[0] = cos(rtb_TmpSignalConversionAtSFunct[4]) * cos
      (rtb_TmpSignalConversionAtSFunct[3]);
    tmp_1[3] = sin(rtb_TmpSignalConversionAtSFunct[5]) * sin
      (rtb_TmpSignalConversionAtSFunct[4]) * cos
      (rtb_TmpSignalConversionAtSFunct[3]) - cos
      (rtb_TmpSignalConversionAtSFunct[5]) * sin
      (rtb_TmpSignalConversionAtSFunct[3]);
    tmp_1[6] = cos(rtb_TmpSignalConversionAtSFunct[5]) * sin
      (rtb_TmpSignalConversionAtSFunct[4]) * cos
      (rtb_TmpSignalConversionAtSFunct[3]) + sin
      (rtb_TmpSignalConversionAtSFunct[5]) * sin
      (rtb_TmpSignalConversionAtSFunct[3]);
    tmp_1[1] = cos(rtb_TmpSignalConversionAtSFunct[4]) * sin
      (rtb_TmpSignalConversionAtSFunct[3]);
    tmp_1[4] = sin(rtb_TmpSignalConversionAtSFunct[5]) * sin
      (rtb_TmpSignalConversionAtSFunct[4]) * sin
      (rtb_TmpSignalConversionAtSFunct[3]) + cos
      (rtb_TmpSignalConversionAtSFunct[5]) * cos
      (rtb_TmpSignalConversionAtSFunct[3]);
    tmp_1[7] = cos(rtb_TmpSignalConversionAtSFunct[5]) * sin
      (rtb_TmpSignalConversionAtSFunct[4]) * sin
      (rtb_TmpSignalConversionAtSFunct[3]) - sin
      (rtb_TmpSignalConversionAtSFunct[5]) * cos
      (rtb_TmpSignalConversionAtSFunct[3]);
    tmp_1[2] = -sin(rtb_TmpSignalConversionAtSFunct[4]);
    tmp_1[5] = sin(rtb_TmpSignalConversionAtSFunct[5]) * cos
      (rtb_TmpSignalConversionAtSFunct[4]);
    tmp_1[8] = cos(rtb_TmpSignalConversionAtSFunct[5]) * cos
      (rtb_TmpSignalConversionAtSFunct[4]);
    SimulatorDrone_inv_a(tmp_1, tmp_2);
    for (i_0 = 0; i_0 < 3; i_0++) {
      tmp_3[i_0] = tmp_2[i_0 + 6] * rtb_TmpSignalConversionAtSFunct[8] +
        (tmp_2[i_0 + 3] * rtb_TmpSignalConversionAtSFunct[7] + tmp_2[i_0] *
         rtb_TmpSignalConversionAtSFunct[6]);
    }

    for (i_0 = 0; i_0 < 6; i_0++) {
      rtb_sys[i_0] = rtb_TmpSignalConversionAtSFunct[i_0];
    }

    rtb_sys[6] = tmp_3[0];
    rtb_sys[7] = tmp_3[1];
    rtb_sys[8] = tmp_3[2];
    rtb_sys[9] = rtb_TmpSignalConversionAtSFunct[9];
    rtb_sys[10] = rtb_TmpSignalConversionAtSFunct[10];
    rtb_sys[11] = rtb_TmpSignalConversionAtSFunct[11];
    rtb_sys[12] = SimulatorDrone_DW->IntMotors_DSTATE[0];
    rtb_sys[13] = SimulatorDrone_DW->IntMotors_DSTATE[1];
    rtb_sys[14] = SimulatorDrone_DW->IntMotors_DSTATE[2];
    rtb_sys[15] = SimulatorDrone_DW->IntMotors_DSTATE[3];

    /* Outport: '<Root>/X' */
    *SimulatorDrone_Y_X = rtb_sys[0];

    /* Outport: '<Root>/Y' */
    *SimulatorDrone_Y_Y = rtb_sys[1];

    /* Outport: '<Root>/Z' */
    *SimulatorDrone_Y_Z = rtb_sys[2];

    /* Outport: '<Root>/dx' incorporates:
     *  MATLAB Function: '<S11>/statetoout'
     */
    *SimulatorDrone_Y_dx = tmp_3[0];

    /* Outport: '<Root>/dy' incorporates:
     *  MATLAB Function: '<S11>/statetoout'
     */
    *SimulatorDrone_Y_dy = tmp_3[1];

    /* Outport: '<Root>/dz' incorporates:
     *  MATLAB Function: '<S11>/statetoout'
     */
    *SimulatorDrone_Y_dz = tmp_3[2];

    /* Outport: '<Root>/yaw' */
    *SimulatorDrone_Y_yaw = rtb_sys[3];

    /* Outport: '<Root>/pitch' */
    *SimulatorDrone_Y_pitch = rtb_sys[4];

    /* Outport: '<Root>/roll' */
    *SimulatorDrone_Y_roll = rtb_sys[5];

    /* Gain: '<S4>/1//2' */
    rtb_u[0] = SimulatorDrone_P->u_Gain * rtb_sys[3];
    rtb_u[1] = SimulatorDrone_P->u_Gain * rtb_sys[4];
    rtb_u[2] = SimulatorDrone_P->u_Gain * rtb_sys[5];

    /* Trigonometry: '<S4>/sincos' */
    rtb_sincos_o1_idx_0 = sin(rtb_u[0]);
    rtb_acc_Word_idx_0 = cos(rtb_u[0]);
    rtb_sincos_o1_idx_1 = sin(rtb_u[1]);
    rtb_acc_Word_idx_1 = cos(rtb_u[1]);
    rtb_sincos_o1_idx_2 = sin(rtb_u[2]);
    rtb_acc_Word_idx_2 = cos(rtb_u[2]);

    /* Outport: '<Root>/quaternions_datout' incorporates:
     *  Fcn: '<S4>/q0'
     *  Fcn: '<S4>/q1'
     *  Fcn: '<S4>/q2'
     *  Fcn: '<S4>/q3'
     */
    SimulatorDrone_Y_quaternions_datout[0] = rtb_acc_Word_idx_0 *
      rtb_acc_Word_idx_1 * rtb_acc_Word_idx_2 + rtb_sincos_o1_idx_0 *
      rtb_sincos_o1_idx_1 * rtb_sincos_o1_idx_2;
    SimulatorDrone_Y_quaternions_datout[1] = rtb_acc_Word_idx_0 *
      rtb_acc_Word_idx_1 * rtb_sincos_o1_idx_2 - rtb_sincos_o1_idx_0 *
      rtb_sincos_o1_idx_1 * rtb_acc_Word_idx_2;
    SimulatorDrone_Y_quaternions_datout[2] = rtb_acc_Word_idx_0 *
      rtb_sincos_o1_idx_1 * rtb_acc_Word_idx_2 + rtb_sincos_o1_idx_0 *
      rtb_acc_Word_idx_1 * rtb_sincos_o1_idx_2;
    SimulatorDrone_Y_quaternions_datout[3] = rtb_sincos_o1_idx_0 *
      rtb_acc_Word_idx_1 * rtb_acc_Word_idx_2 - rtb_acc_Word_idx_0 *
      rtb_sincos_o1_idx_1 * rtb_sincos_o1_idx_2;

    /* Outport: '<Root>/p' incorporates:
     *  MATLAB Function: '<S11>/statetoout'
     */
    *SimulatorDrone_Y_p = rtb_TmpSignalConversionAtSFunct[9];

    /* Outport: '<Root>/q' incorporates:
     *  MATLAB Function: '<S11>/statetoout'
     */
    *SimulatorDrone_Y_q = rtb_TmpSignalConversionAtSFunct[10];

    /* Outport: '<Root>/r' incorporates:
     *  MATLAB Function: '<S11>/statetoout'
     */
    *SimulatorDrone_Y_r = rtb_TmpSignalConversionAtSFunct[11];

    /* Gain: '<S6>/Gain2' */
    rtb_alt_Gain = SimulatorDrone_P->Gain2_Gain * rtb_sys[2];

    /* Product: '<S6>/Product1' incorporates:
     *  Gain: '<S6>/Gain1'
     *  MATLAB Function: '<S11>/statetoout'
     */
    rtb_acc_Word_idx_2 = SimulatorDrone_P->Gain1_Gain[1] *
      rtb_TmpSignalConversionAtSFunct[10] * rtb_alt_Gain;

    /* Product: '<S6>/Product' incorporates:
     *  Gain: '<S6>/Gain1'
     *  MATLAB Function: '<S11>/statetoout'
     */
    rtb_alt_Gain *= SimulatorDrone_P->Gain1_Gain[0] *
      rtb_TmpSignalConversionAtSFunct[9];

    /* Gain: '<S6>/VelocityToOpticalFlow_Gain' incorporates:
     *  MATLAB Function: '<S11>/statetoout'
     *  Sum: '<S6>/Add1'
     */
    SimulatorDrone_B->VelocityToOpticalFlow_Gain[0] = (tmp_3[0] +
      rtb_acc_Word_idx_2) * SimulatorDrone_P->quadEDT.velocityToOpticalFlow_gain;
    SimulatorDrone_B->VelocityToOpticalFlow_Gain[1] = (tmp_3[1] + rtb_alt_Gain) *
      SimulatorDrone_P->quadEDT.velocityToOpticalFlow_gain;
    SimulatorDrone_B->VelocityToOpticalFlow_Gain[2] =
      SimulatorDrone_P->quadEDT.velocityToOpticalFlow_gain * tmp_3[2];

    /* Outport: '<Root>/opticalFlow_datout' */
    SimulatorDrone_Y_opticalFlow_datout[0] =
      SimulatorDrone_B->VelocityToOpticalFlow_Gain[0];
    SimulatorDrone_Y_opticalFlow_datout[1] =
      SimulatorDrone_B->VelocityToOpticalFlow_Gain[1];
    SimulatorDrone_Y_opticalFlow_datout[2] =
      SimulatorDrone_B->VelocityToOpticalFlow_Gain[2];

    /* Outport: '<Root>/sensordataCalib_daout' incorporates:
     *  Constant: '<S1>/sensordataCalib'
     */
    for (i = 0; i < 7; i++) {
      SimulatorDrone_Y_sensordataCalib_daout[i] =
        SimulatorDrone_P->quadEDT.sensordataCalib[i];
    }

    /* End of Outport: '<Root>/sensordataCalib_daout' */

    /* Sum: '<S5>/Add' incorporates:
     *  Bias: '<S5>/Assumes takeoff was level'
     *  Constant: '<S1>/sensordataCalib'
     *  Delay: '<S5>/simualte random usbdelays'
     */
    for (i = 0; i < 6; i++) {
      rtb_Add_p[i] = (SimulatorDrone_P->quadEDT.sensordataCalib[i] +
                      SimulatorDrone_P->Assumestakeoffwaslevel_Bias[i]) +
        SimulatorDrone_DW->simualterandomusbdelays_DSTATE[i];
    }

    /* Outport: '<Root>/ddx' */
    *SimulatorDrone_Y_ddx = rtb_Add_p[0];

    /* Outport: '<Root>/ddy' */
    *SimulatorDrone_Y_ddy = rtb_Add_p[1];

    /* Outport: '<Root>/ddz' */
    *SimulatorDrone_Y_ddz = rtb_Add_p[2];

    /* Outport: '<Root>/pb' */
    *SimulatorDrone_Y_pb = rtb_Add_p[3];

    /* Outport: '<Root>/qb' */
    *SimulatorDrone_Y_qb = rtb_Add_p[4];

    /* Outport: '<Root>/rb' */
    *SimulatorDrone_Y_rb = rtb_Add_p[5];

    /* Outport: '<Root>/altitude_sonar' incorporates:
     *  Bias: '<S5>/Assumes takeoff was level'
     *  Constant: '<S5>/zeroconst'
     *  Delay: '<S5>/simualte random usbdelays'
     *  Sum: '<S5>/Add'
     */
    *SimulatorDrone_Y_altitude_sonar = (SimulatorDrone_P->zeroconst_Value +
      SimulatorDrone_P->Assumestakeoffwaslevel_Bias[6]) +
      SimulatorDrone_DW->simualterandomusbdelays_DSTATE[6];

    /* Outport: '<Root>/prs' incorporates:
     *  Bias: '<S5>/Assumes takeoff was level'
     *  Constant: '<S1>/sensordataCalib'
     *  Delay: '<S5>/simualte random usbdelays'
     *  Sum: '<S5>/Add'
     */
    *SimulatorDrone_Y_prs = (SimulatorDrone_P->quadEDT.sensordataCalib[6] +
      SimulatorDrone_P->Assumestakeoffwaslevel_Bias[7]) +
      SimulatorDrone_DW->simualterandomusbdelays_DSTATE[7];

    /* RandomNumber: '<S10>/Random Number' */
    memcpy(&rtb_RandomNumber[0], &SimulatorDrone_DW->NextOutput[0], 12U * sizeof
           (real_T));

    /* Sum: '<S10>/Sum' incorporates:
     *  Constant: '<S10>/Constant'
     *  Gain: '<S10>/Matrix Gain'
     *  Gain: '<S7>/noiseWeights'
     */
    for (i_0 = 0; i_0 < 12; i_0++) {
      rtb_sincos_o1_idx_0 = 0.0;
      for (i = 0; i < 12; i++) {
        rtb_sincos_o1_idx_0 += SimulatorDrone_P->MatrixGain_Gain[12 * i + i_0] *
          rtb_RandomNumber[i];
      }

      Q[i_0] = rtb_sincos_o1_idx_0 + SimulatorDrone_P->GaussianNoiseGenerator_m;
    }

    /* End of Sum: '<S10>/Sum' */

    /* Sum: '<S7>/Add' incorporates:
     *  Gain: '<S7>/noiseWeights'
     */
    for (i_0 = 0; i_0 < 12; i_0++) {
      rtb_RandomNumber[i_0] =
        SimulatorDrone_P->quadEDT.noiseStatesSensed_weights[i_0] * Q[i_0] +
        rtb_sys[i_0];
    }

    /* End of Sum: '<S7>/Add' */

    /* MATLAB Function: '<S5>/trafo_Body2World_trans' */
    /* MATLAB Function 'SimulatorDrone/DroneRS_Sensorsystem/IMU_Pressure/trafo_Body2World_trans': '<S8>:1' */
    /* '<S8>:1:3' */
    /* '<S8>:1:4' */
    /* '<S8>:1:5' */
    /* '<S8>:1:7' */
    /* '<S8>:1:11' */
    tmp_4[0] = cos(rtb_sys[4]) * cos(rtb_sys[3]);
    tmp_4[3] = cos(rtb_sys[3]) * sin(rtb_sys[4]) * sin(rtb_sys[5]) - cos
      (rtb_sys[5]) * sin(rtb_sys[3]);
    tmp_4[6] = cos(rtb_sys[5]) * cos(rtb_sys[3]) * sin(rtb_sys[4]) + sin
      (rtb_sys[5]) * sin(rtb_sys[3]);
    tmp_4[1] = cos(rtb_sys[4]) * sin(rtb_sys[3]);
    tmp_4[4] = sin(rtb_sys[4]) * sin(rtb_sys[5]) * sin(rtb_sys[3]) + cos
      (rtb_sys[5]) * cos(rtb_sys[3]);
    tmp_4[7] = cos(rtb_sys[5]) * sin(rtb_sys[4]) * sin(rtb_sys[3]) - cos
      (rtb_sys[3]) * sin(rtb_sys[5]);
    tmp_4[2] = -sin(rtb_sys[4]);
    tmp_4[5] = cos(rtb_sys[4]) * sin(rtb_sys[5]);
    tmp_4[8] = cos(rtb_sys[4]) * cos(rtb_sys[5]);
    for (i_0 = 0; i_0 < 3; i_0++) {
      SimulatorDrone_B->y[i_0] = 0.0;
      SimulatorDrone_B->y[i_0] += tmp_4[i_0] * rtb_RandomNumber[6];
      SimulatorDrone_B->y[i_0] += tmp_4[i_0 + 3] * rtb_RandomNumber[7];
      SimulatorDrone_B->y[i_0] += tmp_4[i_0 + 6] * rtb_RandomNumber[8];
    }

    /* End of MATLAB Function: '<S5>/trafo_Body2World_trans' */

    /* Derivative: '<S5>/Derivative' */
    if ((SimulatorDrone_DW->TimeStampA >= SimulatorDrone_M->Timing.t[0]) &&
        (SimulatorDrone_DW->TimeStampB >= SimulatorDrone_M->Timing.t[0])) {
      rtb_acc_Word_idx_0 = 0.0;
      rtb_acc_Word_idx_1 = 0.0;
      rtb_acc_Word_idx_2 = 0.0;
    } else {
      rtb_alt_Gain = SimulatorDrone_DW->TimeStampA;
      lastU = (real_T (*)[3])SimulatorDrone_DW->LastUAtTimeA;
      if (SimulatorDrone_DW->TimeStampA < SimulatorDrone_DW->TimeStampB) {
        if (SimulatorDrone_DW->TimeStampB < SimulatorDrone_M->Timing.t[0]) {
          rtb_alt_Gain = SimulatorDrone_DW->TimeStampB;
          lastU = (real_T (*)[3])SimulatorDrone_DW->LastUAtTimeB;
        }
      } else {
        if (SimulatorDrone_DW->TimeStampA >= SimulatorDrone_M->Timing.t[0]) {
          rtb_alt_Gain = SimulatorDrone_DW->TimeStampB;
          lastU = (real_T (*)[3])SimulatorDrone_DW->LastUAtTimeB;
        }
      }

      rtb_alt_Gain = SimulatorDrone_M->Timing.t[0] - rtb_alt_Gain;
      rtb_acc_Word_idx_0 = (SimulatorDrone_B->y[0] - (*lastU)[0]) / rtb_alt_Gain;
      rtb_acc_Word_idx_1 = (SimulatorDrone_B->y[1] - (*lastU)[1]) / rtb_alt_Gain;
      rtb_acc_Word_idx_2 = (SimulatorDrone_B->y[2] - (*lastU)[2]) / rtb_alt_Gain;
    }

    /* End of Derivative: '<S5>/Derivative' */

    /* MATLAB Function: '<S5>/trafo_World2Body_trans' */
    /* MATLAB Function 'SimulatorDrone/DroneRS_Sensorsystem/IMU_Pressure/trafo_World2Body_trans': '<S9>:1' */
    /* '<S9>:1:3' */
    /* '<S9>:1:4' */
    /* '<S9>:1:5' */
    /* '<S9>:1:7' */
    /* '<S9>:1:12' */
    tmp_5[0] = cos(rtb_sys[4]) * cos(rtb_sys[3]);
    tmp_5[3] = cos(rtb_sys[4]) * sin(rtb_sys[3]);
    tmp_5[6] = -sin(rtb_sys[4]);
    tmp_5[1] = cos(rtb_sys[3]) * sin(rtb_sys[4]) * sin(rtb_sys[5]) - cos
      (rtb_sys[5]) * sin(rtb_sys[3]);
    tmp_5[4] = sin(rtb_sys[4]) * sin(rtb_sys[5]) * sin(rtb_sys[3]) + cos
      (rtb_sys[5]) * cos(rtb_sys[3]);
    tmp_5[7] = cos(rtb_sys[4]) * sin(rtb_sys[5]);
    tmp_5[2] = cos(rtb_sys[5]) * cos(rtb_sys[3]) * sin(rtb_sys[4]) + sin
      (rtb_sys[5]) * sin(rtb_sys[3]);
    tmp_5[5] = cos(rtb_sys[5]) * sin(rtb_sys[4]) * sin(rtb_sys[3]) - cos
      (rtb_sys[3]) * sin(rtb_sys[5]);
    tmp_5[8] = cos(rtb_sys[4]) * cos(rtb_sys[5]);

    /* Sum: '<S5>/Sum1' incorporates:
     *  Constant: '<S5>/gravity'
     *  MATLAB Function: '<S5>/trafo_World2Body_trans'
     */
    rtb_alt_Gain = SimulatorDrone_P->gravity_Value[0] + rtb_acc_Word_idx_0;
    rtb_sincos_o1_idx_1 = SimulatorDrone_P->gravity_Value[1] +
      rtb_acc_Word_idx_1;
    rtb_sincos_o1_idx_0 = SimulatorDrone_P->gravity_Value[2] +
      rtb_acc_Word_idx_2;

    /* MATLAB Function: '<S5>/trafo_World2Body_trans' incorporates:
     *  Gain: '<S5>/accel_gain'
     */
    for (i_0 = 0; i_0 < 3; i_0++) {
      tmp_3[i_0] = tmp_5[i_0 + 6] * rtb_sincos_o1_idx_0 + (tmp_5[i_0 + 3] *
        rtb_sincos_o1_idx_1 + tmp_5[i_0] * rtb_alt_Gain);
    }

    /* SignalConversion: '<S5>/TmpSignal ConversionAtsimulated sensorupdaterate delaysInport1' incorporates:
     *  Gain: '<S5>/accel_gain'
     *  Gain: '<S5>/gyro_pq_gain'
     *  Gain: '<S5>/gyro_r_Gain'
     */
    rtb_TmpSignalConversionAtsimula[0] = SimulatorDrone_P->
      quadEDT.IMUaccel_gain[0] * tmp_3[0];
    rtb_TmpSignalConversionAtsimula[1] = SimulatorDrone_P->
      quadEDT.IMUaccel_gain[1] * tmp_3[1];
    rtb_TmpSignalConversionAtsimula[2] = SimulatorDrone_P->
      quadEDT.IMUaccel_gain[2] * tmp_3[2];
    rtb_TmpSignalConversionAtsimula[3] = SimulatorDrone_P->quadEDT.IMUgyro_gain
      [0] * rtb_RandomNumber[9];
    rtb_TmpSignalConversionAtsimula[4] = SimulatorDrone_P->quadEDT.IMUgyro_gain
      [1] * rtb_RandomNumber[10];
    rtb_TmpSignalConversionAtsimula[5] = SimulatorDrone_P->quadEDT.IMUgyro_gain
      [2] * rtb_RandomNumber[11];

    /* Saturate: '<S5>/Saturation' incorporates:
     *  Gain: '<S5>/alt_Gain'
     */
    rtb_sincos_o1_idx_0 = SimulatorDrone_P->alt_Gain_Gain * rtb_RandomNumber[2];
    if (rtb_sincos_o1_idx_0 > SimulatorDrone_P->Saturation_UpperSat) {
      /* SignalConversion: '<S5>/TmpSignal ConversionAtsimulated sensorupdaterate delaysInport1' */
      rtb_TmpSignalConversionAtsimula[6] = SimulatorDrone_P->Saturation_UpperSat;
    } else if (rtb_sincos_o1_idx_0 < SimulatorDrone_P->quadEDT.altSenor_min) {
      /* SignalConversion: '<S5>/TmpSignal ConversionAtsimulated sensorupdaterate delaysInport1' */
      rtb_TmpSignalConversionAtsimula[6] =
        SimulatorDrone_P->quadEDT.altSenor_min;
    } else {
      /* SignalConversion: '<S5>/TmpSignal ConversionAtsimulated sensorupdaterate delaysInport1' */
      rtb_TmpSignalConversionAtsimula[6] = rtb_sincos_o1_idx_0;
    }

    /* End of Saturate: '<S5>/Saturation' */

    /* SignalConversion: '<S5>/TmpSignal ConversionAtsimulated sensorupdaterate delaysInport1' incorporates:
     *  Gain: '<S5>/altToprs_gain'
     */
    rtb_TmpSignalConversionAtsimula[7] = SimulatorDrone_P->quadEDT.altToPrs_gain
      * rtb_RandomNumber[2];

    /* Delay: '<S5>/simulated sensorupdaterate delays' */
    memcpy(&rtb_simulatedsensorupdateratede[0],
           &SimulatorDrone_DW->simulatedsensorupdateratedelays[0], sizeof(real_T)
           << 3U);

    /* Saturate: '<S3>/pwmSaturation' incorporates:
     *  Delay: '<S1>/Delay1'
     */
    /* MATLAB Function 'SimulatorDrone/Drone_Dynamics/MATLAB Function': '<S12>:1' */
    /* '<S12>:1:2' */
    if (SimulatorDrone_DW->Delay1_DSTATE_j[0] > SimulatorDrone_P->quadEDT.maxpwm)
    {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.maxpwm;
    } else if (SimulatorDrone_DW->Delay1_DSTATE_j[0] <
               SimulatorDrone_P->quadEDT.bomega2pwm) {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.bomega2pwm;
    } else {
      rtb_sincos_o1_idx_0 = SimulatorDrone_DW->Delay1_DSTATE_j[0];
    }

    /* MATLAB Function: '<S3>/MATLAB Function' incorporates:
     *  Constant: '<S3>/Constant'
     *  Constant: '<S3>/Constant1'
     *  Constant: '<S3>/Constant2'
     *  Saturate: '<S3>/pwmSaturation'
     */
    rtb_y_idx_0 = sqrt((rtb_sincos_o1_idx_0 -
                        SimulatorDrone_P->quadEDT.bomega2pwm) /
                       SimulatorDrone_P->quadEDT.aomega2pwm) +
      SimulatorDrone_P->quadEDT.comega2pwm;

    /* Saturate: '<S3>/pwmSaturation' incorporates:
     *  Delay: '<S1>/Delay1'
     */
    if (SimulatorDrone_DW->Delay1_DSTATE_j[1] > SimulatorDrone_P->quadEDT.maxpwm)
    {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.maxpwm;
    } else if (SimulatorDrone_DW->Delay1_DSTATE_j[1] <
               SimulatorDrone_P->quadEDT.bomega2pwm) {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.bomega2pwm;
    } else {
      rtb_sincos_o1_idx_0 = SimulatorDrone_DW->Delay1_DSTATE_j[1];
    }

    /* MATLAB Function: '<S3>/MATLAB Function' incorporates:
     *  Constant: '<S3>/Constant'
     *  Constant: '<S3>/Constant1'
     *  Constant: '<S3>/Constant2'
     *  Saturate: '<S3>/pwmSaturation'
     */
    rtb_y_idx_1 = sqrt((rtb_sincos_o1_idx_0 -
                        SimulatorDrone_P->quadEDT.bomega2pwm) /
                       SimulatorDrone_P->quadEDT.aomega2pwm) +
      SimulatorDrone_P->quadEDT.comega2pwm;

    /* Saturate: '<S3>/pwmSaturation' incorporates:
     *  Delay: '<S1>/Delay1'
     */
    if (SimulatorDrone_DW->Delay1_DSTATE_j[2] > SimulatorDrone_P->quadEDT.maxpwm)
    {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.maxpwm;
    } else if (SimulatorDrone_DW->Delay1_DSTATE_j[2] <
               SimulatorDrone_P->quadEDT.bomega2pwm) {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.bomega2pwm;
    } else {
      rtb_sincos_o1_idx_0 = SimulatorDrone_DW->Delay1_DSTATE_j[2];
    }

    /* MATLAB Function: '<S3>/MATLAB Function' incorporates:
     *  Constant: '<S3>/Constant'
     *  Constant: '<S3>/Constant1'
     *  Constant: '<S3>/Constant2'
     *  Saturate: '<S3>/pwmSaturation'
     */
    rtb_y_idx_2 = sqrt((rtb_sincos_o1_idx_0 -
                        SimulatorDrone_P->quadEDT.bomega2pwm) /
                       SimulatorDrone_P->quadEDT.aomega2pwm) +
      SimulatorDrone_P->quadEDT.comega2pwm;

    /* Saturate: '<S3>/pwmSaturation' incorporates:
     *  Delay: '<S1>/Delay1'
     */
    if (SimulatorDrone_DW->Delay1_DSTATE_j[3] > SimulatorDrone_P->quadEDT.maxpwm)
    {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.maxpwm;
    } else if (SimulatorDrone_DW->Delay1_DSTATE_j[3] <
               SimulatorDrone_P->quadEDT.bomega2pwm) {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.bomega2pwm;
    } else {
      rtb_sincos_o1_idx_0 = SimulatorDrone_DW->Delay1_DSTATE_j[3];
    }

    /* MATLAB Function: '<S3>/MATLAB Function' incorporates:
     *  Constant: '<S3>/Constant'
     *  Constant: '<S3>/Constant1'
     *  Constant: '<S3>/Constant2'
     *  Saturate: '<S3>/pwmSaturation'
     */
    rtb_y = sqrt((rtb_sincos_o1_idx_0 - SimulatorDrone_P->quadEDT.bomega2pwm) /
                 SimulatorDrone_P->quadEDT.aomega2pwm) +
      SimulatorDrone_P->quadEDT.comega2pwm;
    for (i = 0; i < 12; i++) {
      /* SignalConversion: '<S15>/TmpSignal ConversionAt SFunction Inport2' incorporates:
       *  MATLAB Function: '<S11>/MATLAB Function'
       */
      rtb_TmpSignalConversionAtSFunct[i] = SimulatorDrone_B->IntPosestates[i];

      /* MATLAB Function: '<S11>/MATLAB Function' */
      rtb_RandomNumber[i] = 0.0;
    }

    /* SignalConversion: '<S15>/TmpSignal ConversionAt SFunction Inport2' incorporates:
     *  DiscreteIntegrator: '<S11>/IntMotors'
     *  MATLAB Function: '<S11>/MATLAB Function'
     */
    rtb_TmpSignalConversionAtSFunct[12] = SimulatorDrone_DW->IntMotors_DSTATE[0];
    rtb_TmpSignalConversionAtSFunct[13] = SimulatorDrone_DW->IntMotors_DSTATE[1];
    rtb_TmpSignalConversionAtSFunct[14] = SimulatorDrone_DW->IntMotors_DSTATE[2];
    rtb_TmpSignalConversionAtSFunct[15] = SimulatorDrone_DW->IntMotors_DSTATE[3];

    /* MATLAB Function: '<S11>/MATLAB Function' incorporates:
     *  Constant: '<S11>/Constant1'
     *  Constant: '<S11>/Constant10'
     *  Constant: '<S11>/Constant11'
     *  Constant: '<S11>/Constant12'
     *  Constant: '<S11>/Constant13'
     *  Constant: '<S11>/Constant15'
     *  Constant: '<S11>/Constant2'
     *  Constant: '<S11>/Constant3'
     *  Constant: '<S11>/Constant4'
     *  Constant: '<S11>/Constant5'
     *  Constant: '<S11>/Constant6'
     *  Constant: '<S11>/Constant7'
     *  Constant: '<S11>/Constant8'
     *  Constant: '<S11>/Constant9'
     *  DiscreteIntegrator: '<S11>/IntMotors'
     *  Gain: '<S3>/Directions'
     *  Saturate: '<S3>/omegaSaturation'
     */
    /* MATLAB Function 'SimulatorDrone/Drone_Dynamics/Dynamics/MATLAB Function': '<S15>:1' */
    /* % drone derivatives */
    /* global a1s b1s */
    /* rebuilt quad. */
    /* '<S15>:1:6' */
    /* '<S15>:1:7' */
    /* '<S15>:1:8' */
    rtb_sincos_o1_idx_0 = SimulatorDrone_P->quad.r;

    /* '<S15>:1:9' */
    /* '<S15>:1:10' */
    /* '<S15>:1:11' */
    rtb_sincos_o1_idx_1 = SimulatorDrone_P->quad.gamma;

    /* '<S15>:1:12' */
    /* '<S15>:1:13' */
    rtb_sincos_o1_idx_2 = SimulatorDrone_P->quad.rho;

    /* '<S15>:1:14' */
    rtb_acc_Word_idx_0 = SimulatorDrone_P->quad.A;

    /* '<S15>:1:15' */
    /* '<S15>:1:16' */
    /* '<S15>:1:17' */
    /* '<S15>:1:18' */
    /* '<S15>:1:19' */
    /* declarations/init */
    /* '<S15>:1:28' */
    /* '<S15>:1:29' */
    /* '<S15>:1:31' */
    /* '<S15>:1:32' */
    /* '<S15>:1:33' */
    /* CONSTANTS */
    /* Cardinal Direction Indicies */
    /* '<S15>:1:37' */
    /*    N       'North'                             1x1 */
    /* '<S15>:1:38' */
    /*    S       'South'                             1x1 */
    /* '<S15>:1:39' */
    /*    E       'East'                              1x1 */
    /* '<S15>:1:40' */
    /*    W       'West'                              1x1 */
    /* '<S15>:1:43' */
    rtb_alt_Gain = 0.70710678118654757 * SimulatorDrone_P->quad.d;

    /* '<S15>:1:45' */
    /* '<S15>:1:47' */
    D[0] = rtb_alt_Gain;
    D[1] = -rtb_alt_Gain;
    D[2] = SimulatorDrone_P->quad.h;

    /*    Di      Rotor hub displacements             1x3 */
    /* '<S15>:1:48' */
    D[3] = rtb_alt_Gain;
    D[4] = rtb_alt_Gain;
    D[5] = SimulatorDrone_P->quad.h;

    /* '<S15>:1:49' */
    D[6] = -rtb_alt_Gain;
    D[7] = rtb_alt_Gain;
    D[8] = SimulatorDrone_P->quad.h;

    /* '<S15>:1:50' */
    D[9] = -rtb_alt_Gain;
    D[10] = -rtb_alt_Gain;
    D[11] = SimulatorDrone_P->quad.h;

    /* Body-fixed frame references */
    /*    ei      Body fixed frame references         3x1 */
    /* EXTRACT STATES FROM U */
    /* '<S15>:1:58' */
    /* EXTRACT STATES FROM X */
    /*  position in {W} */
    /* '<S15>:1:62' */
    /*  RPY angles {W} */
    /* '<S15>:1:63' */
    /*  velocity in {W} */
    /* '<S15>:1:64' */
    /*  angular velocity in {W} (body pqr? (F)) */
    /* '<S15>:1:65' */
    /*  motor speed */
    /* PREPROCESS ROTATION AND WRONSKIAN MATRICIES */
    /* '<S15>:1:68' */
    /*  yaw */
    /* '<S15>:1:69' */
    /*  pitch */
    /* '<S15>:1:70' */
    /*  roll */
    /* if (z(3)>0) error('z greater than 0 (under ground)!'); end;   */
    /*  rotz(phi)*roty(the)*rotx(psi) */
    /* BBF > Inertial rotation matrix */
    /* '<S15>:1:78' */
    R[0] = cos(rtb_TmpSignalConversionAtSFunct[4]) * cos
      (rtb_TmpSignalConversionAtSFunct[3]);
    R[3] = sin(rtb_TmpSignalConversionAtSFunct[5]) * sin
      (rtb_TmpSignalConversionAtSFunct[4]) * cos
      (rtb_TmpSignalConversionAtSFunct[3]) - cos
      (rtb_TmpSignalConversionAtSFunct[5]) * sin
      (rtb_TmpSignalConversionAtSFunct[3]);
    R[6] = cos(rtb_TmpSignalConversionAtSFunct[5]) * sin
      (rtb_TmpSignalConversionAtSFunct[4]) * cos
      (rtb_TmpSignalConversionAtSFunct[3]) + sin
      (rtb_TmpSignalConversionAtSFunct[5]) * sin
      (rtb_TmpSignalConversionAtSFunct[3]);
    R[1] = cos(rtb_TmpSignalConversionAtSFunct[4]) * sin
      (rtb_TmpSignalConversionAtSFunct[3]);
    R[4] = sin(rtb_TmpSignalConversionAtSFunct[5]) * sin
      (rtb_TmpSignalConversionAtSFunct[4]) * sin
      (rtb_TmpSignalConversionAtSFunct[3]) + cos
      (rtb_TmpSignalConversionAtSFunct[5]) * cos
      (rtb_TmpSignalConversionAtSFunct[3]);
    R[7] = cos(rtb_TmpSignalConversionAtSFunct[5]) * sin
      (rtb_TmpSignalConversionAtSFunct[4]) * sin
      (rtb_TmpSignalConversionAtSFunct[3]) - sin
      (rtb_TmpSignalConversionAtSFunct[5]) * cos
      (rtb_TmpSignalConversionAtSFunct[3]);
    R[2] = -sin(rtb_TmpSignalConversionAtSFunct[4]);
    R[5] = sin(rtb_TmpSignalConversionAtSFunct[5]) * cos
      (rtb_TmpSignalConversionAtSFunct[4]);
    R[8] = cos(rtb_TmpSignalConversionAtSFunct[5]) * cos
      (rtb_TmpSignalConversionAtSFunct[4]);

    /* Manual Construction */
    /*      Q3 = [cos(phi) -sin(phi) 0;sin(phi) cos(phi) 0;0 0 1];   % RZ %Rotation mappings */
    /*      Q2 = [cos(the) 0 sin(the);0 1 0;-sin(the) 0 cos(the)];   % RY */
    /*      Q1 = [1 0 0;0 cos(psi) -sin(psi);0 sin(psi) cos(psi)];   % RX */
    /*      R = Q3*Q2*Q1    %Rotation matrix */
    /*  */
    /*     RZ * RY * RX */
    /* inverted Wronskian (body rates p-q-r to euler rates yaw pitch roll) */
    /* '<S15>:1:90' */
    rtb_acc_Word_idx_1 = cos(rtb_TmpSignalConversionAtSFunct[4]);

    /* ROTOR MODEL */
    /* '<S15>:1:95' */
    for (i = 0; i < 4; i++) {
      /* '<S15>:1:95' */
      /* for each rotor */
      /* Relative motion */
      /* '<S15>:1:98' */
      SimulatorDrone_inv(R, tmp_1);
      for (i_0 = 0; i_0 < 3; i_0++) {
        tmp_3[i_0] = tmp_1[i_0 + 6] * rtb_TmpSignalConversionAtSFunct[8] +
          (tmp_1[i_0 + 3] * rtb_TmpSignalConversionAtSFunct[7] + tmp_1[i_0] *
           rtb_TmpSignalConversionAtSFunct[6]);
      }

      rtb_u[0] = (D[i * 3 + 2] * rtb_TmpSignalConversionAtSFunct[10] - D[i * 3 +
                  1] * rtb_TmpSignalConversionAtSFunct[11]) + tmp_3[0];
      rtb_u[1] = (D[i * 3] * rtb_TmpSignalConversionAtSFunct[11] - D[i * 3 + 2] *
                  rtb_TmpSignalConversionAtSFunct[9]) + tmp_3[1];

      /* '<S15>:1:99' */
      rtb_alt_Gain = sqrt(rtb_u[0] * rtb_u[0] + rtb_u[1] * rtb_u[1]) / (fabs
        (rtb_TmpSignalConversionAtSFunct[i + 12]) * rtb_sincos_o1_idx_0);

      /* Magnitude of mu, planar components */
      /* '<S15>:1:100' */
      /* Non-dimensionalised normal inflow */
      /* Non-dimensionalised induced velocity approximation */
      /* '<S15>:1:103' */
      rtb_acc_Word_idx_2 = rt_atan2d_snf(rtb_u[1], rtb_u[0]);

      /* Sideslip azimuth relative to e1 (zero over nose) */
      /* '<S15>:1:104' */
      /* BBF > mu sideslip rotation matrix */
      /* Flapping */
      /* Longitudinal flapping */
      /* '<S15>:1:108' */
      /* sign(w) * (4/3)*((Ct/sigma)*(2*mu*gamma/3/a)/(1+3*e/2/r) + li)/(1+mu^2/2)]; %Lattitudinal flapping (note sign) */
      /* '<S15>:1:110' */
      rtb_alt_Gain = ((2.6666666666666665 * SimulatorDrone_P->quad.theta0 + 2.0 *
                       SimulatorDrone_P->quad.theta1) * rtb_alt_Gain - ((D[i * 3
        + 1] * rtb_TmpSignalConversionAtSFunct[9] - D[i * 3] *
        rtb_TmpSignalConversionAtSFunct[10]) + tmp_3[2]) / (fabs
        (rtb_TmpSignalConversionAtSFunct[i + 12]) * rtb_sincos_o1_idx_0) * 2.0 *
                      rtb_alt_Gain) / (1.0 - rtb_alt_Gain * rtb_alt_Gain / 2.0);

      /* Rotate the beta flapping angles to longitudinal and lateral coordinates. */
      /* '<S15>:1:111' */
      a1s[i] = (cos(rtb_acc_Word_idx_2) * rtb_alt_Gain + sin(rtb_acc_Word_idx_2)
                * 0.0) - 16.0 / rtb_sincos_o1_idx_1 / fabs
        (rtb_TmpSignalConversionAtSFunct[i + 12]) *
        rtb_TmpSignalConversionAtSFunct[10];

      /* '<S15>:1:112' */
      b1s[i] = (-sin(rtb_acc_Word_idx_2) * rtb_alt_Gain + cos(rtb_acc_Word_idx_2)
                * 0.0) - 16.0 / rtb_sincos_o1_idx_1 / fabs
        (rtb_TmpSignalConversionAtSFunct[i + 12]) *
        rtb_TmpSignalConversionAtSFunct[9];

      /* Forces and torques */
      /* '<S15>:1:115' */
      rtb_alt_Gain = SimulatorDrone_P->quad.Ct * rtb_sincos_o1_idx_2 *
        rtb_acc_Word_idx_0 * (rtb_sincos_o1_idx_0 * rtb_sincos_o1_idx_0) *
        (rtb_TmpSignalConversionAtSFunct[i + 12] *
         rtb_TmpSignalConversionAtSFunct[i + 12]);
      i_0 = 1 + i;
      rtb_RandomNumber[3 * (i_0 - 1)] = -cos(b1s[i]) * sin(a1s[i]) *
        rtb_alt_Gain;
      rtb_RandomNumber[1 + 3 * (i_0 - 1)] = rtb_alt_Gain * sin(b1s[i]);
      rtb_RandomNumber[2 + 3 * (i_0 - 1)] = -cos(a1s[i]) * cos(b1s[i]) *
        rtb_alt_Gain;

      /* Rotor thrust, linearised angle approximations */
      /* '<S15>:1:116' */
      rtb_alt_Gain = -SimulatorDrone_P->quad.Cq * rtb_sincos_o1_idx_2 *
        rtb_acc_Word_idx_0 * rt_powd_snf(rtb_sincos_o1_idx_0, 3.0) *
        rtb_TmpSignalConversionAtSFunct[i + 12] * fabs
        (rtb_TmpSignalConversionAtSFunct[i + 12]);
      i_0 = 1 + i;
      Q[3 * (i_0 - 1)] = rtb_alt_Gain * 0.0;
      Q[1 + 3 * (i_0 - 1)] = rtb_alt_Gain * 0.0;
      Q[2 + 3 * (i_0 - 1)] = rtb_alt_Gain;

      /* Rotor drag torque - note that this preserves w(i) direction sign */
      /* tau(:,i) = cross(T(:,i),D(:,i));                %Torque due to rotor thrust */
      /* '<S15>:1:118' */
      tau[3 * i] = D[i * 3 + 1] * rtb_RandomNumber[i * 3 + 2] - D[i * 3 + 2] *
        rtb_RandomNumber[i * 3 + 1];
      tau[1 + 3 * i] = D[i * 3 + 2] * rtb_RandomNumber[i * 3] -
        rtb_RandomNumber[i * 3 + 2] * D[i * 3];
      tau[2 + 3 * i] = rtb_RandomNumber[i * 3 + 1] * D[i * 3] - D[i * 3 + 1] *
        rtb_RandomNumber[i * 3];

      /* changed sign (F) */
      /* '<S15>:1:95' */
    }

    /* RIGID BODY DYNAMIC MODEL */
    /* '<S15>:1:123' */
    /* '<S15>:1:125' */
    rtb_alt_Gain = 1.0 / SimulatorDrone_P->quad.M;

    /* '<S15>:1:126' */
    for (i_0 = 0; i_0 < 3; i_0++) {
      rtb_u[i_0] = SimulatorDrone_P->quad.J[i_0 + 6] *
        rtb_TmpSignalConversionAtSFunct[11] + (SimulatorDrone_P->quad.J[i_0 + 3]
        * rtb_TmpSignalConversionAtSFunct[10] + SimulatorDrone_P->quad.J[i_0] *
        rtb_TmpSignalConversionAtSFunct[9]);
    }

    /* row sum of torques */
    /* '<S15>:1:128' */
    rtb_acc_Word_idx_2 = 1.0 / SimulatorDrone_P->quadEDT.motorTimeConstant;

    /* '<S15>:1:130' */
    /* This is the state derivative vector */
    /*  End of mdlDerivatives. */
    SimulatorDrone_sum(rtb_RandomNumber, tmp_3);
    SimulatorDrone_inv(SimulatorDrone_P->quad.J, tmp_1);
    SimulatorDrone_sum(tau, tmp);
    SimulatorDrone_sum(Q, tmp_0);
    tmp_6[0] = 0.0;
    tmp_6[3] = sin(rtb_TmpSignalConversionAtSFunct[5]);
    tmp_6[6] = cos(rtb_TmpSignalConversionAtSFunct[5]);
    tmp_6[1] = 0.0;
    tmp_6[4] = cos(rtb_TmpSignalConversionAtSFunct[5]) * cos
      (rtb_TmpSignalConversionAtSFunct[4]);
    tmp_6[7] = -sin(rtb_TmpSignalConversionAtSFunct[5]) * cos
      (rtb_TmpSignalConversionAtSFunct[4]);
    tmp_6[2] = cos(rtb_TmpSignalConversionAtSFunct[4]);
    tmp_6[5] = sin(rtb_TmpSignalConversionAtSFunct[5]) * sin
      (rtb_TmpSignalConversionAtSFunct[4]);
    tmp_6[8] = cos(rtb_TmpSignalConversionAtSFunct[5]) * sin
      (rtb_TmpSignalConversionAtSFunct[4]);
    for (i_0 = 0; i_0 < 3; i_0++) {
      tmp_2[3 * i_0] = tmp_6[3 * i_0] / rtb_acc_Word_idx_1;
      tmp_2[1 + 3 * i_0] = tmp_6[3 * i_0 + 1] / rtb_acc_Word_idx_1;
      tmp_2[2 + 3 * i_0] = tmp_6[3 * i_0 + 2] / rtb_acc_Word_idx_1;
    }

    rtb_sincos_o1_idx_0 = (-(rtb_TmpSignalConversionAtSFunct[10] * rtb_u[2] -
      rtb_TmpSignalConversionAtSFunct[11] * rtb_u[1]) + tmp[0]) + tmp_0[0];
    rtb_sincos_o1_idx_1 = (-(rtb_TmpSignalConversionAtSFunct[11] * rtb_u[0] -
      rtb_TmpSignalConversionAtSFunct[9] * rtb_u[2]) + tmp[1]) + tmp_0[1];
    rtb_sincos_o1_idx_2 = (-(rtb_TmpSignalConversionAtSFunct[9] * rtb_u[1] -
      rtb_TmpSignalConversionAtSFunct[10] * rtb_u[0]) + tmp[2]) + tmp_0[2];
    for (i_0 = 0; i_0 < 3; i_0++) {
      tmp[i_0] = tmp_2[i_0 + 6] * rtb_TmpSignalConversionAtSFunct[11] +
        (tmp_2[i_0 + 3] * rtb_TmpSignalConversionAtSFunct[10] + tmp_2[i_0] *
         rtb_TmpSignalConversionAtSFunct[9]);
    }

    for (i_0 = 0; i_0 < 3; i_0++) {
      tmp_0[i_0] = ((R[i_0 + 3] * rtb_alt_Gain * tmp_3[1] + R[i_0] *
                     rtb_alt_Gain * tmp_3[0]) + R[i_0 + 6] * rtb_alt_Gain *
                    tmp_3[2]) + SimulatorDrone_P->quad.g * (real_T)b[i_0];
    }

    for (i_0 = 0; i_0 < 3; i_0++) {
      tmp_3[i_0] = tmp_1[i_0 + 6] * rtb_sincos_o1_idx_2 + (tmp_1[i_0 + 3] *
        rtb_sincos_o1_idx_1 + tmp_1[i_0] * rtb_sincos_o1_idx_0);
    }

    SimulatorDrone_B->dx[0] = rtb_TmpSignalConversionAtSFunct[6];
    SimulatorDrone_B->dx[1] = rtb_TmpSignalConversionAtSFunct[7];
    SimulatorDrone_B->dx[2] = rtb_TmpSignalConversionAtSFunct[8];
    SimulatorDrone_B->dx[3] = tmp[0];
    SimulatorDrone_B->dx[4] = tmp[1];
    SimulatorDrone_B->dx[5] = tmp[2];
    SimulatorDrone_B->dx[6] = tmp_0[0];
    SimulatorDrone_B->dx[7] = tmp_0[1];
    SimulatorDrone_B->dx[8] = tmp_0[2];
    SimulatorDrone_B->dx[9] = tmp_3[0];
    SimulatorDrone_B->dx[10] = tmp_3[1];
    SimulatorDrone_B->dx[11] = tmp_3[2];
    if (rtb_y_idx_0 > SimulatorDrone_P->quadEDT.maxomega) {
      rtb_y_idx_0 = SimulatorDrone_P->quadEDT.maxomega;
    } else {
      if (rtb_y_idx_0 < SimulatorDrone_P->omegaSaturation_LowerSat) {
        rtb_y_idx_0 = SimulatorDrone_P->omegaSaturation_LowerSat;
      }
    }

    SimulatorDrone_B->dx[12] = (SimulatorDrone_P->Directions_Gain[0] *
      rtb_y_idx_0 - SimulatorDrone_DW->IntMotors_DSTATE[0]) * rtb_acc_Word_idx_2;
    if (rtb_y_idx_1 > SimulatorDrone_P->quadEDT.maxomega) {
      rtb_y_idx_1 = SimulatorDrone_P->quadEDT.maxomega;
    } else {
      if (rtb_y_idx_1 < SimulatorDrone_P->omegaSaturation_LowerSat) {
        rtb_y_idx_1 = SimulatorDrone_P->omegaSaturation_LowerSat;
      }
    }

    SimulatorDrone_B->dx[13] = (SimulatorDrone_P->Directions_Gain[1] *
      rtb_y_idx_1 - SimulatorDrone_DW->IntMotors_DSTATE[1]) * rtb_acc_Word_idx_2;
    if (rtb_y_idx_2 > SimulatorDrone_P->quadEDT.maxomega) {
      rtb_y_idx_2 = SimulatorDrone_P->quadEDT.maxomega;
    } else {
      if (rtb_y_idx_2 < SimulatorDrone_P->omegaSaturation_LowerSat) {
        rtb_y_idx_2 = SimulatorDrone_P->omegaSaturation_LowerSat;
      }
    }

    SimulatorDrone_B->dx[14] = (SimulatorDrone_P->Directions_Gain[2] *
      rtb_y_idx_2 - SimulatorDrone_DW->IntMotors_DSTATE[2]) * rtb_acc_Word_idx_2;
    if (rtb_y > SimulatorDrone_P->quadEDT.maxomega) {
      rtb_y = SimulatorDrone_P->quadEDT.maxomega;
    } else {
      if (rtb_y < SimulatorDrone_P->omegaSaturation_LowerSat) {
        rtb_y = SimulatorDrone_P->omegaSaturation_LowerSat;
      }
    }

    SimulatorDrone_B->dx[15] = (SimulatorDrone_P->Directions_Gain[3] * rtb_y -
      SimulatorDrone_DW->IntMotors_DSTATE[3]) * rtb_acc_Word_idx_2;
  }

  {
    real_T (*lastU)[3];
    int32_T i;
    for (i = 0; i < 12; i++) {
      /* Update for Delay: '<S11>/Delay3' */
      SimulatorDrone_DW->Delay3_DSTATE[i] = SimulatorDrone_B->dx[i];

      /* Update for Delay: '<S11>/Delay1' */
      SimulatorDrone_DW->Delay1_DSTATE[i] = SimulatorDrone_B->IntPosestates[i];
    }

    /* Update for DiscreteIntegrator: '<S11>/IntPosestates' */
    if (!rtb_LogicalOperator) {
      for (i = 0; i < 12; i++) {
        SimulatorDrone_DW->IntPosestates_DSTATE[i] +=
          SimulatorDrone_P->IntPosestates_gainval * SimulatorDrone_B->dx[i];
      }
    }

    SimulatorDrone_DW->IntPosestates_PrevResetState = (int8_T)
      rtb_LogicalOperator;

    /* End of Update for DiscreteIntegrator: '<S11>/IntPosestates' */

    /* Update for DiscreteIntegrator: '<S11>/IntMotors' */
    SimulatorDrone_DW->IntMotors_DSTATE[0] +=
      SimulatorDrone_P->IntMotors_gainval * SimulatorDrone_B->dx[12];
    SimulatorDrone_DW->IntMotors_DSTATE[1] +=
      SimulatorDrone_P->IntMotors_gainval * SimulatorDrone_B->dx[13];
    SimulatorDrone_DW->IntMotors_DSTATE[2] +=
      SimulatorDrone_P->IntMotors_gainval * SimulatorDrone_B->dx[14];
    SimulatorDrone_DW->IntMotors_DSTATE[3] +=
      SimulatorDrone_P->IntMotors_gainval * SimulatorDrone_B->dx[15];

    /* Update for Delay: '<S5>/simualte random usbdelays' */
    memcpy(&SimulatorDrone_DW->simualterandomusbdelays_DSTATE[0],
           &rtb_simulatedsensorupdateratede[0], sizeof(real_T) << 3U);

    /* Update for Delay: '<S1>/Delay1' incorporates:
     *  Update for Inport: '<Root>/motorsPWM_datin'
     */
    SimulatorDrone_DW->Delay1_DSTATE_j[0] = SimulatorDrone_U_y[0];
    SimulatorDrone_DW->Delay1_DSTATE_j[1] = SimulatorDrone_U_y[1];
    SimulatorDrone_DW->Delay1_DSTATE_j[2] = SimulatorDrone_U_y[2];
    SimulatorDrone_DW->Delay1_DSTATE_j[3] = SimulatorDrone_U_y[3];

    /* Update for RandomNumber: '<S10>/Random Number' */
    for (i = 0; i < 12; i++) {
      SimulatorDrone_DW->NextOutput[i] = rt_nrand_Upu32_Yd_f_pw_snf
        (&SimulatorDrone_DW->RandSeed[i]) *
        SimulatorDrone_P->RandomNumber_StdDev +
        SimulatorDrone_P->RandomNumber_Mean;
    }

    /* End of Update for RandomNumber: '<S10>/Random Number' */

    /* Update for Derivative: '<S5>/Derivative' */
    if (SimulatorDrone_DW->TimeStampA == (rtInf)) {
      SimulatorDrone_DW->TimeStampA = SimulatorDrone_M->Timing.t[0];
      lastU = (real_T (*)[3])SimulatorDrone_DW->LastUAtTimeA;
    } else if (SimulatorDrone_DW->TimeStampB == (rtInf)) {
      SimulatorDrone_DW->TimeStampB = SimulatorDrone_M->Timing.t[0];
      lastU = (real_T (*)[3])SimulatorDrone_DW->LastUAtTimeB;
    } else if (SimulatorDrone_DW->TimeStampA < SimulatorDrone_DW->TimeStampB) {
      SimulatorDrone_DW->TimeStampA = SimulatorDrone_M->Timing.t[0];
      lastU = (real_T (*)[3])SimulatorDrone_DW->LastUAtTimeA;
    } else {
      SimulatorDrone_DW->TimeStampB = SimulatorDrone_M->Timing.t[0];
      lastU = (real_T (*)[3])SimulatorDrone_DW->LastUAtTimeB;
    }

    (*lastU)[0] = SimulatorDrone_B->y[0];
    (*lastU)[1] = SimulatorDrone_B->y[1];
    (*lastU)[2] = SimulatorDrone_B->y[2];

    /* End of Update for Derivative: '<S5>/Derivative' */

    /* Update for Delay: '<S5>/simulated sensorupdaterate delays' */
    memcpy(&SimulatorDrone_DW->simulatedsensorupdateratedelays[0],
           &rtb_TmpSignalConversionAtsimula[0], sizeof(real_T) << 3U);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  SimulatorDrone_M->Timing.t[0] =
    (++SimulatorDrone_M->Timing.clockTick0) * SimulatorDrone_M->Timing.stepSize0;

  {
    /* Update absolute timer for sample time: [0.005s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The resolution of this integer timer is 0.005, which is the step size
     * of the task. Size of "clockTick1" ensures timer will not overflow during the
     * application lifespan selected.
     */
    SimulatorDrone_M->Timing.clockTick1++;
  }
}

/* Model initialize function */
void SimulatorDrone_initialize(RT_MODEL_SimulatorDrone_T *const SimulatorDrone_M,
  real_T SimulatorDrone_U_y[4], real_T *SimulatorDrone_Y_X, real_T
  *SimulatorDrone_Y_Y, real_T *SimulatorDrone_Y_Z, real_T *SimulatorDrone_Y_dx,
  real_T *SimulatorDrone_Y_dy, real_T *SimulatorDrone_Y_dz, real_T
  *SimulatorDrone_Y_yaw, real_T *SimulatorDrone_Y_pitch, real_T
  *SimulatorDrone_Y_roll, real_T SimulatorDrone_Y_quaternions_datout[4], real_T *
  SimulatorDrone_Y_p, real_T *SimulatorDrone_Y_q, real_T *SimulatorDrone_Y_r,
  real_T SimulatorDrone_Y_opticalFlow_datout[3], real_T
  SimulatorDrone_Y_sensordataCalib_daout[7], real_T *SimulatorDrone_Y_ddx,
  real_T *SimulatorDrone_Y_ddy, real_T *SimulatorDrone_Y_ddz, real_T
  *SimulatorDrone_Y_pb, real_T *SimulatorDrone_Y_qb, real_T *SimulatorDrone_Y_rb,
  real_T *SimulatorDrone_Y_altitude_sonar, real_T *SimulatorDrone_Y_prs)
{
  P_SimulatorDrone_T *SimulatorDrone_P = ((P_SimulatorDrone_T *)
    SimulatorDrone_M->ModelData.defaultParam);
  DW_SimulatorDrone_T *SimulatorDrone_DW = ((DW_SimulatorDrone_T *)
    SimulatorDrone_M->ModelData.dwork);
  B_SimulatorDrone_T *SimulatorDrone_B = ((B_SimulatorDrone_T *)
    SimulatorDrone_M->ModelData.blockIO);

  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  SimulatorDrone_P->Saturation_UpperSat = rtInf;

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&SimulatorDrone_M->solverInfo,
                          &SimulatorDrone_M->Timing.simTimeStep);
    rtsiSetTPtr(&SimulatorDrone_M->solverInfo, &rtmGetTPtr(SimulatorDrone_M));
    rtsiSetStepSizePtr(&SimulatorDrone_M->solverInfo,
                       &SimulatorDrone_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&SimulatorDrone_M->solverInfo, ((const char_T **)
      (&rtmGetErrorStatus(SimulatorDrone_M))));
    rtsiSetRTModelPtr(&SimulatorDrone_M->solverInfo, SimulatorDrone_M);
  }

  rtsiSetSimTimeStep(&SimulatorDrone_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&SimulatorDrone_M->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr(SimulatorDrone_M, &SimulatorDrone_M->Timing.tArray[0]);
  SimulatorDrone_M->Timing.stepSize0 = 0.005;

  /* block I/O */
  (void) memset(((void *) SimulatorDrone_B), 0,
                sizeof(B_SimulatorDrone_T));

  /* states (dwork) */
  (void) memset((void *)SimulatorDrone_DW, 0,
                sizeof(DW_SimulatorDrone_T));

  /* external inputs */
  (void) memset(SimulatorDrone_U_y, 0,
                4U*sizeof(real_T));

  /* external outputs */
  (*SimulatorDrone_Y_X) = 0.0;
  (*SimulatorDrone_Y_Y) = 0.0;
  (*SimulatorDrone_Y_Z) = 0.0;
  (*SimulatorDrone_Y_dx) = 0.0;
  (*SimulatorDrone_Y_dy) = 0.0;
  (*SimulatorDrone_Y_dz) = 0.0;
  (*SimulatorDrone_Y_yaw) = 0.0;
  (*SimulatorDrone_Y_pitch) = 0.0;
  (*SimulatorDrone_Y_roll) = 0.0;
  (void) memset(&SimulatorDrone_Y_quaternions_datout[0], 0,
                4U*sizeof(real_T));
  (*SimulatorDrone_Y_p) = 0.0;
  (*SimulatorDrone_Y_q) = 0.0;
  (*SimulatorDrone_Y_r) = 0.0;
  (void) memset(&SimulatorDrone_Y_opticalFlow_datout[0], 0,
                3U*sizeof(real_T));
  (void) memset(&SimulatorDrone_Y_sensordataCalib_daout[0], 0,
                7U*sizeof(real_T));
  (*SimulatorDrone_Y_ddx) = 0.0;
  (*SimulatorDrone_Y_ddy) = 0.0;
  (*SimulatorDrone_Y_ddz) = 0.0;
  (*SimulatorDrone_Y_pb) = 0.0;
  (*SimulatorDrone_Y_qb) = 0.0;
  (*SimulatorDrone_Y_rb) = 0.0;
  (*SimulatorDrone_Y_altitude_sonar) = 0.0;
  (*SimulatorDrone_Y_prs) = 0.0;

  {
    uint32_T tseed;
    int32_T r;
    int32_T t;
    real_T y1;
    int32_T i;
    for (i = 0; i < 12; i++) {
      /* InitializeConditions for Delay: '<S11>/Delay3' */
      SimulatorDrone_DW->Delay3_DSTATE[i] =
        SimulatorDrone_P->Delay3_InitialCondition;

      /* InitializeConditions for Delay: '<S11>/Delay1' */
      SimulatorDrone_DW->Delay1_DSTATE[i] =
        SimulatorDrone_P->Delay1_InitialCondition[i];

      /* InitializeConditions for DiscreteIntegrator: '<S11>/IntPosestates' incorporates:
       *  Start for Constant: '<S11>/Constant14'
       */
      SimulatorDrone_DW->IntPosestates_DSTATE[i] =
        SimulatorDrone_P->Constant14_Value[i];
    }

    /* InitializeConditions for DiscreteIntegrator: '<S11>/IntPosestates' */
    SimulatorDrone_DW->IntPosestates_PrevResetState = 0;

    /* InitializeConditions for DiscreteIntegrator: '<S11>/IntMotors' */
    SimulatorDrone_DW->IntMotors_DSTATE[0] = SimulatorDrone_P->IntMotors_IC[0];
    SimulatorDrone_DW->IntMotors_DSTATE[1] = SimulatorDrone_P->IntMotors_IC[1];
    SimulatorDrone_DW->IntMotors_DSTATE[2] = SimulatorDrone_P->IntMotors_IC[2];
    SimulatorDrone_DW->IntMotors_DSTATE[3] = SimulatorDrone_P->IntMotors_IC[3];

    /* InitializeConditions for Delay: '<S5>/simualte random usbdelays' */
    memcpy(&SimulatorDrone_DW->simualterandomusbdelays_DSTATE[0],
           &SimulatorDrone_P->simualterandomusbdelays_Initial[0], sizeof(real_T)
           << 3U);

    /* InitializeConditions for Delay: '<S1>/Delay1' */
    SimulatorDrone_DW->Delay1_DSTATE_j[0] = SimulatorDrone_P->quadEDT.comega2pwm;
    SimulatorDrone_DW->Delay1_DSTATE_j[1] = SimulatorDrone_P->quadEDT.comega2pwm;
    SimulatorDrone_DW->Delay1_DSTATE_j[2] = SimulatorDrone_P->quadEDT.comega2pwm;
    SimulatorDrone_DW->Delay1_DSTATE_j[3] = SimulatorDrone_P->quadEDT.comega2pwm;

    /* InitializeConditions for RandomNumber: '<S10>/Random Number' */
    for (i = 0; i < 12; i++) {
      y1 = floor(SimulatorDrone_P->RandomNumber_Seed[i]);
      if (rtIsNaN(y1) || rtIsInf(y1)) {
        y1 = 0.0;
      } else {
        y1 = fmod(y1, 4.294967296E+9);
      }

      tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
      r = (int32_T)(tseed >> 16U);
      t = (int32_T)(tseed & 32768U);
      tseed = ((((tseed - ((uint32_T)r << 16U)) + t) << 16U) + t) + r;
      if (tseed < 1U) {
        tseed = 1144108930U;
      } else {
        if (tseed > 2147483646U) {
          tseed = 2147483646U;
        }
      }

      y1 = rt_nrand_Upu32_Yd_f_pw_snf(&tseed) *
        SimulatorDrone_P->RandomNumber_StdDev +
        SimulatorDrone_P->RandomNumber_Mean;
      SimulatorDrone_DW->NextOutput[i] = y1;
      SimulatorDrone_DW->RandSeed[i] = tseed;
    }

    /* End of InitializeConditions for RandomNumber: '<S10>/Random Number' */

    /* InitializeConditions for Derivative: '<S5>/Derivative' */
    SimulatorDrone_DW->TimeStampA = (rtInf);
    SimulatorDrone_DW->TimeStampB = (rtInf);

    /* InitializeConditions for Delay: '<S5>/simulated sensorupdaterate delays' */
    memcpy(&SimulatorDrone_DW->simulatedsensorupdateratedelays[0],
           &SimulatorDrone_P->simulatedsensorupdateratedelays[0], sizeof(real_T)
           << 3U);
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
