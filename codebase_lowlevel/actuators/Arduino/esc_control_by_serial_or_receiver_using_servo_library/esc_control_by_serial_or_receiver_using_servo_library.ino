#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <Servo.h> 
int throttle[4] = {0,0,0,0};
Servo motor[4];

//int motor[4] = {6, 7, 8, 13};   //TO DO: old pins used to control motors
int flyCtrl[4] = {2, 3, 4, 5};
int flag = 7;
long dt;
long t[8];
int pwms[5] = {1000,1000,1000,1000,1000};
int test;
char letter;

void setup() {
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,OUTPUT);
  pinMode(7,INPUT);

  motor[0].attach(9);       //those should be the pins of the motor-arduino on the quad
  motor[1].attach(10);      //TO DO: check if this code works with old pins, or change hardware to new pins 
  motor[2].attach(11);
  motor[3].attach(12);
  
  Serial.begin(115200);

  while(!digitalRead(flag))       
      t[5] = micros();
  while(digitalRead(flag))
      t[6] = micros();
  pwms[4] = t[6]-t[5];
  
  while(Serial.read() != -1);
  dt = micros();
  
}

void loop() {
  if(pwms[4] > 1500)     //if switch on, copy and paste receiver signal
  {
      digitalWrite(13,HIGH);
      while(!digitalRead(flyCtrl[0]))
        t[0] = micros();
      while(digitalRead(flyCtrl[0]))
        t[1] = micros();
      while(digitalRead(flyCtrl[1]))
        t[2] = micros();
      while(digitalRead(flyCtrl[2]))
        t[3] = micros();
      while(digitalRead(flyCtrl[3]))
        t[4] = micros();    
      
      while(!digitalRead(flag))       
        t[5] = micros();
      while(digitalRead(flag))     //this flag is on channel 6       
        t[6] = micros();

      pwms[0] = t[1]-t[0];
      pwms[1] = t[2]-t[1];
      pwms[2] = t[3]-t[2];
      pwms[3] = t[4]-t[3];
      pwms[4] = t[6]-t[5];
      
  
      delay(2);   //delay sincronize signals from serial with signals from receiver
      // add serial flush here somewhere to proper read new data when we change do serial
      while(Serial.read() != -1);
      
    
      throttle[0] = map(pwms[0], 1000, 2000, 40, 140);
      motor[0].write(throttle[0]);
      throttle[1] = map(pwms[1], 1000, 2000, 40, 140);
      motor[1].write(throttle[1]);
      throttle[2] = map(pwms[2], 1000, 2000, 40, 140);
      motor[2].write(throttle[2]);
      throttle[3] = map(pwms[3], 1000, 2000, 40, 140);
      motor[3].write(throttle[3]);

  }
  else            //if swicth off, read serial...
  {
      digitalWrite(13,LOW);
      while(!digitalRead(flag))      
         t[5] = micros();
      while(digitalRead(flag))
         t[6] = micros();
      pwms[4] = t[6]-t[5];

      dt = micros();
      
      if(Serial.available()>16){

         while(Serial.read() != 'a'){}
       
         for(int i=0; i<4; i++){
             test = (Serial.read() - '0');
             test = 10*test + (Serial.read() - '0');
             test = 10*test + (Serial.read() - '0');
             test = 10*test + (Serial.read() - '0');      
             if(test < 2001 && test > 999 )
                  pwms[i] = test;
         }
         
         while(Serial.read() != -1);   // serial print from jetson should be faster
        
      }
      //add else here in order to shut down motor if serial not available
      else        // if doesn't receive anything from the jetson, kill all motors
      {
        pwms[0] = 1000;
        pwms[1] = 1000;
        pwms[2] = 1000;
        pwms[3] = 1000;
      }
  

      dt = micros()-dt;
      delay(2 - dt/1000);   
      delayMicroseconds(dt%1000);

      throttle[0] = map(pwms[0], 1000, 2000, 40, 140);
      motor[0].write(throttle[0]);
      throttle[1] = map(pwms[1], 1000, 2000, 40, 140);
      motor[1].write(throttle[1]);
      throttle[2] = map(pwms[2], 1000, 2000, 40, 140);
      motor[2].write(throttle[2]);
      throttle[3] = map(pwms[3], 1000, 2000, 40, 140);
      motor[3].write(throttle[3]);
    
  }
}
