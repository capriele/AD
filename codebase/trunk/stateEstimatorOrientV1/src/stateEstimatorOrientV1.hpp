#ifndef STATEESTIMATORORIENTV1_H
#define STATEESTIMATORORIENTV1_H

#include "../../framework/framework.hpp"
#include <qekf_types.h>
#include <math.h>


class stateEstimatorOrientV1_t : public podBase_t {
  
public: 

  //Pod-specific members to store received messages
  agile::imuRaw_t       	imudata;

  agile::features_t       	features;


  double q[4];				
  struct0_T *cov_matrixes;	
  struct1_T *w_stats;
  struct3_T *reference_versors;	//@TODO generalize for n vectors in the future when start to use the camera, (for now is only 2 since we are using only data from imu)
  double B[10];
  double prev_T; //used to be pointer
  double sigma_b1;
  double sigma_b2;
  int got_initial_features;
  double length1, length2;
 	
  /* B: all measurements 			*/
  /* q: current quaternion estimate: q_k 	*/
  /*  process noise covariance 			*/
  /*  measurement noise covariance 		*/
  /*  quaternion noise covariance 		*/
  /*  reference gravity direction (from accelerometer) */
  /*  reference magnetometer direction 		*/
  /* prev_T: timestamp of the previous measurement */
  /*  correct biases */


  //constructor
  stateEstimatorOrientV1_t (string podName, int64_t callInterval) : podBase_t(podName,callInterval)
	{	
		this->got_initial_features = -1;
		
		// this value is changed for the first time by the end of calibration
		this->features.versorDirection[0][0] = 0;		
		this->features.versorDirection[0][1] = 0;	
		this->features.versorDirection[0][2] = 0;	
		
		this->q[0] = 0.0;		//initial quaternion
		this->q[1] = 0.0;	
		this->q[2] = 0.0;
		this->q[3] = 1.0;

		this->sigma_b1 = 0.0;
		this->sigma_b2 = 0.0;			
		
		this->w_stats = (struct1_T *) malloc(sizeof(struct1_T));

		this->w_stats->sigma.x = 0.0;		 
		this->w_stats->sigma.y = 0.0;
		this->w_stats->sigma.z = 0.0;
		this->w_stats->bias.x = 0.0;		 
		this->w_stats->bias.y = 0.0;
		this->w_stats->bias.z = 0.0;

		this->reference_versors = (struct3_T *) malloc(sizeof(struct3_T));

		this->reference_versors->r1[0] = 0.0;
		this->reference_versors->r1[1] = 0.0;
		this->reference_versors->r1[2] = 0.0;		
		this->reference_versors->r2[0] = 0.0;
		this->reference_versors->r2[1] = 0.0;
		this->reference_versors->r2[2] = 0.0;

		this->B[0]= 0.0;
		this->B[1]= 0.0;
		this->B[2]= 0.0;
		this->B[3]= 0.0;
		this->B[4]= 0.0;
		this->B[5]= 0.0;
		this->B[6]= 0.0;
		this->B[7]= 0.0;
		this->B[8]= 0.0;
		this->B[9]= 0.0;
		
		this->cov_matrixes = (struct0_T *) malloc(sizeof(struct0_T));

		this->cov_matrixes->Q[0] = 0.0;
		this->cov_matrixes->Q[1] = 0.0;
		this->cov_matrixes->Q[2] = 0.0;
		this->cov_matrixes->Q[3] = 0.0;
		this->cov_matrixes->Q[4] = 0.0;
		this->cov_matrixes->Q[5] = 0.0;
		this->cov_matrixes->Q[6] = 0.0;
		this->cov_matrixes->Q[7] = 0.0;
		this->cov_matrixes->Q[8] = 0.0;
				
		this->cov_matrixes->R[0] = 0.0; 
		this->cov_matrixes->R[1] = 0.0; 
		this->cov_matrixes->R[2] = 0.0; 
		this->cov_matrixes->R[3] = 0.0;
		this->cov_matrixes->R[4] = 0.0;
		this->cov_matrixes->R[5] = 0.0;
		this->cov_matrixes->R[6] = 0.0; 
		this->cov_matrixes->R[7] = 0.0; 
		this->cov_matrixes->R[8] = 0.0;
		this->cov_matrixes->R[9] = 0.0; 
		this->cov_matrixes->R[10] = 0.0; 
		this->cov_matrixes->R[11] = 0.0; 
		this->cov_matrixes->R[12] = 0.0; 
		this->cov_matrixes->R[13] = 0.0; 
		this->cov_matrixes->R[14] = 0.0;
		this->cov_matrixes->R[15] = 0.0;
		this->cov_matrixes->R[16] = 0.0; 
		this->cov_matrixes->R[17] = 0.0;
		this->cov_matrixes->R[18] = 0.0;
		this->cov_matrixes->R[19] = 0.0;
 		this->cov_matrixes->R[20] = 0.0; 
		this->cov_matrixes->R[21] = 0.0; 
		this->cov_matrixes->R[22] = 0.0; 
		this->cov_matrixes->R[23] = 0.0;
		this->cov_matrixes->R[24] = 0.0;
		this->cov_matrixes->R[25] = 0.0;
		this->cov_matrixes->R[26] = 0.0; 
		this->cov_matrixes->R[27] = 0.0; 
		this->cov_matrixes->R[28] = 0.0;
		this->cov_matrixes->R[29] = 0.0; 
		this->cov_matrixes->R[30] = 0.0; 
		this->cov_matrixes->R[31] = 0.0; 
		this->cov_matrixes->R[32] = 0.0; 
		this->cov_matrixes->R[33] = 0.0;
		this->cov_matrixes->R[34] = 0.0;
		this->cov_matrixes->R[35] = 0.0;

		this->cov_matrixes->P[0] = 1.0; 
		this->cov_matrixes->P[1] = 0.0; 
		this->cov_matrixes->P[2] = 0.0; 
		this->cov_matrixes->P[3] = 0.0; 
		this->cov_matrixes->P[4] = 0.0; 
		this->cov_matrixes->P[5] = 1.0;
		this->cov_matrixes->P[6] = 0.0; 
		this->cov_matrixes->P[7] = 0.0; 
		this->cov_matrixes->P[8] = 0.0; 
		this->cov_matrixes->P[9] = 0.0; 
		this->cov_matrixes->P[10] = 1.0; 
		this->cov_matrixes->P[11] = 0.0; 
		this->cov_matrixes->P[12] = 0.0; 
		this->cov_matrixes->P[13] = 0.0; 
		this->cov_matrixes->P[14] = 0.0;
		this->cov_matrixes->P[15] = 1.0; 
		
		
	}

  //Pod-specifc members and functions for computations and storing results over more than 1 function cycle call
   agile::stateVariances_t       stateVariances;
   std::string imuRawChannel;
};



#endif
