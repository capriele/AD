#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

 #include <RCArduinoFastLib.h>

 // MultiChannels
//
// rcarduino.blogspot.com
//
// A simple approach for reading three RC Channels using pin change interrupts
//
// See related posts -
// http://rcarduino.blogspot.co.uk/2012/01/how-to-read-rc-receiver-with.html
// http://rcarduino.blogspot.co.uk/2012/03/need-more-interrupts-to-read-more.html
// http://rcarduino.blogspot.co.uk/2012/01/can-i-control-more-than-x-servos-with.html
//
// rcarduino.blogspot.com
//

// include the pinchangeint library - see the links in the related topics section above for details
#include <PinChangeInt.h>

// Assign your channel in pins
int flyCtrl_pinIns[4] = {2, 3, 4, 5}; //6 is flag channel
int flag_pin = 7;

// Assign your channel out pins
int pwms_outPins[4] 	  = {9,10,11,12};

int pwm_out[4] = {1000,1000,1000,1000};

// Assign servo indexes
#define SERVO_M0 0
#define SERVO_M1 1
#define SERVO_M2 2
#define SERVO_M3 3
#define SERVO_FRAME_SPACE 4

#define UPDATEPERIOD 10000 //in us, aka 20000 is 50Hz; 166hz e.g. brings problems, e.g. when all channels are ramped up


// These bit flags are set in bUpdateFlagsShared to indicate which
// channels have new signals
#define M0_FLAG 1
#define M1_FLAG 2
#define M2_FLAG 4
#define M3_FLAG 8
#define FLAG_FLAG 16

// holds the update flags defined above
volatile uint8_t bUpdateFlagsShared;

// shared variables are updated by the ISR and read by loop.
// In loop we immediatley take local copies so that the ISR can keep ownership of the
// shared ones. To access these in loop
// we first turn interrupts off with noInterrupts
// we take a copy to use in loop and the turn interrupts back on
// as quickly as possible, this ensures that we are always able to receive new signals
volatile uint16_t unM0InShared;
volatile uint16_t unM1InShared;
volatile uint16_t unM2InShared;
volatile uint16_t unM3InShared;
volatile uint16_t unFlagInShared;

// These are used to record the rising edge of a pulse in the calcInput functions
// They do not need to be volatile as they are only used in the ISR. If we wanted
// to refer to these in loop and the ISR then they would need to be declared volatile
uint16_t unM0InStart;
uint16_t unM1InStart;
uint16_t unM2InStart;
uint16_t unM3InStart;
uint16_t unFlagInStart;

uint16_t unLastAuxIn = 0;
uint32_t ulVariance = 0;
uint32_t ulGetNextSampleMillis = 0;
uint16_t unMaxDifference = 0;

uint8_t isInterruptsAttached = 1;
int tmp;

void setup()
{


  Serial.println("multiChannels");
  // attach servo objects, these will generate the correct
  // pulses for driving Electronic speed controllers, servos or other devices
  // designed to interface directly with RC Receivers
  CRCArduinoFastServos::attach(SERVO_M0,pwms_outPins[0]);
  CRCArduinoFastServos::attach(SERVO_M1,pwms_outPins[1]);
  CRCArduinoFastServos::attach(SERVO_M2,pwms_outPins[2]);
  CRCArduinoFastServos::attach(SERVO_M3,pwms_outPins[3]);   
 
  // lets set a standard rate of 50 Hz by setting a frame space of 10 * 2000 = 3 Servos + 7 times 2000
  // now: 4 ESC, 100Hz => 6*1000
  //CRCArduinoFastServos::setFrameSpaceA ... pushed to before write to Motor process. keeps update rate constant!
 

  CRCArduinoFastServos::begin();
 
  // using the PinChangeInt library, attach the interrupts
  // used to read the channels
  PCintPort::attachInterrupt(flyCtrl_pinIns[0], calcM0,CHANGE);
  PCintPort::attachInterrupt(flyCtrl_pinIns[1], calcM1,CHANGE);
  PCintPort::attachInterrupt(flyCtrl_pinIns[2], calcM2,CHANGE);
  PCintPort::attachInterrupt(flyCtrl_pinIns[3], calcM3,CHANGE);
  PCintPort::attachInterrupt(flag_pin, calcFlag,CHANGE);
  
  Serial.begin(115200);
  while(Serial.read() != -1);  //fflush serial
}

