/*
Performs multiplexing to measure the impedance value between every nodes
I also uses ADG809 to autowiring for calibration.
It calibrates automatically without any hassle.

Using Arduino MEGA / ADG809(Calibration MUX) / TI MUXes (Tomography, CD74HC4067) / AD5933

Author: Il-Taek Kwon (Library, Program)
(Adapted little portion of Daniel Johnson's code)

*/


#define TWI_FREQ 400000L // Setting TWI/I2C Frequency to 400MHz to speed up.

#define VERBOSE 0 //Toggles verbose debugging output via the serial monitor.
                  //1 = On, 0 = Off.
                  
#define ENABLE_RECALIBRATION 1
// Set whether to recalibrate or not
// 1 = On, 0 = Off

#define cycles_base 511      //First term to set a number of cycles to ignore
                             //to dissipate transients before a measurement is
                             //taken. The max value for this is 511.


#define cycles_multiplier 4  //Set a multiple for the cycles_base which
                             //is used to calculate the desired number
                             //of settling cycles. Values can be 1, 2, or 4.
                             

#define start_frequency 50000 //Set the start frequency, the only one of
                              //interest here(50 kHz).

#define cal_resistance 806  //Set a calibration resistance for the gain
                            //factor. This will have to be measured before any
                            //other measurements are performed.
                           
#define cal_samples 10 //Set a number of measurements to take of the calibration
                       //resistance. These are used to get an average gain
                       //factor.
#define MAX_PAIR 15                       
                       
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif                            
//---

// ================================================================
// MUX Control pins
// ================================================================
const byte MUX_A_S0 = 23; const byte MUX_A_S1 = 24; const byte MUX_A_S2 = 25; const byte MUX_A_S3 = 26;        // MUX A
const byte MUX_B_S0 = 28; const byte MUX_B_S1 = 29; const byte MUX_B_S2 = 30; const byte MUX_B_S3 = 31;      // MUX B
const byte MUX_A_EN = 22; const byte MUX_B_EN = 27;
const byte MUX_CAL_EN = 2; const byte MUX_CAL_A0 = 3; const byte MUX_CAL_A1 = 4;

bool setMUX(int, int);
bool switchCalibrationMUX(byte);
double gainFactor = 0, globalShift = 0;
bool performCalibration();

#include <Wire.h> //Library for I2C communications
#include "AD5933.h" //Library for AD5933 functions (must be installed)


void setup() {
  Serial.begin(9600); //Initialize serial communication for debugging.
  
  // ================================================================
  // For MUX
  // ================================================================
  pinMode(MUX_A_S0, OUTPUT); pinMode(MUX_A_S1, OUTPUT); pinMode(MUX_A_S2, OUTPUT); pinMode(MUX_A_S3, OUTPUT); 
  pinMode(MUX_B_S0, OUTPUT); pinMode(MUX_B_S1, OUTPUT); pinMode(MUX_B_S2, OUTPUT); pinMode(MUX_B_S3, OUTPUT); 
  pinMode(MUX_A_EN, OUTPUT); pinMode(MUX_B_EN, OUTPUT); pinMode(MUX_CAL_EN, OUTPUT); pinMode(MUX_CAL_A0, OUTPUT);
  pinMode(MUX_CAL_A1, OUTPUT);

  digitalWrite(MUX_A_S0, LOW); digitalWrite(MUX_A_S1, LOW); digitalWrite(MUX_A_S2, LOW); digitalWrite(MUX_A_S3, LOW);
  digitalWrite(MUX_B_S0, LOW); digitalWrite(MUX_B_S1, LOW); digitalWrite(MUX_B_S2, LOW); digitalWrite(MUX_B_S3, LOW);
  digitalWrite(MUX_A_EN,HIGH); digitalWrite(MUX_B_EN,HIGH);
  
  switchCalibrationMUX(1);

  //TWBR=1;
  //cbi(TWSR, TWPS0);
  //cbi(TWSR, TWPS1);
  Wire.begin();
  //TWBR=1;
  cbi(TWSR, TWPS0);
  cbi(TWSR, TWPS1);
  delay(100);
  
  //[A.0] Set the clock for internal/external frequency
  //Set the operational clock to internal
  AD5933.setExtClock(false);
  
  //[A.X] Send a reset command to the AD5933.
  AD5933.resetAD5933();
  
  //[A.1] Set the measurement frequency
  AD5933.setStartFreq(start_frequency);

  //[A.2] Set a number of settling time cycles
  AD5933.setSettlingCycles(cycles_base, cycles_multiplier);
  
  AD5933.setStepSizeInHex(1);
  AD5933.setNumofIncrement(2);
  AD5933.setPGA(GAIN_1);
  AD5933.setRange(RANGE_4);
  
  double temp = AD5933.getTemperature();
#if VERBOSE
  Serial.print("Temperature is ");
  Serial.print(temp);
  Serial.println(" degree celcius.");
#endif
  
  Serial.println("Calibration Start! - Switching MUX");
  performCalibration();
  
  Serial.println("Experiment Program");
  Serial.println();
  Serial.println("Purpose: To analyze how the injected signal changes via MUXes");
  Serial.println("During the loop, following instructions are allowed. - N/A");
  
  Serial.println("Switching to measure. Measurement Starting!");

  
}

