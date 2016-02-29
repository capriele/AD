#ifndef IMUACQUISITION_H
#define IMUACQUISITION_H

#include "../../framework/framework.hpp"
#include "../../framework/paramsImuAcquisition.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <math.h>
#include <string>
#include <unistd.h>

class imuAcquisition_t : public podBase_t
{

public:

    //Pod-specific members to store received messages


    //Pod-specifc members and functions for computations and storing results over more than 1 function cycle call

    //char const* usbPortname;		//arduino portname
    std::string usbPortname;		//arduino portname
    int fd;	    		// file descriptor to write to arduino
    char buf[256];

    int statusCalib;		//status of Calibration -2 nothing, -1 running, 0 done
    int64_t starttimeCalib;
    agile::imuRaw_t imuCalib;
    int nMeasurements;
    double deltaGyro[3], meanGyro[3]; //helpers for moving computation of sensor variance
    double deltaAccel[3], meanAccel[3];

    bool noCalib;	//flag: does not do static calibration but uses preset values
    double defaultBias[12]; //sample bias for an IMU gyro-accel-gyroDiagVariance-accelDiagVariance

    agile::stateVariances_t       stateVariances;
    agile::features_t       features;
//  agile::imuRaw_t imuRaw;


    //constructor
    imuAcquisition_t (string podName, int64_t callInterval) : podBase_t(podName, callInterval)
    {
        //Pod-specific initialization
        this->fd = 0;
        this->statusCalib = -2;
        this->starttimeCalib = 0;

        this->features.featureDirectionVersor[0][0] = 0;
        this->features.featureDirectionVersor[0][1] = 0;
        this->features.featureDirectionVersor[0][2] = 0;

        this->features.featureDirectionVersor[1][0] = 0;
        this->features.featureDirectionVersor[1][1] = 0;
        this->features.featureDirectionVersor[1][2] = 0;

	this->defaultBias[0] = -0.0295;
	this->defaultBias[1] =  0.0520;
	this->defaultBias[2] = -0.0023;
	this->defaultBias[3] = -0.1977;
	this->defaultBias[4] = -0.0039;
	this->defaultBias[5] = 11.1232;

	this->defaultBias[6] = 0.000039;
	this->defaultBias[7] = 0.000016;
	this->defaultBias[8] = 0.000011;
	this->defaultBias[9] =  0.00727;
	this->defaultBias[10] = 0.001179;
	this->defaultBias[11] = 0.2136;


	this->noCalib = false;
    };

    bool updateStatus() override;
    bool doComputations() override;
};








#endif
