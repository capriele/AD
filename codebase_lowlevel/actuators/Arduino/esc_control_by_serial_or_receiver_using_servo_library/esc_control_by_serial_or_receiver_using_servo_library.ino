#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <Servo.h> 
int throttle[4] = {0,0,0,0};
Servo motor[4];

int flyCtrl[4] = {2, 3, 4, 5};   //these pins should be conected to flycontroller in the right order (first signal rises at pin2, next pin3, etc)
int flag = 7;                  //pin 7 in used to read the switch signal from transmiter on channel 8
/*first quad
int flyCtrl[4] = {5, 6, 7, 8};   //these pins should be conected to flycontroller in the right order (first signal rises at pin2, next pin3, etc)
int flag = 12;                  //pin 12 in used to read the switch signal from transmiter on channel 8
*/
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

  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);

  motor[0].attach(9);       //those should be the pins of the motor-arduino on the quad
  motor[1].attach(10);      
  motor[2].attach(11);
  motor[3].attach(12);

/*first quad
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  pinMode(8,INPUT);
  pinMode(12,INPUT);
  
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(13,OUTPUT);

  motor[0].attach(2);       //those should be the pins of the motor-arduino on the quad
  motor[1].attach(3);      
  motor[2].attach(4);
  motor[3].attach(13);
*/
  
  Serial.begin(115200);
  
  pwms[4] = pulseIn(flag, HIGH);
  
  while(Serial.read() != -1); //fflush;
  dt = micros();

  Serial.println(pwms[4]);
  
}

void loop() {
  
  if(pwms[4] > 1500)     //if switch on, copy flycontroller`s signal, and paste to motors.
  {
      
      pwms[0] = pulseIn(flyCtrl[0], HIGH);
      pwms[1] = pulseIn(flyCtrl[1], HIGH);
      pwms[2] = pulseIn(flyCtrl[2], HIGH);
      pwms[3] = pulseIn(flyCtrl[3], HIGH);
      pwms[4] = pulseIn(flag, HIGH);
      
   /*   
      while(!digitalRead(flyCtrl[0]))      // "coping"
        t[0] = micros();                   //timestamp when the first channel rises
      while(digitalRead(flyCtrl[0]))
        t[1] = micros();                  //timestamp when the second channel rises (same time the first channels go down)
      while(digitalRead(flyCtrl[1]))
        t[2] = micros();                  // ....
      while(digitalRead(flyCtrl[2]))
        t[3] = micros();
      while(digitalRead(flyCtrl[3]))
        t[4] = micros();    
                                        //room for more signals here if needed, ex.: pid tuning through rc-controller
      while(!digitalRead(flag))       
        t[5] = micros();
      while(digitalRead(flag))     //this flag is on channel 8     (last channel of the current receiver)  
        t[6] = micros();

      pwms[0] = t[1]-t[0];      
      pwms[1] = t[2]-t[1];
      pwms[2] = t[3]-t[2];
      pwms[3] = t[4]-t[3];
      pwms[4] = t[6]-t[5];
     */
      
      // Serial.print(pwms[0]);Serial.print("\t");Serial.print(pwms[1]);Serial.print("\t");Serial.print(pwms[2]);Serial.print("\t");Serial.print(pwms[3]);Serial.print("\t");Serial.println(pwms[4]);
  
      delay(2);   //delay sincronize signals from serial with signals from receiver
      
      while(Serial.read() != -1);  // fflush
      
    
      throttle[0] = map(pwms[0], 1000, 2000, 40, 140);      // "pasting"
      motor[0].write(throttle[0]);
      throttle[1] = map(pwms[1], 1000, 2000, 40, 140);      // the range of servo library is 0-180, but for best 'building' a pulse of width between 1000ms and 2000ms, we must map to 40-140
      motor[1].write(throttle[1]);
      throttle[2] = map(pwms[2], 1000, 2000, 40, 140);
      motor[2].write(throttle[2]);
      throttle[3] = map(pwms[3], 1000, 2000, 40, 140);
      motor[3].write(throttle[3]);

  }
  else            //if swicth is off, read string from serial, compute the pwms values, and 'paste' to motors
  {
      Serial.println(" test 3");
  
    
      while(!digitalRead(flag))      
         t[5] = micros();
      while(digitalRead(flag))
         t[6] = micros();
      pwms[4] = t[6]-t[5];        // check switch

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
      
      else        // with switch off, if doesn't receive anything from the jetson, kill all motors
      {
        pwms[0] = 1000;
        pwms[1] = 1000;
        pwms[2] = 1000;
        pwms[3] = 1000;
      }
  

      dt = micros()-dt;
      delay(2 - dt/1000);           //delay miliseconds, separetely for better precision
      delayMicroseconds(dt%1000);   //delay remaining microseconds

      throttle[0] = map(pwms[0], 1000, 2000, 40, 140);    // the range of servo library is 0-180, but for best 'building' a pulse of width between 1000ms and 2000ms, we must map to 40-140
      motor[0].write(throttle[0]);
      throttle[1] = map(pwms[1], 1000, 2000, 40, 140);
      motor[1].write(throttle[1]);
      throttle[2] = map(pwms[2], 1000, 2000, 40, 140);
      motor[2].write(throttle[2]);
      throttle[3] = map(pwms[3], 1000, 2000, 40, 140);
      motor[3].write(throttle[3]);
    
  }
}
