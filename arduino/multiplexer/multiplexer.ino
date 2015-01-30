//Mux control pins
int as0 = 11;
int as1 = 10;
int as2 = 9;
int as3 = 8;

int bs0 = 7;
int bs1 = 6;
int bs2 = 5;
int bs3 = 4;

//Mux in "SIG" pin
int SIG_pinA = 0;
int SIG_pinB = 1;


void setup(){
  pinMode(as0, OUTPUT); 
  pinMode(as1, OUTPUT); 
  pinMode(as2, OUTPUT); 
  pinMode(as3, OUTPUT); 
  
  pinMode(bs0, OUTPUT); 
  pinMode(bs1, OUTPUT); 
  pinMode(bs2, OUTPUT); 
  pinMode(bs3, OUTPUT); 

  digitalWrite(as0, LOW);
  digitalWrite(as1, LOW);
  digitalWrite(as2, LOW);
  digitalWrite(as3, LOW);
  
  digitalWrite(bs0, LOW);
  digitalWrite(bs1, LOW);
  digitalWrite(bs2, LOW);
  digitalWrite(bs3, LOW);

  Serial.begin(9600);
}


void loop(){

  //Loop through and read all 16 values
  //Reports back "Value at channel 6 is: 346"
  for(int i = 0; i < 16; i ++){
    for(int j = i+1; j < 16; j ++){
      
      Serial.print("Electrode pair: ");
      Serial.print(i);
      Serial.print("-");
      Serial.print(j);
      Serial.print('\n');

      Serial.println(readMuxA(i));
      Serial.println(readMuxB(j));

      
      delay(5);   // Want this whole thing to go as fast as possible
      // Need nested loop to transition between pairs (1-2,1-3,1-4...15-12,15-13,15-14)
    }
  }

}


int readMuxA(int channel){
  int controlPin[] = {as0, as1, as2, as3};

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

  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }

  //read the value at the SIG pin
  int val = analogRead(SIG_pinA);

  //return the value
  return val;
}

int readMuxB(int channel){
  int controlPin[] = {bs0, bs1, bs2, bs3};

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

  //loop through the 4 sig
  for(int j = 0; j < 4; j ++){
    digitalWrite(controlPin[j], muxChannel[channel][j]);
  }

  //read the value at the SIG pin
  int val = analogRead(SIG_pinB);

  //return the value
  return val;
}

