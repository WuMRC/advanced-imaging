/*
Performs multiplexing to measure the impedance value between every nodes
It uses one MUX. It was just first-phase testing routine.
It uses manual calibration method without using any MUX.

Using Arduino Uno /  TI MUXes (Tomography, CD74HC4067) / AD5933

Author: Il-Taek Kwon (Library, Program)
(Adapted little portion of Daniel Johnson's code and comments)

*/

#define TWI_FREQ 400000L // Setting TWI/I2C Frequency to 400MHz to speed up.

#define VERBOSE 0 //Toggles verbose debugging output via the serial monitor.
                  //1 = On, 0 = Off.

#define cycles_base 511      //First term to set a number of cycles to ignore
                             //to dissipate transients before a measurement is
                             //taken. The max value for this is 511.


#define cycles_multiplier 4  //Set a multiple for the cycles_base which
                             //is used to calculate the desired number
                             //of settling cycles. Values can be 1, 2, or 4.
                             

#define start_frequency 50000 //Set the start frequency, the only one of
                              //interest here(50 kHz).

#define cal_resistance 1841 //Set a calibration resistance for the gain
                            //factor. This will have to be measured before any
                            //other measurements are performed.
                           
#define cal_samples 10 //Set a number of measurements to take of the calibration
                       //resistance. These are used to get an average gain
                       //factor.
/*
 * Those six lines of preprocessing codes are for writing bit on the mircocessor register. 
 * However, they are not used actually, so you can ignore. They could be deleted without affcting any functionality.
 * Refer http://playground.arduino.cc/Main/AVR
 */
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif                            

double gain_factor = 0; // Global Variable for Gain Factor (see datasheet)

/* Constants for MUX control pins.
 *  Pins can be assigned as you want.
 */
const int MUX_EN = 2, MUX_S0 = 3, MUX_S1 = 4, MUX_S2 = 5, MUX_S3 = 6; 

int lt1=0; // Memory for current mux channel
boolean setMUX(byte);

#include <Wire.h> //Library for I2C communications
#include "AD5933.h" //Library for AD5933 functions (must be installed)
//#include <AltSoftSerial.h>
//AltSoftSerial altSerial;

void setup()
{
  Serial.begin(115200); //Initialize serial communication for debugging.
  //altSerial.begin(38400);
  #if VERBOSE
  Serial.println("Program start!");
  delay(100);
  #endif
  
  pinMode(MUX_EN, OUTPUT);
  pinMode(MUX_S0, OUTPUT);
  pinMode(MUX_S1, OUTPUT);
  pinMode(MUX_S2, OUTPUT);
  pinMode(MUX_S3, OUTPUT);
  // Setting pin mode
  
  digitalWrite(MUX_EN, HIGH);
  digitalWrite(MUX_S0, HIGH);
  digitalWrite(MUX_S1, HIGH);
  digitalWrite(MUX_S2, HIGH);
  digitalWrite(MUX_S3, HIGH);
  // Turned off MUX & Initial setting for channel 3
  
  //TWBR=1;
  //cbi(TWSR, TWPS0);
  //cbi(TWSR, TWPS1);
  Wire.begin();
  //TWBR=1;
  cbi(TWSR, TWPS0);
  cbi(TWSR, TWPS1);
  delay(100);
  
  AD5933.setExtClock(false);
  //[A.X] Send a reset command to the AD5933.
  if(AD5933.resetAD5933() == true)
  {
    #if VERBOSE
    Serial.println("Reset command sent.");
    delay(100);
    #endif
  }
  else
  {
    #if VERBOSE
    Serial.println("Error sending reset command!");
    delay(1000);
    #endif
  }
  //End [A.X]
  
  //--- A. Initialization and Calibration Meassurement ---
  
  //[A.0] Set the clock for internal/external frequency
  //Set the operational clock to internal
  //AD5933.setExtClock(0);
  
  //[A.1] Set the measurement frequency
  if (AD5933.setStartFreq(start_frequency) == true)
  {
    #if VERBOSE
    Serial.print("Start frequency set to: ");
    Serial.print(start_frequency);
    Serial.println (" Hz.");
    delay(100);
    #endif
  }
  else
  {
    #if VERBOSE
    Serial.print("Error setting start frequency!");
    delay(1000);
    #endif
  }
  //End [A.1]


  //[A.2] Set a number of settling time cycles
  if (AD5933.setSettlingCycles(cycles_base, cycles_multiplier) == true)
  {
    #if VERBOSE
    Serial.print("Settling cycles set to: ");
    Serial.print(cycles_base*cycles_multiplier);
    Serial.println(" cycles.");
    delay(100);
    #endif
  }
  else
  {
    #if VERBOSE
    Serial.print("Error setting settling cycles!");
    delay(1000);
    #endif
  }
  //End [A.2]
  
  AD5933.setIncrementinHex(1);
  AD5933.setNumofIncrement(2);
  AD5933.setVolPGA(0,1); // 0 - 2Vpp, 1 - 0.2Vpp
  
  double temp = AD5933.getTemperature();
  #if VERBOSE
  Serial.print("Temperature is ");
  Serial.print(temp);
  Serial.println(" degree celcius.");
  #endif
  
  //[A.3] Calculate the gain factor (needs cal resistance, # of measurements)
  //Note: The gain factor finding function returns the INVERSE of the factor
  //as defined on the datasheet!
  gain_factor = AD5933.getGainFactor(cal_resistance, cal_samples, false);
  if (gain_factor != -1)
  {
    #if VERBOSE
    Serial.print("Gain factor (");
    Serial.print(cal_samples);
    Serial.print(" samples) is: ");
    Serial.println(gain_factor);
    delay(100);
    #endif
  }
  else
  {
    #if VERBOSE
    Serial.print("Error calculating gain factor!");
    delay(1000);
    #endif
  } 
  //End [A.3]

}

