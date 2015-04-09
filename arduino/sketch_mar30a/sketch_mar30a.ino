// Author: Leandro Arcusin (larcusin@henwaytech.com)

#define TWI_FREQ 400000L
#define cycles_base 100
#define cycles_multiplier 1
#define start_frequency 50000
#define cal_samples 10

#include "AD5933.h"
#include <Wire.h>

const int numofIncrement = 10;
const double calResistance = 511.0;
//double arrGFactor[numofIncrement];
double gainFactor, pShift;

int LED7_R= 3;
int LED7_G= 4;
int LED7_B= 5;

int LED5= 13;
int LED6= 12;
int LED8= 11;

int PUSH1= 27;
int PUSH2= 28;

int BOOST= 22;

int BI_TETRA = 29;

int leds[6]={LED5, LED6, LED7_R, LED7_G, LED7_B,LED8};

void setup() 
{
  
  // put your setup code here, to run once:
  //Set IOs mode asdasdasdasdsaasdadasdasdasd

  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);  
  pinMode(LED7_R, OUTPUT);
  pinMode(LED7_G, OUTPUT);
  pinMode(LED7_B, OUTPUT);
  pinMode(LED8, OUTPUT);
  pinMode(BI_TETRA, OUTPUT);

  pinMode(BOOST, OUTPUT);

  pinMode(PUSH1, INPUT);
  pinMode(PUSH2, INPUT);
  
  pinMode(25, INPUT);
  pinMode(24, INPUT);

  //All leds off
  for(int i=0;i<6;i++) {
    digitalWrite(leds[i],HIGH);
  }
  digitalWrite(BOOST, HIGH);
  digitalWrite(BI_TETRA, HIGH);
  
  Wire.begin();        // join i2c bus (address optional for master)
  AD5933.setExtClock(false);
  AD5933.resetAD5933();
  AD5933.setStartFreq(start_frequency);
  AD5933.setIncrement(5000);
  AD5933.setNumofIncrement(numofIncrement);
  AD5933.setSettlingCycles(cycles_base, cycles_multiplier);
  AD5933.getTemperature();
  AD5933.setVolPGA(3,1);
  
  //gainFactor = AD5933.getGainFactor(calResistance,1);
  if(!AD5933.getGainFactorC(calResistance, 10, gainFactor, pShift, false))
	Serial.println("Calibration error!!");  
 
  Serial.begin(9600);
  Serial.println("Henway Technologies Test program");
  Serial.println("--------------------------------");
 
}

void loop() 
{
  AD5933.tempUpdate();
  AD5933.setCtrMode(REPEAT_FREQ);
 
  double cReal, cImag;
  double Z_Val, phase;
  AD5933.getComplexOnce(gainFactor, pShift, cReal, cImag, Z_Val, phase);
  
  Serial.print(millis() / 1000.0); 
  Serial.print("\t"); 
  Serial.print(cReal);
  Serial.print("\t"); 
  Serial.print(cImag);
  Serial.print("\t"); 
  Serial.print(Z_Val);
  Serial.print("\t"); 
  Serial.print(phase);
  Serial.println();
  
  Serial.print("Temperature = ");
  Serial.println(AD5933.getTemperature());

  
  
  // put your main code here, to run repeatedly:
  Serial.println("LEDs ON");
  for(int i=0;i<6;i++)
  {
    digitalWrite(leds[i],LOW);  //ON
    delay(200);
    digitalWrite(leds[i],HIGH);  //OFF
    delay(200);
  }

  delay(300);
 
  for(int i=0;i<6;i++)
  {
    digitalWrite(leds[i],LOW);  //All ON
  }
  delay(300);
  for(int i=0;i<6;i++)
  {
    digitalWrite(leds[i],HIGH);  //All Off
  }
  Serial.println("Please press SW2 (top button");
  while (digitalRead(PUSH1) == LOW );  //loop until SW2 is pressed
  
  
}
