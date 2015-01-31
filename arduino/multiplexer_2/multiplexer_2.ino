//Mux control pins
int sa0 = 3;
int sa1 = 4;
int sa2 = 5;
int sa3 = 6;
int sb0 = 8;
int sb1 = 9;
int sb2 = 10;
int sb3 = 11;
int enablea = 2;
int enableb = 7;

//Mux in "SIG" pin
int SIG_pin = 0;


void setup(){
  pinMode(enablea, OUTPUT);
  pinMode(sa0, OUTPUT); 
  pinMode(sa1, OUTPUT); 
  pinMode(sa2, OUTPUT); 
  pinMode(sa3, OUTPUT); 
  pinMode(enableb, OUTPUT);
  pinMode(sb0, OUTPUT); 
  pinMode(sb1, OUTPUT); 
  pinMode(sb2, OUTPUT); 
  pinMode(sb3, OUTPUT); 

  digitalWrite(enablea,HIGH);
  digitalWrite(sa0, LOW);
  digitalWrite(sa1, LOW);
  digitalWrite(sa2, LOW);
  digitalWrite(sa3, LOW);

  digitalWrite(enableb,HIGH);
  digitalWrite(sb0, LOW);
  digitalWrite(sb1, LOW);
  digitalWrite(sb2, LOW);
  digitalWrite(sb3, LOW);


  Serial.begin(9600);
}


void loop(){

  //Loop through and read all 16 values
  //Reports back "Value at channel 6 is: 346"
  for(int i = 0; i < 16; i ++){
    
    for(int j = i+1; j < 16; j ++){
      if( i != j )
      {
      
        /*
        Serial.print("Electrode pair: ");
        Serial.print(i);
        Serial.print("-");
        Serial.print(j);
        Serial.print('\n');

        Serial.println(readMux(i,j));
        */
        readMux(i,j);
        delay(1);   // Want this whole thing to go as fast as possible
      }
      // Need nested loop to transition between pairs (1-2,1-3,1-4...15-12,15-13,15-14)
    }
    for(int j = 0; j < i; j ++){
      if( i != j )
      {
      
        /*
        Serial.print("Electrode pair: ");
        Serial.print(i);
        Serial.print("-");
        Serial.print(j);
        Serial.print('\n');

        Serial.println(readMux(i,j));
        */
        readMux(i,j);
        delay(1);   // Want this whole thing to go as fast as possible
      }
      // Need nested loop to transition between pairs (1-2,1-3,1-4...15-12,15-13,15-14)
    }
  }

}


int readMux(int channela, int channelb){
  int controlPina[] = {sa0, sa1, sa2, sa3};
  int controlPinb[] = {sb0, sb1, sb2, sb3};


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

  digitalWrite(enablea,HIGH);
  digitalWrite(enableb,HIGH);
  
  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPina[i], muxChannel[channela][i]);
    digitalWrite(controlPinb[i], muxChannel[channelb][i]);
  }

  digitalWrite(enablea,LOW);
  digitalWrite(enableb,LOW);
  //read the value at the SIG pin
  //int val = analogRead(SIG_pin);

  //return the value
  return 0;
}

