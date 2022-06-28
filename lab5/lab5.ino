const int flex_index = A2;
const int flex_medium = A3;
const int flex_ring = A0;
const int flex_pinkie = A1;

#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

Adafruit_MPR121 cap = Adafruit_MPR121();

uint16_t lasttouched = 0;
uint16_t currtouched = 0;
int mySignal[] = {0,0,0,0};
// mySignal in position: 0 --> up, 1 --> down, 2 --> right, 3 --> left 

boolean check_sign = false, all_down=false; // mode=0 means we are in the relax position (all fingers are up)

float index, medium, ring, pinkie;

int index_high=190, medium_high=270, ring_high=230, pinkie_high=210;
int index_medium1_low=120, index_medium1_high=160, medium_medium1_low=200, medium_medium1_high=240, ring_medium1_low=140, ring_medium1_high=190, pinkie_medium1_low=130, pinkie_medium1_high=180;
int index_medium2_low=60, index_medium2_high=100, medium_medium2_low=110, medium_medium2_high=150, ring_medium2_low=70, ring_medium2_high=110, pinkie_medium2_low=65, pinkie_medium2_high=100;
int index_low=50, medium_low=80, ring_low=60, pinkie_low=55; // down will be status 2 or lower

int index_status=0, medium_status=0, ring_status=0, pinkie_status=0; // 0 means up, 1 means middle1, 2 means middle2, 3 means down

String letter = "-";

void setup() {
  
  Serial.begin(115200);
  pinMode(flex_index, INPUT);
  pinMode(flex_medium, INPUT);
  pinMode(flex_ring, INPUT);
  pinMode(flex_pinkie, INPUT);

  
  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  
}

void loop() {

  read_flex_signals();
  print_serial();
  //send_to_processing();

  // check if a sign is being done
  check_mode();

  // if a sign is being done, check which letter the user is writing 
  if(check_sign){
    read_letter_flex();
  }
  if(!check_sign){
    read_cap_signals();
    read_letter_cap();
  }
  
  Serial.println(letter);
  
  delay(300);

}

void read_flex_signals(){
  
  index = analogRead(flex_index);
  medium = analogRead(flex_medium);
  ring = analogRead(flex_ring);
  pinkie = analogRead(flex_pinkie);

  // up
  if(index>index_high){index_status=0;}
  if(medium>index_high){medium_status=0;}
  if(ring>index_high){ring_status=0;}
  if(pinkie>index_high){pinkie_status=0;}

  // middle1
  if(index>index_medium1_low & index<index_medium1_high){index_status=1;}
  if(medium>medium_medium1_low & medium<medium_medium1_high){medium_status=1;}
  if(ring>ring_medium1_low & ring<ring_medium1_high){ring_status=1;}
  if(pinkie>pinkie_medium1_low & pinkie<pinkie_medium1_high){pinkie_status=1;}

  // middle2
  if(index>index_medium2_low & index<index_medium2_high){index_status=2;}
  if(medium>medium_medium2_low & medium<medium_medium2_high){medium_status=2;}
  if(ring>ring_medium2_low & ring<ring_medium2_high){ring_status=2;}
  if(pinkie>pinkie_medium2_low & pinkie<pinkie_medium2_high){pinkie_status=2;}

  // down
  if(index<index_low){index_status=3;}
  if(medium<medium_low){medium_status=3;}
  if(ring<ring_low){ring_status=3;}
  if(pinkie<pinkie_low){pinkie_status=3;}

  
}

void print_serial(){

  Serial.print(index);
  Serial.print("  |  ");
  Serial.print(medium);
  Serial.print("  |  ");
  Serial.print(ring);
  Serial.print("  |  ");
  Serial.print(pinkie);
  Serial.print("  |  ");
  Serial.print("\n");

  Serial.print(index_status);
  Serial.print("  |  ");
  Serial.print(medium_status);
  Serial.print("  |  ");
  Serial.print(ring_status);
  Serial.print("  |  ");
  Serial.print(pinkie_status);
  Serial.print("  |  ");
  Serial.print("\n");
  

}

void check_mode(){

  if(index_status==0 & medium_status==0 & ring_status==0 & pinkie_status==0){
    // relax mode, all fingers are up 
    check_sign = false;
    letter = "-";
  }
  else{
    check_sign = true;
  }
}

