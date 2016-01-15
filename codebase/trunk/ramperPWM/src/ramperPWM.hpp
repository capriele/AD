#ifndef RAMPERPWM_H
#define RAMPERPWM_H

#include "../../framework/framework.hpp"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <math.h>
#include <string>

//powerAdjustment (Gain) Motorcommander 
#define MINPWM 1000
#define STEPSPWM 25

//epsilon
#define EPSREMOTE 0.00001


class ramperPWM_t : public podBase_t {
  
public: 

  	
  //Pod-specific members to store computation results
  agile::motorsPwms_t   motorsPwms;

  int16_t motorIdxToRamp;

  bool isWriteToArduino;	//flag is motorcommander tries to write to arduino or not

  char const* portname;		//arduino portname
  int fd;	    		// file descriptor to write to arduino
  int writeToArdStatus;	    	// return value of write process to arduino

  //constructor
  ramperPWM_t (string podName, int64_t callInterval) : podBase_t(podName, callInterval)
	{	
	//Pod-specific initialization
	//...
	motorsPwms.pwms[0] = MINPWM;
	motorsPwms.pwms[1] = MINPWM;
	motorsPwms.pwms[2] = MINPWM;
	motorsPwms.pwms[3] = MINPWM;

	motorIdxToRamp = 0;

	this->fd = 0;
	this->writeToArdStatus = 0;
	this->portname = "/dev/ttyUSB0"; /* The IMU-Arduino is on /dev/ttyUSB0 */
	this->isWriteToArduino = false; 
	}


 static void changemode(int dir)
{
  static struct termios oldt, newt;
 
  if ( dir == 1 )
  {
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  }
  else
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}
 
static int kbhit (void)
{
  struct timeval tv;
  fd_set rdfs;
 
  tv.tv_sec = 0;
  tv.tv_usec = 0;
 
  FD_ZERO(&rdfs);
  FD_SET (STDIN_FILENO, &rdfs);
 
  select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);
 
}

};




#endif