void loop()
{
    
  // create local variables to hold a local copies of the channel inputs
  // these are declared static so that thier values will be retained
  // between calls to loop.
  
  static uint16_t unM0In;
  static uint16_t unM1In;
  static uint16_t unM2In;
  static uint16_t unM3In;
  static uint16_t unGapPWM;
  static uint16_t unFlagIn;
  // local copy of update flags
  static uint8_t bUpdateFlags;
  static long lastJetson;

//Serial.println(unM3In); reading works!
//adjust spacing!


  if ((unFlagIn<1200) & (!isInterruptsAttached))
  {
  PCintPort::attachInterrupt(flyCtrl_pinIns[0], calcM0,CHANGE);
  PCintPort::attachInterrupt(flyCtrl_pinIns[1], calcM1,CHANGE);
  PCintPort::attachInterrupt(flyCtrl_pinIns[2], calcM2,CHANGE);
  PCintPort::attachInterrupt(flyCtrl_pinIns[3], calcM3,CHANGE); 
  isInterruptsAttached=1;  
  }
  else if (unFlagIn>1200)
  {
  PCintPort::detachInterrupt(flyCtrl_pinIns[0]);
  PCintPort::detachInterrupt(flyCtrl_pinIns[1]);
  PCintPort::detachInterrupt(flyCtrl_pinIns[2]);
  PCintPort::detachInterrupt(flyCtrl_pinIns[3]);   
  isInterruptsAttached = 0;
  };


  if(bUpdateFlagsShared)
      {
        noInterrupts(); // turn interrupts off quickly while we take local copies of the shared variables
    
        // take a local copy of which channels were updated in case we need to use this in the rest of loop
        bUpdateFlags = bUpdateFlagsShared;
      
        // in the current code, the shared values are always populated
        // so we could copy them without testing the flags
        // however in the future this could change, so lets
        // only copy when the flags tell us we can.
      
        if(bUpdateFlags & M0_FLAG)
        {
          unM0In = unM0InShared;
    
        }
      
      if(bUpdateFlags & M1_FLAG)
        {
          unM1In = unM1InShared;
        }
      if(bUpdateFlags & M2_FLAG)
        {
          unM2In = unM2InShared;
        }
      
      if(bUpdateFlags & M3_FLAG)
        {
          unM3In = unM3InShared;
        }
        
        if(bUpdateFlags & FLAG_FLAG)
        {
          unFlagIn = unFlagInShared;
        }
       
        // clear shared copy of updated flags as we have already taken the updates
        // we still have a local copy if we need to use it in bUpdateFlags
        bUpdateFlagsShared = 0;
      
   
        interrupts(); // we have local copies of the inputs, so now we can turn interrupts back on
        // as soon as interrupts are back on, we can no longer use the shared copies, the interrupt
        // service routines own these and could update them at any time. During the update, the
        // shared copies may contain junk. Luckily we have our local copies to work with :-)
      };
      
  if (unFlagIn > 1800)     //kill switch (SE switch completely up (physical position)). Receiver also sends this as failsafe
    {
      Serial.println("emergency detected");
      unGapPWM = 4000;
      CRCArduinoFastServos::setFrameSpaceA(SERVO_FRAME_SPACE,UPDATEPERIOD-unGapPWM);
      CRCArduinoFastServos::writeMicroseconds(SERVO_M0,1000);
      CRCArduinoFastServos::writeMicroseconds(SERVO_M1,1000);
      CRCArduinoFastServos::writeMicroseconds(SERVO_M2,1000);
      CRCArduinoFastServos::writeMicroseconds(SERVO_M3,1000);
        while(Serial.read() != -1);  //fflush serial
       /*      
       motor[0].writeMicroseconds(1000);
       motor[1].writeMicroseconds(1000);
       motor[2].writeMicroseconds(1000);
       motor[3].writeMicroseconds(1000); 
       */
    }  
  else if (unFlagIn<1200)
        {
          
          unGapPWM = unM0In+unM1In+unM2In+unM3In;
          CRCArduinoFastServos::setFrameSpaceA(SERVO_FRAME_SPACE,UPDATEPERIOD-unGapPWM);
          // check shared update flags to see if any channels have a new signal

          //Serial.println(unFlagIn);
          // do any processing from here onwards
          // only use the local values unAuxIn, unThrottleIn and unSteeringIn, the shared
          // variables unAuxInShared, unThrottleInShared, unSteeringInShared are always owned by
          // the interrupt routines and should not be used in loop
        
          // the following code provides simple pass through
          // this is a good initial test, the Arduino will pass through
          // receiver input as if the Arduino is not there.
          // This should be used to confirm the circuit and power
          // before attempting any custom processing in a project.
        
          // we are checking to see if the channel value has changed, this is indicated
          // by the flags. For the simple pass through we don't really need this check,
          // but for a more complex project where a new signal requires significant processing
          // this allows us to only calculate new values when we have new inputs, rather than
          // on every cycle.
          if(bUpdateFlags & M0_FLAG)
          {
            //Serial.println(unM0In);    
            CRCArduinoFastServos::writeMicroseconds(SERVO_M0 ,unM0In);    
          }
        
          if(bUpdateFlags & M1_FLAG)
          {
            CRCArduinoFastServos::writeMicroseconds(SERVO_M1,unM1In);
          }
        
          if(bUpdateFlags & M2_FLAG)
          {
            CRCArduinoFastServos::writeMicroseconds(SERVO_M2,unM2In);
          }
        
        
          if(bUpdateFlags & M3_FLAG)
          {
            CRCArduinoFastServos::writeMicroseconds(SERVO_M3,unM3In);
          }
        
        
          bUpdateFlags = 0;
          while(Serial.read() != -1);  //fflush serial
      } //end if from flightcontroller
 else            //if swicth is middle, read string from serial, compute the pwms values, and 'paste' to motors
  {
   
 
      if(Serial.available()>16)
      {

         while(Serial.read() != 'a'){}
       
         for(int i=0; i<4; i++){
             tmp = (Serial.read() - '0');
             tmp = 10*tmp + (Serial.read() - '0');
             tmp = 10*tmp + (Serial.read() - '0');
             tmp = 10*tmp + (Serial.read() - '0');      
             if(tmp < 2001 && tmp > 999 )
                  pwm_out[i] = tmp;
                   //        Serial.print(pwm_out[i]);         
         }
         //Serial.println();
         lastJetson = micros();
         
         while(Serial.read() != -1);   // serial print from jetson should be faster, clear jetson-printed stuff
        
      }
     
     if ((micros()-lastJetson)>10000) //Jetson updates at 200Hz, expected udpate every 5ms. if nothing after 10ms set zero
       {
        Serial.println((micros()-lastJetson));  
        pwm_out[0] = 1000;
        pwm_out[1] = 1000;
        pwm_out[2] = 1000;
        pwm_out[3] = 1000;
      }
      
      //Serial.println(pwm_out[0]);
      unGapPWM = pwm_out[0]+pwm_out[1]+pwm_out[2]+pwm_out[3];
      CRCArduinoFastServos::setFrameSpaceA(SERVO_FRAME_SPACE,UPDATEPERIOD-unGapPWM);
      CRCArduinoFastServos::writeMicroseconds(SERVO_M0,pwm_out[0]);
      CRCArduinoFastServos::writeMicroseconds(SERVO_M1,pwm_out[1]);
      CRCArduinoFastServos::writeMicroseconds(SERVO_M2,pwm_out[2]);
      CRCArduinoFastServos::writeMicroseconds(SERVO_M3,pwm_out[3]);
      /*
      motor[0].writeMicroseconds(pwm_out[0]);
      motor[1].writeMicroseconds(pwm_out[1]);
      motor[2].writeMicroseconds(pwm_out[2]);
      motor[3].writeMicroseconds(pwm_out[3]);*/
    
  }
}


