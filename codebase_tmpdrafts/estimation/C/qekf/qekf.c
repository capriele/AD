#include "qekf.h"
#include "norm.h"
#include "mrdivide.h"

/* Function Definitions */

/*
 * Arguments    : double q[4]
 *                const struct0_T *cov_matrixes
 *                const struct1_T *w_stats
 *                const struct3_T *reference_versors
 *                const double B[10]
 *                double *prev_T
 *                double P[16]
 * Return Type  : void
 */
void qekf(double q[4], const struct0_T *cov_matrixes, const struct1_T *w_stats,
          const struct3_T *reference_versors, const double B[10], double *prev_T,
          double P[16])
{
  double wx;
  double wy;
  double wz;
  double dt;
  double x[3];
  double w_k;
  double scale;
  int k;
  double absxk;
  double t;
  double Exp_Omega_k_T[16];
  double b_Exp_Omega_k_T[16];
  int i0;
  static const signed char b[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    1 };

  double b_q[12];
  double Gama_k[12];
  double c_Exp_Omega_k_T[16];
  double b_Gama_k[16];
  double qp[4];
  double c_Gama_k[12];
  int i1;
  double D_q_k[9];
  double dDdq1[9];
  double dDdq2[9];
  double dDdq3[9];
  double dDdq4[9];
  double b_dDdq2[3];
  double b_dDdq3[3];
  double b_dDdq4[3];
  double b_dDdq1[3];
  double c_dDdq2[3];
  double c_dDdq3[3];
  double c_dDdq4[3];
  double H_q[24];
  double b_H_q[24];
  double P_b[36];
  double K[24];
  double b_B[3];
  double c_B[3];
  double d_B[6];
  double b_D_q_k[6];
  double e_B[6];

  /* B: all measurements */
  /* q: current quaternion estimate: q_k */
  /*  process noise covariance */
  /*  measurement noise covariance */
  /*  quaternion noise covariance */
  /*  reference gravity direction (from accelerometer) */
  /*  reference magnetometer direction */
  /* prev_T: timestamp of the previous measurement */
  /*  correct biases */
  wx = B[1] - w_stats->bias.x;

  /*  x angular velocity mesauremnt */
  wy = B[2] - w_stats->bias.y;

  /*  y angular velocity mesauremnt */
  wz = B[3] - w_stats->bias.z;

  /*  z angular velocity mesauremnt */
  /*  compute time interval between current and previous measurement (integration period) */
  dt = B[0] - *prev_T;

  /*  time between mesaurements (s) */
  /*  store in better format the current direction measurements */
  /*  gravity versor measurement */
  /*  magnetic versor measurement */
  /*  rotation rate matrix */
  /*  Continuous-time process model: qdot = (1/2) Omega q */
  /*  Discrete-time process model: q_{k+1} = e^{1/2 Omega dt} q_k (Wertz78book, pag 564) */

  x[0] = wx;
  x[1] = wy;
  x[2] = wz;
  w_k = 0.0;
  scale = 2.2250738585072014E-308;
  for (k = 0; k < 3; k++) {
    absxk = fabs(x[k]);
    if (absxk > scale) {
      t = scale / absxk;
      w_k = 1.0 + w_k * t * t;
      scale = absxk;
    } else {
      t = absxk / scale;
      w_k += t * t;
    }
  }

  w_k = scale * sqrt(w_k);

  /*  See Wertz78book, eq (17.18) */
  absxk = cos(w_k * (dt / 2.0));
  scale = 1.0 / w_k * sin(w_k * (dt / 2.0));
  Exp_Omega_k_T[0] = 0.0;
  Exp_Omega_k_T[4] = -wx;
  Exp_Omega_k_T[8] = -wy;
  Exp_Omega_k_T[12] = -wz;
  Exp_Omega_k_T[1] = wx;
  Exp_Omega_k_T[5] = 0.0;
  Exp_Omega_k_T[9] = wz;
  Exp_Omega_k_T[13] = -wy;
  Exp_Omega_k_T[2] = wy;
  Exp_Omega_k_T[6] = -wz;
  Exp_Omega_k_T[10] = 0.0;
  Exp_Omega_k_T[14] = wx;
  Exp_Omega_k_T[3] = wz;
  Exp_Omega_k_T[7] = wy;
  Exp_Omega_k_T[11] = -wx;
  Exp_Omega_k_T[15] = 0.0;
  for (k = 0; k < 4; k++) {
    for (i0 = 0; i0 < 4; i0++) {
      b_Exp_Omega_k_T[i0 + (k << 2)] = absxk * (double)b[i0 + (k << 2)] + scale *
        Exp_Omega_k_T[i0 + (k << 2)];
    }
  }

  /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  /* 1a - Estimated state propagation */
  scale = dt / 2.0;
  b_q[0] = -q[1];
  b_q[4] = -q[2];
  b_q[8] = -q[3];
  b_q[1] = q[0];
  b_q[5] = -q[3];
  b_q[9] = q[2];
  b_q[2] = q[3];
  b_q[6] = q[0];
  b_q[10] = -q[1];
  b_q[3] = -q[2];
  b_q[7] = q[1];
  b_q[11] = q[0];
  for (k = 0; k < 4; k++) {
    qp[k] = 0.0;
    for (i0 = 0; i0 < 4; i0++) {
      qp[k] += b_Exp_Omega_k_T[k + (i0 << 2)] * q[i0];
    }

    for (i0 = 0; i0 < 3; i0++) {
      c_Gama_k[k + (i0 << 2)] = 0.0;
      for (i1 = 0; i1 < 4; i1++) {
        c_Gama_k[k + (i0 << 2)] += scale * b_Exp_Omega_k_T[k + (i1 << 2)] *
          b_q[i1 + (i0 << 2)];
      }
    }

    for (i0 = 0; i0 < 4; i0++) {
      Exp_Omega_k_T[k + (i0 << 2)] = 0.0;
      for (i1 = 0; i1 < 4; i1++) {
        Exp_Omega_k_T[k + (i0 << 2)] += b_Exp_Omega_k_T[k + (i1 << 2)] *
          cov_matrixes->P[i1 + (i0 << 2)];
      }
    }

    for (i0 = 0; i0 < 3; i0++) {
      Gama_k[k + (i0 << 2)] = 0.0;
      for (i1 = 0; i1 < 3; i1++) {
        Gama_k[k + (i0 << 2)] += c_Gama_k[k + (i1 << 2)] * cov_matrixes->Q[i1 +
          3 * i0];
      }
    }

    for (i0 = 0; i0 < 4; i0++) {
      c_Exp_Omega_k_T[k + (i0 << 2)] = 0.0;
      for (i1 = 0; i1 < 4; i1++) {
        c_Exp_Omega_k_T[k + (i0 << 2)] += Exp_Omega_k_T[k + (i1 << 2)] *
          b_Exp_Omega_k_T[i0 + (i1 << 2)];
      }
    }
  }

  for (k = 0; k < 4; k++) {
    for (i0 = 0; i0 < 4; i0++) {
      b_Gama_k[k + (i0 << 2)] = 0.0;
      for (i1 = 0; i1 < 3; i1++) {
        b_Gama_k[k + (i0 << 2)] += Gama_k[k + (i1 << 2)] * c_Gama_k[i0 + (i1 <<
          2)];
      }
    }
  }

  for (k = 0; k < 4; k++) {
    for (i0 = 0; i0 < 4; i0++) {
      P[i0 + (k << 2)] = c_Exp_Omega_k_T[i0 + (k << 2)] + b_Gama_k[i0 + (k << 2)];
    }
  }

  /* ---------------------------------------------------------------------- */
  /* 1b - Measuremnts Prediction */
  D_q_k[0] = ((qp[0] * qp[0] + qp[1] * qp[1]) - qp[2] * qp[2]) - qp[3] * qp[3];
  D_q_k[3] = 2.0 * (qp[1] * qp[2] + qp[0] * qp[3]);
  D_q_k[6] = 2.0 * (qp[1] * qp[3] - qp[0] * qp[2]);
  D_q_k[1] = 2.0 * (qp[1] * qp[2] - qp[0] * qp[3]);
  D_q_k[4] = ((qp[0] * qp[0] - qp[1] * qp[1]) + qp[2] * qp[2]) - qp[3] * qp[3];
  D_q_k[7] = 2.0 * (qp[3] * qp[2] + qp[0] * qp[1]);
  D_q_k[2] = 2.0 * (qp[1] * qp[3] + qp[0] * qp[2]);
  D_q_k[5] = 2.0 * (qp[3] * qp[2] - qp[0] * qp[1]);
  D_q_k[8] = ((qp[0] * qp[0] - qp[1] * qp[1]) - qp[2] * qp[2]) + qp[3] * qp[3];
  dDdq1[0] = 2.0 * qp[0];
  dDdq1[3] = 2.0 * qp[3];
  dDdq1[6] = -2.0 * qp[2];
  dDdq1[1] = -2.0 * qp[3];
  dDdq1[4] = 2.0 * qp[0];
  dDdq1[7] = 2.0 * qp[1];
  dDdq1[2] = 2.0 * qp[2];
  dDdq1[5] = -2.0 * qp[1];
  dDdq1[8] = 2.0 * qp[0];
  dDdq2[0] = 2.0 * qp[1];
  dDdq2[3] = 2.0 * qp[2];
  dDdq2[6] = 2.0 * qp[3];
  dDdq2[1] = 2.0 * qp[2];
  dDdq2[4] = -2.0 * qp[1];
  dDdq2[7] = 2.0 * qp[0];
  dDdq2[2] = 2.0 * qp[3];
  dDdq2[5] = -2.0 * qp[0];
  dDdq2[8] = -2.0 * qp[1];
  dDdq3[0] = -2.0 * qp[2];
  dDdq3[3] = 2.0 * qp[1];
  dDdq3[6] = -2.0 * qp[0];
  dDdq3[1] = 2.0 * qp[1];
  dDdq3[4] = 2.0 * qp[2];
  dDdq3[7] = 2.0 * qp[3];
  dDdq3[2] = 2.0 * qp[0];
  dDdq3[5] = 2.0 * qp[3];
  dDdq3[8] = -2.0 * qp[2];
  dDdq4[0] = -2.0 * qp[3];
  dDdq4[3] = 2.0 * qp[0];
  dDdq4[6] = 2.0 * qp[1];
  dDdq4[1] = -2.0 * qp[0];
  dDdq4[4] = -2.0 * qp[3];
  dDdq4[7] = 2.0 * qp[2];
  dDdq4[2] = 2.0 * qp[1];
  dDdq4[5] = 2.0 * qp[2];
  dDdq4[8] = 2.0 * qp[3];
  for (k = 0; k < 3; k++) {
    x[k] = 0.0;
    for (i0 = 0; i0 < 3; i0++) {
      x[k] += dDdq1[k + 3 * i0] * reference_versors->r1[i0];
    }

    b_dDdq2[k] = 0.0;
    for (i0 = 0; i0 < 3; i0++) {
      b_dDdq2[k] += dDdq2[k + 3 * i0] * reference_versors->r1[i0];
    }

    b_dDdq3[k] = 0.0;
    for (i0 = 0; i0 < 3; i0++) {
      b_dDdq3[k] += dDdq3[k + 3 * i0] * reference_versors->r1[i0];
    }

    b_dDdq4[k] = 0.0;
    for (i0 = 0; i0 < 3; i0++) {
      b_dDdq4[k] += dDdq4[k + 3 * i0] * reference_versors->r1[i0];
    }

    b_dDdq1[k] = 0.0;
    for (i0 = 0; i0 < 3; i0++) {
      b_dDdq1[k] += dDdq1[k + 3 * i0] * reference_versors->r2[i0];
    }

    c_dDdq2[k] = 0.0;
    for (i0 = 0; i0 < 3; i0++) {
      c_dDdq2[k] += dDdq2[k + 3 * i0] * reference_versors->r2[i0];
    }

    c_dDdq3[k] = 0.0;
    for (i0 = 0; i0 < 3; i0++) {
      c_dDdq3[k] += dDdq3[k + 3 * i0] * reference_versors->r2[i0];
    }

    c_dDdq4[k] = 0.0;
    for (i0 = 0; i0 < 3; i0++) {
      c_dDdq4[k] += dDdq4[k + 3 * i0] * reference_versors->r2[i0];
    }

    H_q[k] = x[k];
    H_q[6 + k] = b_dDdq2[k];
    H_q[12 + k] = b_dDdq3[k];
    H_q[18 + k] = b_dDdq4[k];
  }

  for (k = 0; k < 3; k++) {
    H_q[k + 3] = b_dDdq1[k];
    H_q[k + 9] = c_dDdq2[k];
    H_q[k + 15] = c_dDdq3[k];
    H_q[k + 21] = c_dDdq4[k];
  }

  for (k = 0; k < 6; k++) {
    for (i0 = 0; i0 < 4; i0++) {
      b_H_q[k + 6 * i0] = 0.0;
      for (i1 = 0; i1 < 4; i1++) {
        b_H_q[k + 6 * i0] += H_q[k + 6 * i1] * P[i1 + (i0 << 2)];
      }
    }
  }

  for (k = 0; k < 6; k++) {
    for (i0 = 0; i0 < 6; i0++) {
      scale = 0.0;
      for (i1 = 0; i1 < 4; i1++) {
        scale += b_H_q[k + 6 * i1] * H_q[i0 + 6 * i1];
      }

      P_b[k + 6 * i0] = scale + cov_matrixes->R[k + 6 * i0];
    }
  }

  /* ---------------------------------------------------------------------- */
  /* 1c - Cross-covariance */
  /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  /* 2a - Gain */
  for (k = 0; k < 4; k++) {
    for (i0 = 0; i0 < 6; i0++) {
      K[k + (i0 << 2)] = 0.0;
      for (i1 = 0; i1 < 4; i1++) {
        K[k + (i0 << 2)] += P[k + (i1 << 2)] * H_q[i0 + 6 * i1];
      }
    }
  }

  mrdivide(K, P_b);

  /* ---------------------------------------------------------------------- */
  /* 2b - Estimation update */
  b_B[0] = B[4];
  b_B[1] = B[5];
  b_B[2] = B[6];
  c_B[0] = B[7];
  c_B[1] = B[8];
  c_B[2] = B[9];
  for (k = 0; k < 3; k++) {
    d_B[k] = b_B[k];
  }

  for (k = 0; k < 3; k++) {
    d_B[k + 3] = c_B[k];
    x[k] = 0.0;
    for (i0 = 0; i0 < 3; i0++) {
      x[k] += D_q_k[k + 3 * i0] * reference_versors->r1[i0];
    }

    b_dDdq2[k] = 0.0;
    for (i0 = 0; i0 < 3; i0++) {
      b_dDdq2[k] += D_q_k[k + 3 * i0] * reference_versors->r2[i0];
    }

    b_D_q_k[k] = x[k];
  }

  for (k = 0; k < 3; k++) {
    b_D_q_k[k + 3] = b_dDdq2[k];
  }

  for (k = 0; k < 6; k++) {
    e_B[k] = d_B[k] - b_D_q_k[k];
  }

  for (k = 0; k < 4; k++) {
    scale = 0.0;
    for (i0 = 0; i0 < 6; i0++) {
      scale += K[k + (i0 << 2)] * e_B[i0];
    }

    q[k] = qp[k] + scale;
  }

  for (k = 0; k < 4; k++) {
    for (i0 = 0; i0 < 6; i0++) {
      b_H_q[k + (i0 << 2)] = 0.0;
      for (i1 = 0; i1 < 6; i1++) {
        b_H_q[k + (i0 << 2)] += K[k + (i1 << 2)] * P_b[i1 + 6 * i0];
      }
    }
  }

  for (k = 0; k < 4; k++) {
    for (i0 = 0; i0 < 4; i0++) {
      scale = 0.0;
      for (i1 = 0; i1 < 6; i1++) {
        scale += b_H_q[k + (i1 << 2)] * K[i0 + (i1 << 2)];
      }

      P[k + (i0 << 2)] -= scale;
    }
  }

  /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  /* 3 - Normalization */
  if (norm(q) != 0.0) {
    scale = 1.0 / norm(q);
    for (k = 0; k < 4; k++) {
      q[k] *= scale;
    }
  }

  /* [q' wx wy wz] */
  *prev_T = B[0];
}

/*
 * File trailer for qekf.c
 *
 * [EOF]
 */
