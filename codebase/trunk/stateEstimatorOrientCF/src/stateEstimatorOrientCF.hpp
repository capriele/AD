#ifndef STATEESTIMATORORIENTCF_H
#define STATEESTIMATORORIENTCF_H

#include "../../framework/framework.hpp"
#include <math.h>


#define IIRIMU 0.9

class stateEstimatorOrientCF_t : public podBase_t {
  
public: 

  //Pod-specific members to store received messages
  agile::imuRaw_t       	imudata;

  //agile::features_t       	features;


  int isGotBiases;

  //constructor
  stateEstimatorOrientCF_t (string podName, int64_t callInterval) : podBase_t(podName,callInterval)
	{	
		this->isGotBiases = -1;	

		// this value is changed for the first time by the end of calibration
		this->biases.accel[0] = 0;		
		this->biases.accel[1] = 0;	
		this->biases.accel[2] = 0;			
	}

  //Pod-specifc members and functions for computations and storing results over more than 1 function cycle call
   agile::stateVariances_t       stateVariances;
   std::string imuRawChannel;
   agile::imuRaw_t	 	biases;
   //double imuRawLast[6];
   double imuFiltered[6];
};



#endif
