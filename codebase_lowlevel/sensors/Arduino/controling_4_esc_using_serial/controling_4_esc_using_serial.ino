     
  int motor[4] = {3, 6, 9, 12};     
  
  int T1 = 2050;
  int T2 = 18000;
  
  int t0 = 1000;
  int t1 = 1000;
  int t2 = 1000;
  int t3 = 1000;
  
  long dt, tt1, tt2;
  
  String  time0, time1, time2, time3;
  
  int count = 0;
  
//------------------------------------------------------------------------------------------------------------    
  
 void setup(){
    pinMode(13, OUTPUT); 
    digitalWrite(13, HIGH);
   
    Serial.begin(115200);          
  
    pinMode(motor[0], OUTPUT);
    pinMode(motor[1], OUTPUT);
    pinMode(motor[2], OUTPUT);
    pinMode(motor[3], OUTPUT);
    
    pinMode(2, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    
    digitalWrite(motor[0], LOW);
    digitalWrite(motor[1], LOW);
    digitalWrite(motor[2], LOW);
    digitalWrite(motor[3], LOW);
    
    digitalWrite(2, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH);
    
    dt = micros();
    Serial.flush();
 }
 
//------------------------------------------------------------------------------------------------------------    

 void loop(){ 
    if(count%(t0/20) == 0){
      digitalWrite(13, !digitalRead(13));   
    }
    count++;
    
    digitalWrite(motor[0], HIGH);
    delayMicroseconds(100);
    delayMicroseconds(t0);
    digitalWrite(motor[0], LOW);
    delayMicroseconds(T1-t0);
    
    
    digitalWrite(motor[1], HIGH);
    delayMicroseconds(100);
    delayMicroseconds(t1);
    digitalWrite(motor[1], LOW);
    delayMicroseconds(T1-t1);
    
    digitalWrite(motor[2], HIGH);
    delayMicroseconds(100);
    delayMicroseconds(t2);
    digitalWrite(motor[2], LOW);
    delayMicroseconds(T1-t2);
    
    digitalWrite(motor[3], HIGH);
    delayMicroseconds(100);
    delayMicroseconds(t3);
    digitalWrite(motor[3], LOW);
    delayMicroseconds(T1-t3);
    
     
  
    if(Serial.available()>0){
     
       t0 = (Serial.read() - '0');
       t0 = 10*t0 + (Serial.read() - '0');
       t0 = 10*t0 + (Serial.read() - '0');
       t0 = 10*t0 + (Serial.read() - '0');   
      
       t1 = (Serial.read() - '0');
       t1 = 10*t1 + (Serial.read() - '0');
       t1 = 10*t1 + (Serial.read() - '0');
       t1 = 10*t1 + (Serial.read() - '0');
       
       t2 = (Serial.read() - '0');
       t2 = 10*t2 + (Serial.read() - '0');
       t2 = 10*t2 + (Serial.read() - '0');
       t2 = 10*t2 + (Serial.read() - '0');
       
       t3 = (Serial.read() - '0');
       t3 = 10*t3 + (Serial.read() - '0');
       t3 = 10*t3 + (Serial.read() - '0');
       t3 = 10*t3 + (Serial.read() - '0');
      
    }
    
    //Serial.print(t0);Serial.print("\t");Serial.print(t1);Serial.print("\t");
    //Serial.print(t2);Serial.print("\t");Serial.print(t3);Serial.print("\t");
   
    dt = micros()-dt;
    
    delay(10 - dt/1000);   
    delayMicroseconds(dt%1000);
    //Serial.print(dt);Serial.print("\t");Serial.print(dt/1000);Serial.print("\t");Serial.println(dt%1000);
    dt = micros();
  }

