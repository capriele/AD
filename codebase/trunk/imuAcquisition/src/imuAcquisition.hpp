#ifndef IMUACQUISITION_H
#define IMUACQUISITION_H

#include "../../framework/framework.hpp"


#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <math.h>
#include <string>
#include <unistd.h>

class imuAcquisition_t : public podBase_t {
  
public: 

  //Pod-specific members to store received messages


  //Pod-specifc members and functions for computations and storing results over more than 1 function cycle call
  char const* portname;		//arduino portname
  int fd;	    		// file descriptor to write to arduino
  char buf[256];

  int statusCalib;		//status of Calibration -2 nothing, -1 running, 0 done
  int64_t starttimeCalib;
  agile::imuRaw_t imuCalib;
  int nMeasurements;
  double delta[3], mean[3];

  agile::stateVariances_t       stateVariances;
  agile::features_t       features;
//  agile::imuRaw_t imuRaw;


  //constructor
  imuAcquisition_t (string podName, int64_t callInterval) : podBase_t(podName,callInterval)
	{	
	//Pod-specific initialization
	this->fd = 0;
	this->portname = "/dev/ttyUSB0";
	this->statusCalib = -2;
	this->starttimeCalib = 0;

	this->features.featureDirectionVersor[0][0] = 0;
	this->features.featureDirectionVersor[0][1] = 0;
	this->features.featureDirectionVersor[0][2] = 0;
	
	this->features.featureDirectionVersor[1][0] = 0;
	this->features.featureDirectionVersor[1][1] = 0;
	this->features.featureDirectionVersor[1][2] = 0;
	

	};




};








#endif