void read_letter_flex(){
  
  if(index_status==0 & (medium_status==3 |  medium_status==2) & (ring_status==3 | ring_status==2) & (pinkie_status==3 | pinkie_status==2)){
    letter = "a";
  }
  if(index_status==2 & (medium_status==3 |  medium_status==2) & (ring_status==3 | ring_status==2) & (pinkie_status==3 | pinkie_status==2)){
    letter = "b";
  }
  if(index_status==1 & (medium_status==3 |  medium_status==2) & (ring_status==3 | ring_status==2) & (pinkie_status==3 | pinkie_status==2)){
    letter = "c";
  }
  if((index_status==3 | index_status==2) & medium_status==0 & ring_status==0 & pinkie_status==0){
    letter = "d";
  }
  if(index_status== 0 & (medium_status==3 |  medium_status==2) & (ring_status==3 | ring_status==2) & pinkie_status==0){
    letter = "e";
  }
  if(index_status==0 & (medium_status==3 |  medium_status==2) & ring_status==0 & pinkie_status==0){
    letter = "f";
  }
  if(index_status==1 & (medium_status==3 |  medium_status==2) & (ring_status==3 | ring_status==2) & pinkie_status==1){
    letter = "g";
  }
  if((index_status==3 | index_status==2) & medium_status==0 & (ring_status==3 | ring_status==2) & (pinkie_status==3 | pinkie_status==2)){
    letter = "h";
  }
  if((index_status==3 | index_status==2) & (medium_status==3 |  medium_status==2) & ring_status==0 & pinkie_status==0){
    letter = "i";
  }
  if(index_status==0 & medium_status==0 & (ring_status==3 | ring_status==2) & (pinkie_status==3 | pinkie_status==2)){
    letter = "j";
  }
  if(index_status==1 & medium_status==1 & (ring_status==3 | ring_status==2) & (pinkie_status==3 | pinkie_status==2)){
    letter = "u";
  }
  if((index_status==3 | index_status==2) & (medium_status==3 |  medium_status==2) & (ring_status==3 | ring_status==2) & pinkie_status==0){
    letter = "z";
  }
  if(index_status==3 & medium_status==3 & ring_status==3 & pinkie_status==3){
    letter = "space";
  }
  
}

void send_to_processing(){
  Serial.println(letter);
  
}

void read_cap_signals(){

  currtouched = cap.touched();
  
  for (uint8_t i=0; i<12; i++) {
    
    // if it *is* touched and *wasnt* touched before, change mySignal to 1
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      // pad touched 
      mySignal[i] = 1;
    }
    
    // if it *was* touched and now *isnt*, change mySignal to 0
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      // pad released 
      mySignal[i] = 0;
    }
  }

  // reset the state
  lasttouched = currtouched; 
}

void read_letter_cap(){

  if(mySignal[0]==1 & mySignal[1]==0 & mySignal[2]==0 & mySignal[3]==0){
    letter = "l";
  }
  if(mySignal[0]==0 & mySignal[1]==1 & mySignal[2]==0 & mySignal[3]==0){
    letter = "m";
  }
  if(mySignal[0]==0 & mySignal[1]==0 & mySignal[2]==1 & mySignal[3]==0){
    letter = "n";
  }
  if(mySignal[0]==0 & mySignal[1]==0 & mySignal[2]==0 & mySignal[3]==1){
    letter = "o";
  }
  if(mySignal[0]==1 & mySignal[1]==1 & mySignal[2]==0 & mySignal[3]==0){
    letter = "p";
  }
  if(mySignal[0]==0 & mySignal[1]==1 & mySignal[2]==1 & mySignal[3]==0){
    letter = "q";
  }
  if(mySignal[0]==0 & mySignal[1]==0 & mySignal[2]==1 & mySignal[3]==1){
    letter = "r";
  }
  if(mySignal[0]==1 & mySignal[1]==0 & mySignal[2]==0 & mySignal[3]==1){
    letter = "s";
  }
  if(mySignal[0]==0 & mySignal[1]==1 & mySignal[2]==0 & mySignal[3]==1){
    letter = "t";
  }
  if(mySignal[0]==1 & mySignal[1]==0 & mySignal[2]==1 & mySignal[3]==0){
    letter = "v";
  }
  if(mySignal[0]==1 & mySignal[1]==1 & mySignal[2]==1 & mySignal[3]==0){
    letter = "k";
  }
  if(mySignal[0]==0 & mySignal[1]==1 & mySignal[2]==1 & mySignal[3]==1){
    letter = "y";
  }
  if(mySignal[0]==1 & mySignal[1]==0 & mySignal[2]==1 & mySignal[3]==1){
    letter = "w";
  }
  if(mySignal[0]==1 & mySignal[1]==1 & mySignal[2]==0 & mySignal[3]==1){
    letter = "x";
  }
}
