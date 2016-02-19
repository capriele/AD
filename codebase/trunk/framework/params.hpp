#ifndef PARAMETERS_H
#define PARAMETERS_H

/*
parameters.hpp contains universal parameters that are used in multiple parts of the code/various PODS
*/


//Drone general parameters
#define QUADMASS   0.946 	//[kg] @TODO this is with one battery
#define GRAVITY    9.81
#define QUADBISQRT 675.0 	//sqrt(1/quad.b)

//Drone Motor parameters
#define ZERORPMPWM 1000		//PWM value for no rotation of motors (0 RPM)
#define MAXPWM 1900		//PWM max value
#define IDLERPMPWM 1100		//PWM value to see motors spinning slowly


#define AW2PWM 2.6673e-4   //omega = sqrt((pwm-BPWM2W)/APWM2W)+CPWM2W :: pwm = AW2PWM*(w-CW2PWM)^2+BW2PWM
#define BW2PWM 1009.0
#define CW2PWM -200.2

#define THRUST2OMEGA2 455390 // THRUST2OMEGA2=1/ALPHAT with ALPHAT 2.2e-6 //Thrust = ALPHAT*w^2 (w=rotational speed of motor in rad/s)

#define TAKEOFFINTERVAL 1000	// time that drone is in takeoff mode after starting flight before transitioning to full flight mode [milliseconds]


//Safety regulations
#define MAXPOS_CRITICAL 9.0  		//max position range before critical status is issued[m]
#define MAXPOS_FATAL 	10.0 		//max position range before fatal status is issued[m]

#define MAXALT_CRITICAL 4.0		//max altitude before critical status is issued (note, this is positive, against convention of Z pointing downwards)[m]
#define MAXALT_FATAL 	5.0 		//max altitude before fatal status is issued (note, this is positive, against convention of Z pointing downwards)[m]

#define MAXORIENT_RP_CRITICAL 1.0	//max roll pitch angle critical ... [rad]
#define MAXORIENT_RP_FATAL    1.2	//max roll pitch angle fatal ... [rad]

#define MAXORIENT_Y_CRITICAL 3.0	//max yaw angle critical ... [rad]
#define MAXORIENT_Y_FATAL    3.14	//yaw angle  fatal ...[rad]

#define MAXVELPOS_CRITICAL   4.0	//max velocity critical ... [m/s]
#define MAXVELPOS_FATAL      5.0	//max velocity fatal ... [m/s]

#define MAXVELORIENT_PQ_CRITICAL   300.0	//max angular velocity roll pitch angle critical ... [rad/s]//@TODO choose bound, filter accel
#define MAXVELORIENT_PQ_FATAL      400.0	//max angular velocity roll pitch angle [rad/s]//@TODO choose bound, filter accel

#define MAXVELORIENT_R_CRITICAL    100.0	//max angular velocity yaw angle critical ...[rad/s]//@TODO choose bound, filter accel
#define MAXVELORIENT_R_FATAL       150.0	//max angular velocity yaw angle fatal ... [rad/s]//@TODO choose bound, filter accel

#define MAXACC_CRITICAL    	   300.0	//max acceleration critical ... ~3g [m/s^2] //@TODO choose bound, filter accel
#define MAXACC_FATAL       	   400.0	//max acceleration fatal ~4g [m/s^2]	    //@TODO choose bound, filter accel 


//CALLINTERVALS PODS: inverse rate at which glib mainloops of PODS are run [ms]

#define CALLINTERVAL_DEFAULT 10
#define CALLINTERVAL_EXAMPLEPOD 10
#define CALLINTERVAL_TESTSENDER 10

#define CALLINTERVAL_IMUACQUISITION 10		//IMU set to run at 100hz
#define CALLINTERVAL_STATEESTIMATORORIENTV1 10
#define CALLINTERVAL_STATEESTIMATORORIENTCF 10
#define CALLINTERVAL_DETECTORVIS 20
#define CALLINTERVAL_POSEESTIMVIS 20
#define CALLINTERVAL_REMOTECONTROLLER 20
#define CALLINTERVAL_WATCHDOG 10
#define CALLINTERVAL_CONTROLLERPDORIENT 10
#define CALLINTERVAL_CONTROLLERPDPOSE 10
#define CALLINTERVAL_CONTROLLERSOCORIENT 10
#define CALLINTERVAL_CONTROLLERSOCPOSE 10
#define CALLINTERVAL_MOTORCOMMANDER 5
#define CALLINTERVAL_SIMULATOR 5


//multi-POD communication, timing
#define MAXPODDELAY_NOTIFY_X 1.2 // POD computation interval can take at max MAXPODDELAY_X-times the POD's callinterval before notification is issued //@TODO this should be lower!, currently only used in imuAcquisition!
#define MAXPODDELAY_X 1.8 	 // POD computation interval can take at max MAXPODDELAY_X-times the POD's callinterval before error is issued //@TODO this should be lower!
#define MAXAGEMSGS_X 1.2  	 // stored message from a subscribed channel is considered out of date if most recent message was stored earlier than MAXAGEMSGS_X-times the expected receive interval of that message //@TODO this should be lower!
#define AGEMSG_X_TUNNEL 100.0	 //factor to increase tolerance for delays in receive-interval for messages (put to messages that are received over lcm) @TODO hack fix lcm/tunnel speed, should be lower, either delay or a lot of missed messaged (lost packets?!) missing clock sync should not be the issues since _received_-intervals are compared

#define MS2US 1000		 // conversion us to ms
#define DEADMSGDELAY_X 2	 // factor when delayed message is interpreted as sender POD probably being dead


//SENSOR STUFF
#define CALIBINTERVAL_IMU 2000	// length of calibration interval for IMU [milliseconds]


#endif
