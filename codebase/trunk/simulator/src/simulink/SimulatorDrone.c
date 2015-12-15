/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: SimulatorDrone.c
 *
 * Code generated for Simulink model 'SimulatorDrone'.
 *
 * Model version                  : 1.3154
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Tue Dec 15 16:40:04 2015
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
  real_T SimulatorDrone_U_motorsPWM_datin[4], real_T *SimulatorDrone_Y_X, real_T
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
  real_T rtb_TmpSignalConversionAtDelay2[8];
  boolean_T rtb_LogicalOperator;

  {
    real_T (*lastU)[3];
    real_T a1s[4];
    real_T b1s[4];
    real_T tau[12];
    real_T D[12];
    real_T R[9];
    static const int8_T b[3] = { 0, 0, 1 };

    real_T rtb_RandomNumber[12];
    boolean_T rtb_Compare_a;
    real_T rtb_u[3];
    real_T rtb_Product;
    real_T rtb_Add_p[8];
    real_T rtb_y[4];
    real_T Product[12];
    int32_T i;
    real_T tmp[3];
    real_T tmp_0[9];
    real_T tmp_1[9];
    real_T tmp_2[3];
    int32_T i_0;
    real_T tmp_3[9];
    real_T tmp_4[9];
    real_T tmp_5[3];
    real_T tmp_6[9];
    real_T rtb_sincos_o1_idx_0;
    real_T rtb_acc_Word_idx_0;
    real_T rtb_sincos_o1_idx_1;
    real_T rtb_acc_Word_idx_1;
    real_T rtb_sincos_o1_idx_2;
    real_T rtb_acc_Word_idx_2;
    real_T tmp_7;

    /* Clock: '<S11>/Clock' */
    rtb_Clock = SimulatorDrone_M->Timing.t[0];

    /* Delay: '<S11>/Delay3' */
    memcpy(&rtb_RandomNumber[0], &SimulatorDrone_DW->Delay3_DSTATE[0], 12U *
           sizeof(real_T));

    /* Logic: '<S11>/Logical Operator' incorporates:
     *  Constant: '<S13>/Constant'
     *  Constant: '<S14>/Constant'
     *  Delay: '<S11>/Delay2'
     *  RelationalOperator: '<S13>/Compare'
     *  RelationalOperator: '<S14>/Compare'
     */
    rtb_LogicalOperator = ((rtb_RandomNumber[8] >=
      SimulatorDrone_P->CompareToConstant1_const) &&
      (SimulatorDrone_DW->Delay2_DSTATE >=
       SimulatorDrone_P->CompareToConstant_const));

    /* RelationalOperator: '<S15>/Compare' incorporates:
     *  Constant: '<S15>/Constant'
     */
    rtb_Compare_a = (rtb_Clock <= SimulatorDrone_P->quadEDT.sampletime);

    /* Product: '<S11>/Product' incorporates:
     *  Constant: '<S11>/Constant14'
     */
    for (i = 0; i < 12; i++) {
      Product[i] = SimulatorDrone_P->Constant14_Value[i] * (real_T)rtb_Compare_a;
    }

    /* End of Product: '<S11>/Product' */

    /* DiscreteIntegrator: '<S11>/Discrete-Time Integrator' */
    if (SimulatorDrone_DW->DiscreteTimeIntegrator_IC_LOADI != 0) {
      memcpy(&SimulatorDrone_DW->DiscreteTimeIntegrator_DSTATE[0], &Product[0],
             12U * sizeof(real_T));
    }

    for (i = 0; i < 12; i++) {
      if (rtb_LogicalOperator ||
          (SimulatorDrone_DW->DiscreteTimeIntegrator_PrevRese != 0)) {
        SimulatorDrone_DW->DiscreteTimeIntegrator_DSTATE[i] = Product[i];
      }

      SimulatorDrone_B->DiscreteTimeIntegrator[i] =
        SimulatorDrone_DW->DiscreteTimeIntegrator_DSTATE[i];
    }

    /* End of DiscreteIntegrator: '<S11>/Discrete-Time Integrator' */

    /* MATLAB Function: '<S11>/statetoout' */
    /* MATLAB Function 'SimulatorDrone/Drone_Dynamics/Dynamics/statetoout': '<S17>:1' */
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
    /* '<S17>:1:46' */
    /*  RPY angles */
    /* '<S17>:1:47' */
    /*  yaw */
    /* '<S17>:1:48' */
    /*  pitch */
    /* '<S17>:1:49' */
    /*  roll */
    /*  rotz(phi)*roty(the)*rotx(psi) */
    /* BBF > Inertial rotation matrix */
    /* '<S17>:1:53' */
    /* inverted Wronskian */
    /*  return velocity in the body frame */
    /* output global pos and euler angles */
    /* translational velocity mapped to body frame -> i.e. output v in bodyframe! (F) */
    /* '<S17>:1:62' */
    /* iW*x(10:12)];    % RPY rates mapped to body frame ->     i.e. : this outputed euler rates!, now in bodyrates pqr (F) */
    /* sys = [x(1:6); iW*x(7:9);  iW*x(10:12)]; */
    /* sys = x; */
    /*  End of mdlOutputs. */
    tmp_0[0] = cos(SimulatorDrone_B->DiscreteTimeIntegrator[4]) * cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]);
    tmp_0[3] = sin(SimulatorDrone_B->DiscreteTimeIntegrator[5]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[4]) * cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]) - cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[5]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]);
    tmp_0[6] = cos(SimulatorDrone_B->DiscreteTimeIntegrator[5]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[4]) * cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]) + sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[5]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]);
    tmp_0[1] = cos(SimulatorDrone_B->DiscreteTimeIntegrator[4]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]);
    tmp_0[4] = sin(SimulatorDrone_B->DiscreteTimeIntegrator[5]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[4]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]) + cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[5]) * cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]);
    tmp_0[7] = cos(SimulatorDrone_B->DiscreteTimeIntegrator[5]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[4]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]) - sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[5]) * cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]);
    tmp_0[2] = -sin(SimulatorDrone_B->DiscreteTimeIntegrator[4]);
    tmp_0[5] = sin(SimulatorDrone_B->DiscreteTimeIntegrator[5]) * cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[4]);
    tmp_0[8] = cos(SimulatorDrone_B->DiscreteTimeIntegrator[5]) * cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[4]);
    SimulatorDrone_inv_a(tmp_0, tmp_1);
    for (i_0 = 0; i_0 < 3; i_0++) {
      tmp_2[i_0] = tmp_1[i_0 + 6] * SimulatorDrone_B->DiscreteTimeIntegrator[8]
        + (tmp_1[i_0 + 3] * SimulatorDrone_B->DiscreteTimeIntegrator[7] +
           tmp_1[i_0] * SimulatorDrone_B->DiscreteTimeIntegrator[6]);
    }

    for (i_0 = 0; i_0 < 6; i_0++) {
      Product[i_0] = SimulatorDrone_B->DiscreteTimeIntegrator[i_0];
    }

    Product[6] = tmp_2[0];
    Product[7] = tmp_2[1];
    Product[8] = tmp_2[2];
    Product[9] = SimulatorDrone_B->DiscreteTimeIntegrator[9];
    Product[10] = SimulatorDrone_B->DiscreteTimeIntegrator[10];
    Product[11] = SimulatorDrone_B->DiscreteTimeIntegrator[11];

    /* Outport: '<Root>/X' */
    *SimulatorDrone_Y_X = Product[0];

    /* Outport: '<Root>/Y' */
    *SimulatorDrone_Y_Y = Product[1];

    /* Outport: '<Root>/Z' */
    *SimulatorDrone_Y_Z = Product[2];

    /* Outport: '<Root>/dx' incorporates:
     *  MATLAB Function: '<S11>/statetoout'
     */
    *SimulatorDrone_Y_dx = tmp_2[0];

    /* Outport: '<Root>/dy' incorporates:
     *  MATLAB Function: '<S11>/statetoout'
     */
    *SimulatorDrone_Y_dy = tmp_2[1];

    /* Outport: '<Root>/dz' incorporates:
     *  MATLAB Function: '<S11>/statetoout'
     */
    *SimulatorDrone_Y_dz = tmp_2[2];

    /* Outport: '<Root>/yaw' */
    *SimulatorDrone_Y_yaw = Product[3];

    /* Outport: '<Root>/pitch' */
    *SimulatorDrone_Y_pitch = Product[4];

    /* Outport: '<Root>/roll' */
    *SimulatorDrone_Y_roll = Product[5];

    /* Gain: '<S4>/1//2' */
    rtb_u[0] = SimulatorDrone_P->u_Gain * Product[3];
    rtb_u[1] = SimulatorDrone_P->u_Gain * Product[4];
    rtb_u[2] = SimulatorDrone_P->u_Gain * Product[5];

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
    *SimulatorDrone_Y_p = SimulatorDrone_B->DiscreteTimeIntegrator[9];

    /* Outport: '<Root>/q' incorporates:
     *  MATLAB Function: '<S11>/statetoout'
     */
    *SimulatorDrone_Y_q = SimulatorDrone_B->DiscreteTimeIntegrator[10];

    /* Outport: '<Root>/r' incorporates:
     *  MATLAB Function: '<S11>/statetoout'
     */
    *SimulatorDrone_Y_r = SimulatorDrone_B->DiscreteTimeIntegrator[11];

    /* Outport: '<Root>/opticalFlow_datout' incorporates:
     *  Delay: '<S6>/Delay3'
     */
    SimulatorDrone_Y_opticalFlow_datout[0] = SimulatorDrone_DW->Delay3_DSTATE_b
      [0];
    SimulatorDrone_Y_opticalFlow_datout[1] = SimulatorDrone_DW->Delay3_DSTATE_b
      [1];
    SimulatorDrone_Y_opticalFlow_datout[2] = SimulatorDrone_DW->Delay3_DSTATE_b
      [2];

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
     *  Delay: '<S5>/Delay2'
     */
    for (i = 0; i < 6; i++) {
      rtb_Add_p[i] = (SimulatorDrone_P->quadEDT.sensordataCalib[i] +
                      SimulatorDrone_P->Assumestakeoffwaslevel_Bias[i]) +
        SimulatorDrone_DW->Delay2_DSTATE_m[i];
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
     *  Delay: '<S5>/Delay2'
     *  Sum: '<S5>/Add'
     */
    *SimulatorDrone_Y_altitude_sonar = (SimulatorDrone_P->zeroconst_Value +
      SimulatorDrone_P->Assumestakeoffwaslevel_Bias[6]) +
      SimulatorDrone_DW->Delay2_DSTATE_m[6];

    /* Outport: '<Root>/prs' incorporates:
     *  Bias: '<S5>/Assumes takeoff was level'
     *  Constant: '<S1>/sensordataCalib'
     *  Delay: '<S5>/Delay2'
     *  Sum: '<S5>/Add'
     */
    *SimulatorDrone_Y_prs = (SimulatorDrone_P->quadEDT.sensordataCalib[6] +
      SimulatorDrone_P->Assumestakeoffwaslevel_Bias[7]) +
      SimulatorDrone_DW->Delay2_DSTATE_m[7];

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

      tau[i_0] = rtb_sincos_o1_idx_0 +
        SimulatorDrone_P->GaussianNoiseGenerator_m;
    }

    /* End of Sum: '<S10>/Sum' */

    /* Sum: '<S7>/Add' incorporates:
     *  Gain: '<S7>/noiseWeights'
     */
    for (i_0 = 0; i_0 < 12; i_0++) {
      rtb_RandomNumber[i_0] =
        SimulatorDrone_P->quadEDT.noiseStatesSensed_weights[i_0] * tau[i_0] +
        Product[i_0];
    }

    /* End of Sum: '<S7>/Add' */

    /* MATLAB Function: '<S5>/trafo_Body2World_trans' */
    /* MATLAB Function 'SimulatorDrone/DroneRS_Sensorsystem/IMU_Pressure/trafo_Body2World_trans': '<S8>:1' */
    /* '<S8>:1:3' */
    /* '<S8>:1:4' */
    /* '<S8>:1:5' */
    /* '<S8>:1:7' */
    /* '<S8>:1:11' */
    tmp_3[0] = cos(Product[4]) * cos(Product[3]);
    tmp_3[3] = cos(Product[3]) * sin(Product[4]) * sin(Product[5]) - cos
      (Product[5]) * sin(Product[3]);
    tmp_3[6] = cos(Product[5]) * cos(Product[3]) * sin(Product[4]) + sin
      (Product[5]) * sin(Product[3]);
    tmp_3[1] = cos(Product[4]) * sin(Product[3]);
    tmp_3[4] = sin(Product[4]) * sin(Product[5]) * sin(Product[3]) + cos
      (Product[5]) * cos(Product[3]);
    tmp_3[7] = cos(Product[5]) * sin(Product[4]) * sin(Product[3]) - cos
      (Product[3]) * sin(Product[5]);
    tmp_3[2] = -sin(Product[4]);
    tmp_3[5] = cos(Product[4]) * sin(Product[5]);
    tmp_3[8] = cos(Product[4]) * cos(Product[5]);
    for (i_0 = 0; i_0 < 3; i_0++) {
      SimulatorDrone_B->y[i_0] = 0.0;
      SimulatorDrone_B->y[i_0] += tmp_3[i_0] * rtb_RandomNumber[6];
      SimulatorDrone_B->y[i_0] += tmp_3[i_0 + 3] * rtb_RandomNumber[7];
      SimulatorDrone_B->y[i_0] += tmp_3[i_0 + 6] * rtb_RandomNumber[8];
    }

    /* End of MATLAB Function: '<S5>/trafo_Body2World_trans' */

    /* Derivative: '<S5>/Derivative' */
    if ((SimulatorDrone_DW->TimeStampA >= SimulatorDrone_M->Timing.t[0]) &&
        (SimulatorDrone_DW->TimeStampB >= SimulatorDrone_M->Timing.t[0])) {
      rtb_acc_Word_idx_0 = 0.0;
      rtb_acc_Word_idx_1 = 0.0;
      rtb_acc_Word_idx_2 = 0.0;
    } else {
      rtb_Product = SimulatorDrone_DW->TimeStampA;
      lastU = (real_T (*)[3])SimulatorDrone_DW->LastUAtTimeA;
      if (SimulatorDrone_DW->TimeStampA < SimulatorDrone_DW->TimeStampB) {
        if (SimulatorDrone_DW->TimeStampB < SimulatorDrone_M->Timing.t[0]) {
          rtb_Product = SimulatorDrone_DW->TimeStampB;
          lastU = (real_T (*)[3])SimulatorDrone_DW->LastUAtTimeB;
        }
      } else {
        if (SimulatorDrone_DW->TimeStampA >= SimulatorDrone_M->Timing.t[0]) {
          rtb_Product = SimulatorDrone_DW->TimeStampB;
          lastU = (real_T (*)[3])SimulatorDrone_DW->LastUAtTimeB;
        }
      }

      rtb_Product = SimulatorDrone_M->Timing.t[0] - rtb_Product;
      rtb_acc_Word_idx_0 = (SimulatorDrone_B->y[0] - (*lastU)[0]) / rtb_Product;
      rtb_acc_Word_idx_1 = (SimulatorDrone_B->y[1] - (*lastU)[1]) / rtb_Product;
      rtb_acc_Word_idx_2 = (SimulatorDrone_B->y[2] - (*lastU)[2]) / rtb_Product;
    }

    /* End of Derivative: '<S5>/Derivative' */

    /* MATLAB Function: '<S5>/trafo_World2Body_trans' */
    /* MATLAB Function 'SimulatorDrone/DroneRS_Sensorsystem/IMU_Pressure/trafo_World2Body_trans': '<S9>:1' */
    /* '<S9>:1:3' */
    /* '<S9>:1:4' */
    /* '<S9>:1:5' */
    /* '<S9>:1:7' */
    /* '<S9>:1:12' */
    tmp_4[0] = cos(Product[4]) * cos(Product[3]);
    tmp_4[3] = cos(Product[4]) * sin(Product[3]);
    tmp_4[6] = -sin(Product[4]);
    tmp_4[1] = cos(Product[3]) * sin(Product[4]) * sin(Product[5]) - cos
      (Product[5]) * sin(Product[3]);
    tmp_4[4] = sin(Product[4]) * sin(Product[5]) * sin(Product[3]) + cos
      (Product[5]) * cos(Product[3]);
    tmp_4[7] = cos(Product[4]) * sin(Product[5]);
    tmp_4[2] = cos(Product[5]) * cos(Product[3]) * sin(Product[4]) + sin
      (Product[5]) * sin(Product[3]);
    tmp_4[5] = cos(Product[5]) * sin(Product[4]) * sin(Product[3]) - cos
      (Product[3]) * sin(Product[5]);
    tmp_4[8] = cos(Product[4]) * cos(Product[5]);

    /* Sum: '<S5>/Sum1' incorporates:
     *  Constant: '<S5>/gravity'
     *  MATLAB Function: '<S5>/trafo_World2Body_trans'
     */
    tmp_7 = SimulatorDrone_P->gravity_Value[0] + rtb_acc_Word_idx_0;
    rtb_sincos_o1_idx_1 = SimulatorDrone_P->gravity_Value[1] +
      rtb_acc_Word_idx_1;
    rtb_sincos_o1_idx_0 = SimulatorDrone_P->gravity_Value[2] +
      rtb_acc_Word_idx_2;

    /* MATLAB Function: '<S5>/trafo_World2Body_trans' incorporates:
     *  Gain: '<S5>/accel_gain'
     */
    for (i_0 = 0; i_0 < 3; i_0++) {
      tmp_5[i_0] = tmp_4[i_0 + 6] * rtb_sincos_o1_idx_0 + (tmp_4[i_0 + 3] *
        rtb_sincos_o1_idx_1 + tmp_4[i_0] * tmp_7);
    }

    /* SignalConversion: '<S5>/TmpSignal ConversionAtDelay2Inport1' incorporates:
     *  Gain: '<S5>/accel_gain'
     *  Gain: '<S5>/gyro_pq_gain'
     *  Gain: '<S5>/gyro_r_Gain'
     */
    rtb_TmpSignalConversionAtDelay2[0] = SimulatorDrone_P->
      quadEDT.IMUaccel_gain[0] * tmp_5[0];
    rtb_TmpSignalConversionAtDelay2[1] = SimulatorDrone_P->
      quadEDT.IMUaccel_gain[1] * tmp_5[1];
    rtb_TmpSignalConversionAtDelay2[2] = SimulatorDrone_P->
      quadEDT.IMUaccel_gain[2] * tmp_5[2];
    rtb_TmpSignalConversionAtDelay2[3] = SimulatorDrone_P->quadEDT.IMUgyro_gain
      [0] * rtb_RandomNumber[9];
    rtb_TmpSignalConversionAtDelay2[4] = SimulatorDrone_P->quadEDT.IMUgyro_gain
      [1] * rtb_RandomNumber[10];
    rtb_TmpSignalConversionAtDelay2[5] = SimulatorDrone_P->quadEDT.IMUgyro_gain
      [2] * rtb_RandomNumber[11];

    /* Saturate: '<S5>/Saturation' incorporates:
     *  Gain: '<S5>/alt_Gain'
     */
    rtb_sincos_o1_idx_0 = SimulatorDrone_P->alt_Gain_Gain * rtb_RandomNumber[2];
    if (rtb_sincos_o1_idx_0 > SimulatorDrone_P->Saturation_UpperSat) {
      /* SignalConversion: '<S5>/TmpSignal ConversionAtDelay2Inport1' */
      rtb_TmpSignalConversionAtDelay2[6] = SimulatorDrone_P->Saturation_UpperSat;
    } else if (rtb_sincos_o1_idx_0 < SimulatorDrone_P->quadEDT.altSenor_min) {
      /* SignalConversion: '<S5>/TmpSignal ConversionAtDelay2Inport1' */
      rtb_TmpSignalConversionAtDelay2[6] =
        SimulatorDrone_P->quadEDT.altSenor_min;
    } else {
      /* SignalConversion: '<S5>/TmpSignal ConversionAtDelay2Inport1' */
      rtb_TmpSignalConversionAtDelay2[6] = rtb_sincos_o1_idx_0;
    }

    /* End of Saturate: '<S5>/Saturation' */

    /* SignalConversion: '<S5>/TmpSignal ConversionAtDelay2Inport1' incorporates:
     *  Gain: '<S5>/altToprs_gain'
     */
    rtb_TmpSignalConversionAtDelay2[7] = SimulatorDrone_P->quadEDT.altToPrs_gain
      * rtb_RandomNumber[2];

    /* Gain: '<S6>/Gain2' */
    rtb_Product = SimulatorDrone_P->Gain2_Gain * Product[2];

    /* Product: '<S6>/Product1' incorporates:
     *  Gain: '<S6>/Gain1'
     *  MATLAB Function: '<S11>/statetoout'
     */
    rtb_sincos_o1_idx_0 = SimulatorDrone_P->Gain1_Gain[1] *
      SimulatorDrone_B->DiscreteTimeIntegrator[10] * rtb_Product;

    /* Product: '<S6>/Product' incorporates:
     *  Gain: '<S6>/Gain1'
     *  MATLAB Function: '<S11>/statetoout'
     */
    rtb_Product *= SimulatorDrone_P->Gain1_Gain[0] *
      SimulatorDrone_B->DiscreteTimeIntegrator[9];

    /* Gain: '<S6>/VelocityToOpticalFlow_Gain' incorporates:
     *  MATLAB Function: '<S11>/statetoout'
     *  Sum: '<S6>/Add1'
     */
    SimulatorDrone_B->VelocityToOpticalFlow_Gain[0] = (tmp_2[0] +
      rtb_sincos_o1_idx_0) *
      SimulatorDrone_P->quadEDT.velocityToOpticalFlow_gain;
    SimulatorDrone_B->VelocityToOpticalFlow_Gain[1] = (tmp_2[1] + rtb_Product) *
      SimulatorDrone_P->quadEDT.velocityToOpticalFlow_gain;
    SimulatorDrone_B->VelocityToOpticalFlow_Gain[2] =
      SimulatorDrone_P->quadEDT.velocityToOpticalFlow_gain * tmp_2[2];

    /* Saturate: '<S3>/pwmSaturation' incorporates:
     *  Inport: '<Root>/motorsPWM_datin'
     */
    /* MATLAB Function 'SimulatorDrone/Drone_Dynamics/MATLAB Function': '<S12>:1' */
    /* '<S12>:1:2' */
    if (SimulatorDrone_U_motorsPWM_datin[0] > SimulatorDrone_P->quadEDT.maxpwm)
    {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.maxpwm;
    } else if (SimulatorDrone_U_motorsPWM_datin[0] <
               SimulatorDrone_P->quadEDT.comega2topwm) {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.comega2topwm;
    } else {
      rtb_sincos_o1_idx_0 = SimulatorDrone_U_motorsPWM_datin[0];
    }

    /* MATLAB Function: '<S3>/MATLAB Function' incorporates:
     *  Constant: '<S3>/Constant'
     *  Constant: '<S3>/Constant2'
     *  Saturate: '<S3>/pwmSaturation'
     */
    rtb_y[0] = (rtb_sincos_o1_idx_0 - SimulatorDrone_P->quadEDT.comega2topwm) /
      SimulatorDrone_P->quadEDT.aomega2topwm;

    /* Saturate: '<S3>/pwmSaturation' incorporates:
     *  Inport: '<Root>/motorsPWM_datin'
     */
    if (SimulatorDrone_U_motorsPWM_datin[1] > SimulatorDrone_P->quadEDT.maxpwm)
    {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.maxpwm;
    } else if (SimulatorDrone_U_motorsPWM_datin[1] <
               SimulatorDrone_P->quadEDT.comega2topwm) {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.comega2topwm;
    } else {
      rtb_sincos_o1_idx_0 = SimulatorDrone_U_motorsPWM_datin[1];
    }

    /* MATLAB Function: '<S3>/MATLAB Function' incorporates:
     *  Constant: '<S3>/Constant'
     *  Constant: '<S3>/Constant2'
     *  Saturate: '<S3>/pwmSaturation'
     */
    rtb_y[1] = (rtb_sincos_o1_idx_0 - SimulatorDrone_P->quadEDT.comega2topwm) /
      SimulatorDrone_P->quadEDT.aomega2topwm;

    /* Saturate: '<S3>/pwmSaturation' incorporates:
     *  Inport: '<Root>/motorsPWM_datin'
     */
    if (SimulatorDrone_U_motorsPWM_datin[2] > SimulatorDrone_P->quadEDT.maxpwm)
    {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.maxpwm;
    } else if (SimulatorDrone_U_motorsPWM_datin[2] <
               SimulatorDrone_P->quadEDT.comega2topwm) {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.comega2topwm;
    } else {
      rtb_sincos_o1_idx_0 = SimulatorDrone_U_motorsPWM_datin[2];
    }

    /* MATLAB Function: '<S3>/MATLAB Function' incorporates:
     *  Constant: '<S3>/Constant'
     *  Constant: '<S3>/Constant2'
     *  Saturate: '<S3>/pwmSaturation'
     */
    rtb_y[2] = (rtb_sincos_o1_idx_0 - SimulatorDrone_P->quadEDT.comega2topwm) /
      SimulatorDrone_P->quadEDT.aomega2topwm;

    /* Saturate: '<S3>/pwmSaturation' incorporates:
     *  Inport: '<Root>/motorsPWM_datin'
     */
    if (SimulatorDrone_U_motorsPWM_datin[3] > SimulatorDrone_P->quadEDT.maxpwm)
    {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.maxpwm;
    } else if (SimulatorDrone_U_motorsPWM_datin[3] <
               SimulatorDrone_P->quadEDT.comega2topwm) {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.comega2topwm;
    } else {
      rtb_sincos_o1_idx_0 = SimulatorDrone_U_motorsPWM_datin[3];
    }

    /* MATLAB Function: '<S3>/MATLAB Function' incorporates:
     *  Constant: '<S3>/Constant'
     *  Constant: '<S3>/Constant1'
     *  Constant: '<S3>/Constant2'
     *  Saturate: '<S3>/pwmSaturation'
     */
    rtb_y[3] = (rtb_sincos_o1_idx_0 - SimulatorDrone_P->quadEDT.comega2topwm) /
      SimulatorDrone_P->quadEDT.aomega2topwm;
    rtb_y[0] = sqrt(rtb_y[0]);
    rtb_y[1] = sqrt(rtb_y[1]);
    rtb_y[2] = sqrt(rtb_y[2]);
    rtb_y[3] = sqrt(rtb_y[3]);
    rtb_y[0] += SimulatorDrone_P->quadEDT.bomega2topwm;
    rtb_y[1] += SimulatorDrone_P->quadEDT.bomega2topwm;
    rtb_y[2] += SimulatorDrone_P->quadEDT.bomega2topwm;
    rtb_y[3] += SimulatorDrone_P->quadEDT.bomega2topwm;

    /* Gain: '<S3>/Directions' incorporates:
     *  Saturate: '<S3>/omegaSaturation'
     */
    if (rtb_y[0] > SimulatorDrone_P->quadEDT.maxomega) {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.maxomega;
    } else if (rtb_y[0] < SimulatorDrone_P->omegaSaturation_LowerSat) {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->omegaSaturation_LowerSat;
    } else {
      rtb_sincos_o1_idx_0 = rtb_y[0];
    }

    rtb_y[0] = SimulatorDrone_P->Directions_Gain[0] * rtb_sincos_o1_idx_0;
    if (rtb_y[1] > SimulatorDrone_P->quadEDT.maxomega) {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.maxomega;
    } else if (rtb_y[1] < SimulatorDrone_P->omegaSaturation_LowerSat) {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->omegaSaturation_LowerSat;
    } else {
      rtb_sincos_o1_idx_0 = rtb_y[1];
    }

    rtb_y[1] = SimulatorDrone_P->Directions_Gain[1] * rtb_sincos_o1_idx_0;
    if (rtb_y[2] > SimulatorDrone_P->quadEDT.maxomega) {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.maxomega;
    } else if (rtb_y[2] < SimulatorDrone_P->omegaSaturation_LowerSat) {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->omegaSaturation_LowerSat;
    } else {
      rtb_sincos_o1_idx_0 = rtb_y[2];
    }

    rtb_y[2] = SimulatorDrone_P->Directions_Gain[2] * rtb_sincos_o1_idx_0;
    if (rtb_y[3] > SimulatorDrone_P->quadEDT.maxomega) {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->quadEDT.maxomega;
    } else if (rtb_y[3] < SimulatorDrone_P->omegaSaturation_LowerSat) {
      rtb_sincos_o1_idx_0 = SimulatorDrone_P->omegaSaturation_LowerSat;
    } else {
      rtb_sincos_o1_idx_0 = rtb_y[3];
    }

    rtb_y[3] = SimulatorDrone_P->Directions_Gain[3] * rtb_sincos_o1_idx_0;

    /* End of Gain: '<S3>/Directions' */

    /* MATLAB Function: '<S11>/MATLAB Function' incorporates:
     *  Constant: '<S11>/Constant1'
     *  Constant: '<S11>/Constant10'
     *  Constant: '<S11>/Constant11'
     *  Constant: '<S11>/Constant12'
     *  Constant: '<S11>/Constant13'
     *  Constant: '<S11>/Constant2'
     *  Constant: '<S11>/Constant3'
     *  Constant: '<S11>/Constant4'
     *  Constant: '<S11>/Constant5'
     *  Constant: '<S11>/Constant6'
     *  Constant: '<S11>/Constant7'
     *  Constant: '<S11>/Constant8'
     *  Constant: '<S11>/Constant9'
     */
    /* MATLAB Function 'SimulatorDrone/Drone_Dynamics/Dynamics/MATLAB Function': '<S16>:1' */
    /* % drone derivatives */
    /* global a1s b1s */
    /* rebuilt quad. */
    /* '<S16>:1:6' */
    /* '<S16>:1:7' */
    /* '<S16>:1:8' */
    rtb_sincos_o1_idx_1 = SimulatorDrone_P->quad.r;

    /* '<S16>:1:9' */
    /* '<S16>:1:10' */
    /* '<S16>:1:11' */
    rtb_sincos_o1_idx_2 = SimulatorDrone_P->quad.gamma;

    /* '<S16>:1:12' */
    /* '<S16>:1:13' */
    rtb_acc_Word_idx_0 = SimulatorDrone_P->quad.rho;

    /* '<S16>:1:14' */
    rtb_acc_Word_idx_1 = SimulatorDrone_P->quad.A;

    /* '<S16>:1:15' */
    /* '<S16>:1:16' */
    /* '<S16>:1:17' */
    /* '<S16>:1:18' */
    /* declarations/init */
    /* '<S16>:1:26' */
    /* '<S16>:1:27' */
    /* '<S16>:1:29' */
    /* '<S16>:1:30' */
    /* '<S16>:1:31' */
    /* CONSTANTS */
    /* Cardinal Direction Indicies */
    /* '<S16>:1:35' */
    /*    N       'North'                             1x1 */
    /* '<S16>:1:36' */
    /*    S       'South'                             1x1 */
    /* '<S16>:1:37' */
    /*    E       'East'                              1x1 */
    /* '<S16>:1:38' */
    /*    W       'West'                              1x1 */
    /* '<S16>:1:41' */
    rtb_Product = 0.70710678118654757 * SimulatorDrone_P->quad.d;

    /* '<S16>:1:43' */
    memset(&rtb_RandomNumber[0], 0, 12U * sizeof(real_T));

    /* '<S16>:1:45' */
    D[0] = rtb_Product;
    D[1] = -rtb_Product;
    D[2] = SimulatorDrone_P->quad.h;

    /*    Di      Rotor hub displacements             1x3 */
    /* '<S16>:1:46' */
    D[3] = rtb_Product;
    D[4] = rtb_Product;
    D[5] = SimulatorDrone_P->quad.h;

    /* '<S16>:1:47' */
    D[6] = -rtb_Product;
    D[7] = rtb_Product;
    D[8] = SimulatorDrone_P->quad.h;

    /* '<S16>:1:48' */
    D[9] = -rtb_Product;
    D[10] = -rtb_Product;
    D[11] = SimulatorDrone_P->quad.h;

    /* Body-fixed frame references */
    /*    ei      Body fixed frame references         3x1 */
    /* EXTRACT STATES FROM U */
    /* '<S16>:1:56' */
    /* EXTRACT STATES FROM X */
    /*  position in {W} */
    /* '<S16>:1:60' */
    /*  RPY angles {W} */
    /* '<S16>:1:61' */
    /*  velocity in {W} */
    /* '<S16>:1:62' */
    /*  angular velocity in {W} (body pqr? (F)) */
    /* PREPROCESS ROTATION AND WRONSKIAN MATRICIES */
    /* '<S16>:1:65' */
    /*  yaw */
    /* '<S16>:1:66' */
    /*  pitch */
    /* '<S16>:1:67' */
    /*  roll */
    /* if (z(3)>0) error('z greater than 0 (under ground)!'); end;   */
    /*  rotz(phi)*roty(the)*rotx(psi) */
    /* BBF > Inertial rotation matrix */
    /* '<S16>:1:75' */
    R[0] = cos(SimulatorDrone_B->DiscreteTimeIntegrator[4]) * cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]);
    R[3] = sin(SimulatorDrone_B->DiscreteTimeIntegrator[5]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[4]) * cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]) - cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[5]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]);
    R[6] = cos(SimulatorDrone_B->DiscreteTimeIntegrator[5]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[4]) * cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]) + sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[5]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]);
    R[1] = cos(SimulatorDrone_B->DiscreteTimeIntegrator[4]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]);
    R[4] = sin(SimulatorDrone_B->DiscreteTimeIntegrator[5]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[4]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]) + cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[5]) * cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]);
    R[7] = cos(SimulatorDrone_B->DiscreteTimeIntegrator[5]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[4]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]) - sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[5]) * cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[3]);
    R[2] = -sin(SimulatorDrone_B->DiscreteTimeIntegrator[4]);
    R[5] = sin(SimulatorDrone_B->DiscreteTimeIntegrator[5]) * cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[4]);
    R[8] = cos(SimulatorDrone_B->DiscreteTimeIntegrator[5]) * cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[4]);

    /* Manual Construction */
    /*      Q3 = [cos(phi) -sin(phi) 0;sin(phi) cos(phi) 0;0 0 1];   % RZ %Rotation mappings */
    /*      Q2 = [cos(the) 0 sin(the);0 1 0;-sin(the) 0 cos(the)];   % RY */
    /*      Q1 = [1 0 0;0 cos(psi) -sin(psi);0 sin(psi) cos(psi)];   % RX */
    /*      R = Q3*Q2*Q1    %Rotation matrix */
    /*  */
    /*     RZ * RY * RX */
    /* inverted Wronskian (body rates p-q-r to euler rates yaw pitch roll) */
    /* '<S16>:1:87' */
    rtb_acc_Word_idx_2 = cos(SimulatorDrone_B->DiscreteTimeIntegrator[4]);

    /* ROTOR MODEL */
    /* '<S16>:1:92' */
    for (i = 0; i < 4; i++) {
      /* '<S16>:1:92' */
      /* for each rotor */
      /* Relative motion */
      /* '<S16>:1:95' */
      SimulatorDrone_inv(R, tmp_0);
      for (i_0 = 0; i_0 < 3; i_0++) {
        tmp_2[i_0] = tmp_0[i_0 + 6] * SimulatorDrone_B->DiscreteTimeIntegrator[8]
          + (tmp_0[i_0 + 3] * SimulatorDrone_B->DiscreteTimeIntegrator[7] +
             tmp_0[i_0] * SimulatorDrone_B->DiscreteTimeIntegrator[6]);
      }

      rtb_u[0] = (D[i * 3 + 2] * SimulatorDrone_B->DiscreteTimeIntegrator[10] -
                  D[i * 3 + 1] * SimulatorDrone_B->DiscreteTimeIntegrator[11]) +
        tmp_2[0];
      rtb_u[1] = (D[i * 3] * SimulatorDrone_B->DiscreteTimeIntegrator[11] - D[i *
                  3 + 2] * SimulatorDrone_B->DiscreteTimeIntegrator[9]) + tmp_2
        [1];

      /* '<S16>:1:96' */
      rtb_Product = sqrt(rtb_u[0] * rtb_u[0] + rtb_u[1] * rtb_u[1]) / (fabs
        (rtb_y[i]) * rtb_sincos_o1_idx_1);

      /* Magnitude of mu, planar components */
      /* '<S16>:1:97' */
      /* Non-dimensionalised normal inflow */
      /* Non-dimensionalised induced velocity approximation */
      /* '<S16>:1:100' */
      rtb_sincos_o1_idx_0 = rt_atan2d_snf(rtb_u[1], rtb_u[0]);

      /* Sideslip azimuth relative to e1 (zero over nose) */
      /* '<S16>:1:101' */
      /* BBF > mu sideslip rotation matrix */
      /* Flapping */
      /* Longitudinal flapping */
      /* '<S16>:1:105' */
      /* sign(w) * (4/3)*((Ct/sigma)*(2*mu*gamma/3/a)/(1+3*e/2/r) + li)/(1+mu^2/2)]; %Lattitudinal flapping (note sign) */
      /* '<S16>:1:107' */
      tmp_7 = ((2.6666666666666665 * SimulatorDrone_P->quad.theta0 + 2.0 *
                SimulatorDrone_P->quad.theta1) * rtb_Product - ((D[i * 3 + 1] *
                 SimulatorDrone_B->DiscreteTimeIntegrator[9] - D[i * 3] *
                 SimulatorDrone_B->DiscreteTimeIntegrator[10]) + tmp_2[2]) /
               (fabs(rtb_y[i]) * rtb_sincos_o1_idx_1) * 2.0 * rtb_Product) /
        (1.0 - rtb_Product * rtb_Product / 2.0);

      /* Rotate the beta flapping angles to longitudinal and lateral coordinates. */
      /* '<S16>:1:108' */
      a1s[i] = (cos(rtb_sincos_o1_idx_0) * tmp_7 + sin(rtb_sincos_o1_idx_0) *
                0.0) - 16.0 / rtb_sincos_o1_idx_2 / fabs(rtb_y[i]) *
        SimulatorDrone_B->DiscreteTimeIntegrator[10];

      /* '<S16>:1:109' */
      b1s[i] = (-sin(rtb_sincos_o1_idx_0) * tmp_7 + cos(rtb_sincos_o1_idx_0) *
                0.0) - 16.0 / rtb_sincos_o1_idx_2 / fabs(rtb_y[i]) *
        SimulatorDrone_B->DiscreteTimeIntegrator[9];

      /* Forces and torques */
      /* '<S16>:1:112' */
      rtb_Product = SimulatorDrone_P->quad.Ct * rtb_acc_Word_idx_0 *
        rtb_acc_Word_idx_1 * (rtb_sincos_o1_idx_1 * rtb_sincos_o1_idx_1) *
        (rtb_y[i] * rtb_y[i]);
      i_0 = 1 + i;
      rtb_RandomNumber[3 * (i_0 - 1)] = -cos(b1s[i]) * sin(a1s[i]) * rtb_Product;
      rtb_RandomNumber[1 + 3 * (i_0 - 1)] = rtb_Product * sin(b1s[i]);
      rtb_RandomNumber[2 + 3 * (i_0 - 1)] = -cos(a1s[i]) * cos(b1s[i]) *
        rtb_Product;

      /* Rotor thrust, linearised angle approximations */
      /* '<S16>:1:113' */
      rtb_Product = -SimulatorDrone_P->quad.Cq * rtb_acc_Word_idx_0 *
        rtb_acc_Word_idx_1 * rt_powd_snf(rtb_sincos_o1_idx_1, 3.0) * rtb_y[i] *
        fabs(rtb_y[i]);
      i_0 = 1 + i;
      Product[3 * (i_0 - 1)] = rtb_Product * 0.0;
      Product[1 + 3 * (i_0 - 1)] = rtb_Product * 0.0;
      Product[2 + 3 * (i_0 - 1)] = rtb_Product;

      /* Rotor drag torque - note that this preserves w(i) direction sign */
      /* tau(:,i) = cross(T(:,i),D(:,i));                %Torque due to rotor thrust */
      /* '<S16>:1:115' */
      tau[3 * i] = D[i * 3 + 1] * rtb_RandomNumber[i * 3 + 2] - D[i * 3 + 2] *
        rtb_RandomNumber[i * 3 + 1];
      tau[1 + 3 * i] = D[i * 3 + 2] * rtb_RandomNumber[i * 3] -
        rtb_RandomNumber[i * 3 + 2] * D[i * 3];
      tau[2 + 3 * i] = rtb_RandomNumber[i * 3 + 1] * D[i * 3] - D[i * 3 + 1] *
        rtb_RandomNumber[i * 3];

      /* changed sign (F) */
      /* '<S16>:1:92' */
    }

    /* RIGID BODY DYNAMIC MODEL */
    /* '<S16>:1:120' */
    /* '<S16>:1:122' */
    rtb_Product = 1.0 / SimulatorDrone_P->quad.M;

    /* '<S16>:1:123' */
    for (i_0 = 0; i_0 < 3; i_0++) {
      rtb_u[i_0] = SimulatorDrone_P->quad.J[i_0 + 6] *
        SimulatorDrone_B->DiscreteTimeIntegrator[11] + (SimulatorDrone_P->
        quad.J[i_0 + 3] * SimulatorDrone_B->DiscreteTimeIntegrator[10] +
        SimulatorDrone_P->quad.J[i_0] * SimulatorDrone_B->
        DiscreteTimeIntegrator[9]);
    }

    /* row sum of torques */
    /* '<S16>:1:124' */
    /* This is the state derivative vector */
    /*  End of mdlDerivatives. */
    SimulatorDrone_sum(rtb_RandomNumber, tmp_2);
    SimulatorDrone_inv(SimulatorDrone_P->quad.J, tmp_0);
    SimulatorDrone_sum(tau, tmp_5);
    SimulatorDrone_sum(Product, tmp);
    tmp_6[0] = 0.0;
    tmp_6[3] = sin(SimulatorDrone_B->DiscreteTimeIntegrator[5]);
    tmp_6[6] = cos(SimulatorDrone_B->DiscreteTimeIntegrator[5]);
    tmp_6[1] = 0.0;
    tmp_6[4] = cos(SimulatorDrone_B->DiscreteTimeIntegrator[5]) * cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[4]);
    tmp_6[7] = -sin(SimulatorDrone_B->DiscreteTimeIntegrator[5]) * cos
      (SimulatorDrone_B->DiscreteTimeIntegrator[4]);
    tmp_6[2] = cos(SimulatorDrone_B->DiscreteTimeIntegrator[4]);
    tmp_6[5] = sin(SimulatorDrone_B->DiscreteTimeIntegrator[5]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[4]);
    tmp_6[8] = cos(SimulatorDrone_B->DiscreteTimeIntegrator[5]) * sin
      (SimulatorDrone_B->DiscreteTimeIntegrator[4]);
    for (i_0 = 0; i_0 < 3; i_0++) {
      tmp_1[3 * i_0] = tmp_6[3 * i_0] / rtb_acc_Word_idx_2;
      tmp_1[1 + 3 * i_0] = tmp_6[3 * i_0 + 1] / rtb_acc_Word_idx_2;
      tmp_1[2 + 3 * i_0] = tmp_6[3 * i_0 + 2] / rtb_acc_Word_idx_2;
    }

    tmp_7 = (-(SimulatorDrone_B->DiscreteTimeIntegrator[10] * rtb_u[2] -
               SimulatorDrone_B->DiscreteTimeIntegrator[11] * rtb_u[1]) + tmp_5
             [0]) + tmp[0];
    rtb_sincos_o1_idx_1 = (-(SimulatorDrone_B->DiscreteTimeIntegrator[11] *
      rtb_u[0] - SimulatorDrone_B->DiscreteTimeIntegrator[9] * rtb_u[2]) +
      tmp_5[1]) + tmp[1];
    rtb_sincos_o1_idx_0 = (-(SimulatorDrone_B->DiscreteTimeIntegrator[9] *
      rtb_u[1] - SimulatorDrone_B->DiscreteTimeIntegrator[10] * rtb_u[0]) +
      tmp_5[2]) + tmp[2];
    for (i_0 = 0; i_0 < 3; i_0++) {
      tmp_5[i_0] = tmp_1[i_0 + 6] * SimulatorDrone_B->DiscreteTimeIntegrator[11]
        + (tmp_1[i_0 + 3] * SimulatorDrone_B->DiscreteTimeIntegrator[10] +
           tmp_1[i_0] * SimulatorDrone_B->DiscreteTimeIntegrator[9]);
    }

    for (i_0 = 0; i_0 < 3; i_0++) {
      tmp[i_0] = ((R[i_0 + 3] * rtb_Product * tmp_2[1] + R[i_0] * rtb_Product *
                   tmp_2[0]) + R[i_0 + 6] * rtb_Product * tmp_2[2]) +
        SimulatorDrone_P->quad.g * (real_T)b[i_0];
    }

    for (i_0 = 0; i_0 < 3; i_0++) {
      tmp_2[i_0] = tmp_0[i_0 + 6] * rtb_sincos_o1_idx_0 + (tmp_0[i_0 + 3] *
        rtb_sincos_o1_idx_1 + tmp_0[i_0] * tmp_7);
    }

    SimulatorDrone_B->dx[0] = SimulatorDrone_B->DiscreteTimeIntegrator[6];
    SimulatorDrone_B->dx[1] = SimulatorDrone_B->DiscreteTimeIntegrator[7];
    SimulatorDrone_B->dx[2] = SimulatorDrone_B->DiscreteTimeIntegrator[8];
    SimulatorDrone_B->dx[3] = tmp_5[0];
    SimulatorDrone_B->dx[4] = tmp_5[1];
    SimulatorDrone_B->dx[5] = tmp_5[2];
    SimulatorDrone_B->dx[6] = tmp[0];
    SimulatorDrone_B->dx[7] = tmp[1];
    SimulatorDrone_B->dx[8] = tmp[2];
    SimulatorDrone_B->dx[9] = tmp_2[0];
    SimulatorDrone_B->dx[10] = tmp_2[1];
    SimulatorDrone_B->dx[11] = tmp_2[2];

    /* End of MATLAB Function: '<S11>/MATLAB Function' */
  }

  {
    real_T (*lastU)[3];
    int32_T iU;

    /* Update for Delay: '<S11>/Delay3' */
    memcpy(&SimulatorDrone_DW->Delay3_DSTATE[0], &SimulatorDrone_B->dx[0], 12U *
           sizeof(real_T));

    /* Update for Delay: '<S11>/Delay2' */
    SimulatorDrone_DW->Delay2_DSTATE = SimulatorDrone_B->DiscreteTimeIntegrator
      [2];

    /* Update for DiscreteIntegrator: '<S11>/Discrete-Time Integrator' */
    SimulatorDrone_DW->DiscreteTimeIntegrator_IC_LOADI = 0U;
    if (!rtb_LogicalOperator) {
      for (iU = 0; iU < 12; iU++) {
        SimulatorDrone_DW->DiscreteTimeIntegrator_DSTATE[iU] +=
          SimulatorDrone_P->DiscreteTimeIntegrator_gainval *
          SimulatorDrone_B->dx[iU];
      }
    }

    SimulatorDrone_DW->DiscreteTimeIntegrator_PrevRese = (int8_T)
      rtb_LogicalOperator;

    /* End of Update for DiscreteIntegrator: '<S11>/Discrete-Time Integrator' */

    /* Update for Delay: '<S6>/Delay3' */
    SimulatorDrone_DW->Delay3_DSTATE_b[0] =
      SimulatorDrone_B->VelocityToOpticalFlow_Gain[0];
    SimulatorDrone_DW->Delay3_DSTATE_b[1] =
      SimulatorDrone_B->VelocityToOpticalFlow_Gain[1];
    SimulatorDrone_DW->Delay3_DSTATE_b[2] =
      SimulatorDrone_B->VelocityToOpticalFlow_Gain[2];

    /* Update for Delay: '<S5>/Delay2' */
    memcpy(&SimulatorDrone_DW->Delay2_DSTATE_m[0],
           &rtb_TmpSignalConversionAtDelay2[0], sizeof(real_T) << 3U);

    /* Update for RandomNumber: '<S10>/Random Number' */
    for (iU = 0; iU < 12; iU++) {
      SimulatorDrone_DW->NextOutput[iU] = rt_nrand_Upu32_Yd_f_pw_snf
        (&SimulatorDrone_DW->RandSeed[iU]) *
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
  real_T SimulatorDrone_U_motorsPWM_datin[4], real_T *SimulatorDrone_Y_X, real_T
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
  (void) memset(SimulatorDrone_U_motorsPWM_datin, 0,
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

    /* InitializeConditions for Delay: '<S11>/Delay3' */
    for (i = 0; i < 12; i++) {
      SimulatorDrone_DW->Delay3_DSTATE[i] =
        SimulatorDrone_P->Delay3_InitialCondition;
    }

    /* End of InitializeConditions for Delay: '<S11>/Delay3' */

    /* InitializeConditions for Delay: '<S11>/Delay2' */
    SimulatorDrone_DW->Delay2_DSTATE = SimulatorDrone_P->Delay2_InitialCondition;

    /* InitializeConditions for DiscreteIntegrator: '<S11>/Discrete-Time Integrator' */
    SimulatorDrone_DW->DiscreteTimeIntegrator_IC_LOADI = 1U;
    SimulatorDrone_DW->DiscreteTimeIntegrator_PrevRese = 0;

    /* InitializeConditions for Delay: '<S6>/Delay3' */
    SimulatorDrone_DW->Delay3_DSTATE_b[0] =
      SimulatorDrone_P->Delay3_InitialCondition_p;
    SimulatorDrone_DW->Delay3_DSTATE_b[1] =
      SimulatorDrone_P->Delay3_InitialCondition_p;
    SimulatorDrone_DW->Delay3_DSTATE_b[2] =
      SimulatorDrone_P->Delay3_InitialCondition_p;

    /* InitializeConditions for Delay: '<S5>/Delay2' */
    memcpy(&SimulatorDrone_DW->Delay2_DSTATE_m[0],
           &SimulatorDrone_P->Delay2_InitialCondition_p[0], sizeof(real_T) << 3U);

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
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
