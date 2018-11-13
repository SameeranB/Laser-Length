


#include <QMC5883L.h>

QMC5883L compass;



#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;


// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

#define LONG_RANGE


// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define HIGH_SPEED
#define HIGH_ACCURACY
char a;

void setup()
{

  Serial.begin(9600);
  Wire.begin();

  sensor.init();
  sensor.setTimeout(500);

#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  sensor.setMeasurementTimingBudget(200000);
#endif




  compass.init();
  compass.setSamplingRate(50);



}

float TOF() // Returns the distance to the wall
{
  Serial.print("The Length is : ");
  Serial.println(sensor.readRangeSingleMillimeters());
  return(sensor.readRangeSingleMillimeters());
 

 
}

int QMC() // Returns the orientation in degrees
{

  int heading = compass.readHeading();
  if (heading == 0) {
    /* Still calibrating, so measure but don't print */
  } else {
    Serial.print("The Compass Reading is : ");
    Serial.println(compass.readHeading());
    return(compass.readHeading());
  }

}

void loop()
{
  float T1=0,T2=0,C1=0,C2=0;

  Serial.println("Point to the first edge and enter 1");
  while(Serial.available()== 0)
  {
    
  }
  if(Serial.available()) // Wait till an input is detected
  {
    a=Serial.read();
  }
  
    if(a=='1') // if input is '1' then continue
    {
    T1=TOF(); // Enter the Distance and angle to the first wall
    C1=QMC();
    a = '0';
    delay(1000);
    }

  if(T1!=0)
  {
    Serial.println("Point to the second edge and enter 1");
   while(T2 == 0)
  {
    
  
    if(Serial.available())// Wait till an input is detected
  {
    a=Serial.read();
  }
  
    if(a=='1')// if input is '1' then continue
    {
    T2=TOF();// Enter the Distance and angle to the second wall
    C2=QMC();
    a = '0';
    delay(1000);
    }
  }
  }
  if(T2!=0)
  {
    int angle,temp1,temp2;
    float answer;
    angle=abs(C1-C2);
    if(angle>180) // Calculating the shortest angle between the two given degrees
    {
      if(C1>=C2)
      {
        temp1=C1;
        temp2=C2;
        Serial.print("C1 = :");
        Serial.println(temp1);
        Serial.print("C2 = :");
        Serial.println(temp2);
      }
      else
      {
        temp1=C2;
        temp2=C1;
        Serial.print("C1 = :");
        Serial.println(temp2);
        Serial.print("C2 = :");
        Serial.println(temp1);
      }
      temp1=360-temp1;
      
      angle=abs(temp1+temp2);
      
    }
    
    Serial.print("The angle is : ");
    Serial.println(angle);
    Serial.print("T1 = :");
    Serial.println(T1);
    Serial.print("T2 = :");
    Serial.println(T2);
    answer=sqrt((T1*T1)+ (T2*T2)- 2*T1*T2*cos(angle)); // Equation to calculate the length of the wall using Cosine Rule
    Serial.print("The Length of the wall is:  ");
    Serial.println(answer);
  }



}
