#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

/*
using interrupts to read rc-jetson-flag from slow 50Hz receiver on channel 8.
connect channel 8 signal to d2 and d3
set rising flag interrupt on d2
set falling flag interrupt on d3

*/

#include <PinChangeInt.h>
#include "Servo.h"


int flCtrl_inPins[4]   = {2, 3, 4, 5};   //these pins should be conected to flycontroller in the right order (first signal rises at pin2, next pin3, etc) 
int motors_outPins[4]  = {7,8,10,11};
int switch_risePin = 2;                 //connect channel 8: rc-vs-jetson-kill-switch            
int switch_fallPin = 3; 		//connect channel 8: rc-vs-jetson-kill-switch            //can save this one when the pin 2-rise_interrupt function attaches a falling-interrupt to pin 2; and the falling intterupt routine reverse this; or attach change-intterup and check in interrupt routine if pin is low or high
volatile long switchPWM = 1000;   
//long tmpswitchPWM = 1000;   
volatile long riseSwitchTime = 0;      
volatile int prevTimeFlCtrl =  0;
//volatile long fallSwitchTime = 0;            
//long tmpt1;
//long tmpt2;
uint8_t latest_interrupted_pin;

Servo motor[4];

volatile int pwm_read[4] = {1000,1000,1000,1000};
int pwm_out[4] = {1000,1000,1000,1000};
int tmp;

void rising()
{
    prevTimeFlCtrl = micros();
    PCintPort::attachInterrupt(flCtrl_inPins[0], &falling, FALLING);
  
}
 
void falling() {
  latest_interrupted_pin=PCintPort::arduinoPin;
  if (latest_interrupted_pin == flCtrl_inPins[0])
      {
        PCintPort::attachInterrupt(latest_interrupted_pin, &rising, RISING);
      }      
     
  pwm_read[latest_interrupted_pin-2] = micros()-prevTimeFlCtrl;
  //Serial.println( pwm_read[latest_interrupted_pin-2]);
}


void setup() {
  pinMode(flCtrl_inPins[0],INPUT);digitalWrite(flCtrl_inPins[0], HIGH);
  pinMode(flCtrl_inPins[1],INPUT);digitalWrite(flCtrl_inPins[1], HIGH);
  pinMode(flCtrl_inPins[2],INPUT);digitalWrite(flCtrl_inPins[2], HIGH);
  pinMode(flCtrl_inPins[3],INPUT);digitalWrite(flCtrl_inPins[3], HIGH);
  
  PCintPort::attachInterrupt(flCtrl_inPins[0], &rising, RISING); //this is the main channel!
  PCintPort::attachInterrupt(flCtrl_inPins[1], &falling, FALLING);
  PCintPort::attachInterrupt(flCtrl_inPins[2], &falling, FALLING);
  PCintPort::attachInterrupt(flCtrl_inPins[3], &falling, FALLING);

  
  pinMode(motors_outPins[0],OUTPUT);
  pinMode(motors_outPins[1],OUTPUT);
  pinMode(motors_outPins[2],OUTPUT);
  pinMode(motors_outPins[3],OUTPUT);

  motor[0].attach(motors_outPins[0]);
  motor[1].attach(motors_outPins[1]);      
  motor[2].attach(motors_outPins[2]);
  motor[3].attach(motors_outPins[3]);
  

  Serial.begin(115200);
 
  while(Serial.read() != -1); //fflush, clear jetson-printed stuff
  
}

void loop() {

    if (switchPWM>2300) switchPWM=1000;
  
  //Serial.println(pwm_read[0]);
  
  if(switchPWM > 1800)     //kill switch (SE switch completely up (physical position)). Receiver also sends this as failsafe
    {
      Serial.println("emergency deteced");
       motor[0].writeMicroseconds(1000);
       motor[1].writeMicroseconds(1000);
       motor[2].writeMicroseconds(1000);
       motor[3].writeMicroseconds(1000); 
    }
  
  
  else if(switchPWM < 1200)     //pilot wants flycontroller to control motors: read flycontroller`s signal write same pwm to motors.
  {
      //this has blocking code. flightcontroller output runs at 400Hz, with this code, output down to 100Hz (?), 100% motor 1 leads to a .5ms gap before m2 sends pulse - .5ms to write to motors and be ready for pulseln on m2!
      //noInterrupts();
      //tmpt1=micros();
      //pwm_read[0] = pulseIn(flCtrl_inPins[0], HIGH);
      noInterrupts();
      int pwm_read0 = pwm_read[0];
            interrupts();
      //interrupts();
      
      if ((pwm_read0>500) && (pwm_read0<2500))
        {
        //pwm_out[0] = map(pwm_read[0], 1300, 2600, 1000, 2000);
        motor[0].writeMicroseconds(pwm_read0);      
      }

      
      //max pwm is 2ms+0.05write+stuff.. leave 0.3ms free to to directly sit in next pulseln function!
      
            
      /*
      //noInterrupts();      
      //pwm_read[1] = pulseIn(flCtrl_inPins[1], HIGH,pulselnTimeout);
      //interrupts();  
      //pwm_out[1] = map(pwm_read[1], 1300, 2600, 1000, 2000);
      motor[1].writeMicroseconds(pwm_out[1]);
      
      //delayMicroseconds(400);
      
      //noInterrupts();       
      //pwm_read[2] = pulseIn(flCtrl_inPins[2], HIGH,pulselnTimeout);
      //interrupts();  
      //pwm_out[2] = map(pwm_read[2], 1300, 2600, 1000, 2000);
      motor[2].writeMicroseconds(pwm_out[2]);
   
       //delayMicroseconds(400);   
      
      //noInterrupts();              
      //pwm_read[3] = pulseIn(flCtrl_inPins[3], HIGH,pulselnTimeout);
      //interrupts();  
      //pwm_out[3] = map(pwm_read[3], 1300, 2600, 1000, 2000);
      motor[3].writeMicroseconds(pwm_out[3]);
      
      //delayMicroseconds(400);   
      */
      while(Serial.read() != -1);  // fflush, clear jetson printed stuff
	
  }
  else            //if swicth is middle, read string from serial, compute the pwms values, and 'paste' to motors
  {
   
 
      if(Serial.available()>16){

         while(Serial.read() != 'a'){}
       
         for(int i=0; i<4; i++){
             tmp = (Serial.read() - '0');
             tmp = 10*tmp + (Serial.read() - '0');
             tmp = 10*tmp + (Serial.read() - '0');
             tmp = 10*tmp + (Serial.read() - '0');      
             if(tmp < 2001 && tmp > 999 )
                  pwm_out[i] = tmp;
         }
         
         while(Serial.read() != -1);   // serial print from jetson should be faster, clear jetson-printed stuff
        
      }
      
      else        // with switch off, if doesn't receive anything from the jetson, kill all motors
      {
        pwm_out[0] = 1000;
        pwm_out[1] = 1000;
        pwm_out[2] = 1000;
        pwm_out[3] = 1000;
      }
  
      motor[0].writeMicroseconds(pwm_out[0]);
      motor[1].writeMicroseconds(pwm_out[1]);
      motor[2].writeMicroseconds(pwm_out[2]);
      motor[3].writeMicroseconds(pwm_out[3]);
    
  }
}
