/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: qekf.h
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 24-Aug-2015 11:29:39
 */

#ifndef __QEKF_H__
#define __QEKF_H__

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "qekf_types.h"

/* Function Declarations */
extern void qekf(double q[4], const struct0_T *cov_matrixes, const struct1_T
                 *w_stats, const struct3_T *reference_versors, const double B[10],
                 double *prev_T, double P[16]);

#endif

/*
 * File trailer for qekf.h
 *
 * [EOF]
 */