void loop() {
  static byte lt1=1, lt2 = 0;
  //--- B. Repeated single measurement ---
  //Gain factor calibration already sets the frequency, so just send 
  //repeat single magnitude capture command.
  
#if VERBOSE
  double temp = AD5933.getTemperature();
  Serial.print("Temperature is ");
  Serial.print(temp);
  Serial.println(" degree celcius.");
#else
  AD5933.tempUpdate();
#endif
  
  //[B.1] Issue a "repeat frequency" command.
  setMUX(lt1,lt2);
  delay(50);
  AD5933.setCtrMode(REPEAT_FREQ);
  delay(50);
  //End [B.1]
   
  double Z_value = -1, phaseVal = -10;  
  AD5933.getComplex(gainFactor, globalShift, Z_value, phaseVal); 
  
  Serial.print(millis());
  Serial.print("\t");
  Serial.print("Pair ");
  Serial.print(lt1);
  Serial.print(" - ");
  Serial.print(lt2);
  Serial.print('\t');
  
  //[B.2.3] Output the impedance value (serial, array, etc.) 
  
  Serial.print("\t");
  Serial.print(Z_value);
  //Serial.println(" Ohms.");
  Serial.print("\t");
  Serial.print(phaseVal);
  Serial.println();
  
  //End [B.3]
  
  // --- End B ---
  
  do{
    
    lt1++;
    if(lt1 > MAX_PAIR)
    {
      lt1=0;
      lt2++;
      if(lt2 > MAX_PAIR)
        lt2 = 0;
    }
  }while(lt1 == lt2);
 
#if ENABLE_RECALIBRATION  
  if( lt1 == 1 && lt2 == 0)
      performCalibration();
#endif

#if VERBOSE
  Serial.println("End Loop!");
#endif
  
}

bool performCalibration()
{
  //[A.3] Calculate the gain factor (needs cal resistance, # of measurements)
  //Note: The gain factor finding function returns the INVERSE of the factor
  //as defined on the datasheet!
  
  switchCalibrationMUX(1);
  AD5933.getGainFactor(cal_resistance, cal_samples, gainFactor, globalShift, false);
#if VERBOSE
  if (gainFactor != -1)
  {
    Serial.print("Gain factor (");
    Serial.print(cal_samples);
    Serial.print(" samples) is: ");
    Serial.println(gainFactor);
    delay(100);
  }
  else
  {
    Serial.print("Error calculating gain factor!");
    delay(1000);
  } 
#endif
  switchCalibrationMUX(4);
  //End [A.3]
   
}

// num2swt: 1 to 4
bool switchCalibrationMUX(byte num2swt)
{
  digitalWrite(MUX_CAL_EN, LOW);
  num2swt--;
  if( num2swt % 2 == 0 )
    digitalWrite(MUX_CAL_A0, LOW);
  else
    digitalWrite(MUX_CAL_A0, HIGH);
  
  if( (num2swt > 1) % 2 == 0 )
    digitalWrite(MUX_CAL_A1, LOW);
  else 
    digitalWrite(MUX_CAL_A1, HIGH);
    
  digitalWrite(MUX_CAL_EN, HIGH);
  return true;
}

bool setMUX(byte channela, byte channelb){
  byte controlPina[] = {MUX_A_S0, MUX_A_S1, MUX_A_S2, MUX_A_S3};
  byte controlPinb[] = {MUX_B_S0, MUX_B_S1, MUX_B_S2, MUX_B_S3};

  byte muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };

  digitalWrite(MUX_A_EN,HIGH);
  digitalWrite(MUX_B_EN,HIGH);
  
  //loop through the 4 sig
  for(byte i = 0; i < 4; i ++){
    digitalWrite(controlPina[i], muxChannel[channela][i]);
    digitalWrite(controlPinb[i], muxChannel[channelb][i]);
  }

  digitalWrite(MUX_A_EN,LOW);
  digitalWrite(MUX_B_EN,LOW);
  //read the value at the SIG pin
  
  return true; // Do not return meaningful data.
}
