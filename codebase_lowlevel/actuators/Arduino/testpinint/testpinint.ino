//http://www.benripley.com/diy/arduino/three-ways-to-read-a-pwm-signal-with-arduino/

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <PinChangeInt.h>
 
#define MY_PIN 2 // we could choose any pin
#define MY_PIN2 3 // we could choose any pin
 
volatile int pwm_value[5] = {0,0,0,0,0};
volatile int prev_time =  0;
uint8_t latest_interrupted_pin;
 
void rising()
{
    prev_time = micros();
    PCintPort::attachInterrupt(MY_PIN, &falling, FALLING);
  
}
 
void falling() {
  latest_interrupted_pin=PCintPort::arduinoPin;
  if (latest_interrupted_pin == MY_PIN)
      {
        PCintPort::attachInterrupt(latest_interrupted_pin, &rising, RISING);
      }      
     
  pwm_value[latest_interrupted_pin-2] = micros()-prev_time;
  //Serial.println(state);
}
 
void setup() {
  pinMode(MY_PIN, INPUT);
  digitalWrite(MY_PIN, HIGH);
  
  pinMode(MY_PIN2, INPUT);
  digitalWrite(MY_PIN2, HIGH);
  
  Serial.begin(115200);
  PCintPort::attachInterrupt(MY_PIN, &rising, RISING);
  PCintPort::attachInterrupt(MY_PIN2, &falling, FALLING);
}
 
void loop() {
 Serial.println(pwm_value[0]);
  Serial.println(pwm_value[1]);
 delayMicroseconds(100000);
 
  }
