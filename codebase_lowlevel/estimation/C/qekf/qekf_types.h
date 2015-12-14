/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: qekf_types.h
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 24-Aug-2015 11:29:39
 */

#ifndef __QEKF_TYPES_H__
#define __QEKF_TYPES_H__

/* Include Files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef typedef_struct0_T
#define typedef_struct0_T

typedef struct {
  double Q[9];
  double R[36];
  double P[16];
} struct0_T;

#endif                                 /*typedef_struct0_T*/

#ifndef typedef_struct2_T
#define typedef_struct2_T

typedef struct {
  double x;
  double y;
  double z;
} struct2_T;

#endif                                 /*typedef_struct2_T*/

#ifndef typedef_struct1_T
#define typedef_struct1_T

typedef struct {
  struct2_T bias;
  struct2_T sigma;
} struct1_T;

#endif                                 /*typedef_struct1_T*/

#ifndef typedef_struct3_T
#define typedef_struct3_T

typedef struct {
  double r1[3];
  double r2[3];
} struct3_T;

#endif                                 /*typedef_struct3_T*/
#endif

/*
 * File trailer for qekf_types.h
 *
 * [EOF]
 */
