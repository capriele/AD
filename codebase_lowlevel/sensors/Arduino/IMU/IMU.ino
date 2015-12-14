#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_LSM303_U.h>

Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

long t;
long k = 1000000;
long gx,gy,gz;
long ax,ay,az;
long mx,my,mz;

void displaySensorDetails(void)
{
  sensor_t sensor;
  gyro.getSensor(&sensor);
/*  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" rad/s");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" rad/s");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" rad/s");  
  Serial.println("------------------------------------");
  Serial.println("");
*/  delay(500);
  accel.getSensor(&sensor);
/*  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
*/  delay(500);
  mag.getSensor(&sensor);
/*  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
  Serial.println("------------------------------------");
  Serial.println("");
*/  delay(500);
}

void setup(void) 
{
  pinMode(A6, INPUT);
  //digitalWrite(A6, LOW);
  //pinMode(A7, OUTPUT);
  //digitalWrite(A7, LOW);
  Serial.begin(115200);
  
//  Serial.println("Gyroscope Test"); Serial.println("");
  /* Enable auto-ranging */
  gyro.enableAutoRange(true);
  /* Initialise the sensor */
  if(!gyro.begin())
  {
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
    while(1);
  }
  
//  Serial.println("Accelerometer Test"); Serial.println(""); 
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  
//  Serial.println("Magnetometer Test"); Serial.println("");
  /* Enable auto-gain */
  mag.enableAutoRange(true);
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  t = micros();
}

void loop(void) 
{
  
  
  //t = micros();
  /* Get a new sensor event */ 
  sensors_event_t event1, event2, event3; 
  gyro.getEvent(&event1);
  accel.getEvent(&event2);
  mag.getEvent(&event3);
  gx=k*event1.gyro.x; 
  gy=k*event1.gyro.y;
  gz=k*event1.gyro.z;
  ax=k*event2.acceleration.x;
  ay=k*event2.acceleration.y;
  az=k*event2.acceleration.z; 
  mx=k*event3.magnetic.x; 
  my=k*event3.magnetic.y;
  mz=k*event3.magnetic.z;
 
  Serial.print(micros()); Serial.print(" ");
  Serial.print(gx); Serial.print(" ");
  Serial.print(gy); Serial.print(" ");
  Serial.print(gz); Serial.print(" "); //Serial.print("rad/s "); Serial.print("  ");
  Serial.print(ax); Serial.print(" ");
  Serial.print(ay); Serial.print(" ");
  Serial.print(az); Serial.print(" "); //Serial.print("m/s^2 "); Serial.print("  ");
  Serial.print(mx); Serial.print(" ");
  Serial.print(my); Serial.print(" ");
  Serial.print(mz); Serial.println("  ");//Serial.print("  ");Serial.println("uT");
  
  //delay(5);
}