void loop()
{
  //--- B. Repeated single measurement ---
  //Gain factor calibration already sets the frequency, so just send 
  //repeat single magnitude capture command.
  
  // Temperature update inside/outside of chip.
  #if VERBOSE
  double temp = AD5933.getTemperature();
  Serial.print("Temperature is ");
  Serial.print(temp);
  Serial.println(" degree celcius.");
  #else
  AD5933.tempUpdate();
  #endif
  
  //[B.1] Issue a "repeat frequency" command.
  #if VERBOSE
  if (AD5933.setCtrMode(REPEAT_FREQ) == true)
  {
    Serial.println("Repeat_Frequency command sent.");
    delay(100);
  }
  else
  {
    Serial.println("Error sending Repeat_Frequency command!");
    delay(1000);
  }
  #else
  setMUX(lt1); // Routing the multiplexer
  delay(100);
  AD5933.setCtrMode(REPEAT_FREQ);
  #endif
  //End [B.1]
   
  #if VERBOSE
  //[B.2.1] Capture the magnitude from real & imaginary registers.
  double Z_magnitude = AD5933.getMagOnce();
  Serial.print("Magnitude found to be: ");
  Serial.println(Z_magnitude);
  delay(100);
  //End [B.2.1]
  
  //[B.2.2] Calculate the impedance using the magnitude and gain factor.
  double Z_value = gain_factor/Z_magnitude;
  Serial.print("Impedance found to be: ");
  Serial.print(Z_value);
  Serial.println(" Ohms.");
  delay(100);
  //End [B.2.2]
  
  #else
  double Z_value = gain_factor/AD5933.getMagOnce();
  // Shorten version
  #endif
  
  Serial.print(millis() / 1000.0);  // Timestamp in seconds
  
  Serial.print("\t");
  Serial.print("MUX: ");
  Serial.print(lt1); // Prining channel
  
  //[B.2.3] Output the impedance value (serial, array, etc.) 
  
  //Serial.print(" Impedance = ");
  Serial.print("\t");
  Serial.print(Z_value);
  //Serial.println(" Ohms.");
  Serial.println();  
  //End [B.3]
  
  // --- End B ---

  lt1++; // Going to next channel
  if(lt1 > 15)
    lt1=0; // Reset channel to zero if exceeded maximum
  #if VERBOSE
  Serial.println("End Loop!");
  #endif
}

/*
 * Function of setting and routing MUX
 * byte channel - 0~15 - Channel #
 */
boolean setMUX(byte channel)
{
  
  int controlPin[] = {MUX_S0, MUX_S1, MUX_S2, MUX_S3};
  // Control pin numbers are saved on controlPin array

  int muxChannel[16][4]={
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
  // Saving combination of pin inputs for each channel to close

  digitalWrite(MUX_EN,HIGH); // Turning off MUX
  
  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
    
  }

  digitalWrite(MUX_EN,LOW); // Turning on MUX
  return true; // meaning nothing; saying that it worked well.

}