// simple interrupt service routine
void calcM0()
{
  if(PCintPort::pinState)
  {
    unM0InStart = TCNT1;
  }
  else
  {
    unM0InShared = (TCNT1 - unM0InStart)>>1;
    bUpdateFlagsShared |= M0_FLAG;
  }
}

void calcM1()
{
  if(PCintPort::pinState)
  {
    unM1InStart = TCNT1;
  }
  else
  {
    unM1InShared = (TCNT1 - unM1InStart)>>1;
    bUpdateFlagsShared |= M1_FLAG;
  }
}


void calcM2()
{
  if(PCintPort::pinState)
  {
    unM2InStart = TCNT1;
  }
  else
  {
    unM2InShared = (TCNT1 - unM2InStart)>>1;
    bUpdateFlagsShared |= M2_FLAG;
  }
}


void calcM3()
{
  if(PCintPort::pinState)
  {
    unM3InStart = TCNT1;
  }
  else
  {
    unM3InShared = (TCNT1 - unM3InStart)>>1;
    bUpdateFlagsShared |= M3_FLAG;
  }
}



void calcFlag()
{
  if(PCintPort::pinState)
  {
    unFlagInStart = TCNT1;
  }
  else
  {
    unFlagInShared = (TCNT1 - unFlagInStart)>>1;
    bUpdateFlagsShared |= FLAG_FLAG;  }
}




