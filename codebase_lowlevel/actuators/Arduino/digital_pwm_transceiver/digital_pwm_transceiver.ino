
int motor[4] = {6, 7, 8, 13};
int flyCrtl[4] = {2, 3, 4, 5};

double t[8];
void setup() {
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  Serial.begin(115200);

  while(!digitalRead(6))       //flag channel
      t[4] = micros();
  while(digitalRead(6))
      t[5] = micros();
  
  Serial.flush();
  dt = micros();
  
}

void loop() {
  if(t[5]-t[4] > 1500)     //if swicth on, copy and past receiver signal
  {
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
      while(digitalRead(flyCtrl[4]))      //this should be the flag channel
        t[5] = micros();                 
                                         
      
      //@TODO: add delay here to have both signals (receiver or serial) sycronized
      delay(10);
      // add serial flush here somewhere to proper read new data when we change do serial
      Serial.flush();
       
      dt = micros();
      
      digitalWrite(motor[0],HIGH);
      delayMicroseconds(t[1]-t[0]);
      digitalWrite(motor[0],LOW);
      digitalWrite(motor[1],HIGH);
      delayMicroseconds(t[2]-t[1]);
      digitalWrite(motor[1],LOW);
      digitalWrite(motor[2],HIGH);
      delayMicroseconds(t[3]-t[2]);
      digitalWrite(motor[2],LOW);
      digitalWrite(motor[3],HIGH);
      delayMicroseconds(t[4]-t[3]);
      digitalWrite(motor[3],LOW);
  }
  else            //if swicth off, read serial...
  {
      while(!digitalRead(6))       //flag channel should be the fisrt one
         t[4] = micros();
      while(digitalRead(6))
         t[5] = micros();
  
      if(Serial.available()>0){
       
         t[0] = (Serial.read() - '0');
         t[0] = 10*t[0] + (Serial.read() - '0');
         t[0] = 10*t[0] + (Serial.read() - '0');
         t[0] = 10*t[0] + (Serial.read() - '0');   
        
         t[1] = (Serial.read() - '0');
         t[1] = 10*t[1] + (Serial.read() - '0');
         t[1] = 10*t[1] + (Serial.read() - '0');
         t[1] = 10*t[1] + (Serial.read() - '0');
         
         t[2] = (Serial.read() - '0');
         t[2] = 10*t[2] + (Serial.read() - '0');
         t[2] = 10*t[2] + (Serial.read() - '0');
         t[2] = 10*t[2] + (Serial.read() - '0');
         
         t[3] = (Serial.read() - '0');
         t[3] = 10*t[3] + (Serial.read() - '0');
         t[3] = 10*t[3] + (Serial.read() - '0');
         t[3] = 10*t[3] + (Serial.read() - '0');

         Serial.flush();   // serial print from jetson should be slighty faster
        
      }
      //add else here in order to shut down motor if serial not available
      else
      {
        t0 = 1000;
        t1 = 1000;
        t2 = 1000;
        t3 = 1000;
      }


      dt = micros()-dt;
      delay(18 - dt/1000);   
      delayMicroseconds(dt%1000);
      dt = micros();
      
      
      digitalWrite(motor[0], HIGH);
      delayMicroseconds(t[0]); 
      digitalWrite(motor[0], LOW);
      
      digitalWrite(motor[1], HIGH);
      delayMicroseconds(t[1]);
      digitalWrite(motor[0], LOW);
  
      digitalWrite(motor[2], HIGH);
      delayMicroseconds(t[2]);
      digitalWrite(motor[0], LOW);
  
      digitalWrite(motor[3], HIGH);
      delayMicroseconds(t[3]);
      digitalWrite(motor[0], LOW);    
    
  }

}
