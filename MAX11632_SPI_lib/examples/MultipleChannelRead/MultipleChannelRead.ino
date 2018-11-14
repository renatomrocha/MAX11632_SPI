/*Example code to scan 5 channels from 0-4 on MAX11632 with single command
Author: Renato Rocha
*/

#include "MAX11632_SPI.h"

//Initiate MAX11632 with desired CS PIN
//MISO,MOSI and SCK must be the ones from ICSP interface

MAX11632 MAX_ADC = MAX11632(16);

uint16_t buff[6]; // Create buffer with size equal to number of channels to read

void setup() {
  Serial.begin(115200);
  MAX_ADC.begin();                               

}


void loop() {

  MAX_ADC.readMultipleChannels(buff,5); 

  Serial.println("/////////////////");
  Serial.println(buff[0]);
  Serial.println("-----");
  Serial.println(buff[1]);
  Serial.println("-----");
  Serial.println(buff[2]);
  Serial.println("-----");
  Serial.println(buff[3]);
  Serial.println("-----");
  Serial.println(buff[4]);
 
  Serial.println("//////////////////");
  
  delay(1000);
 


}



