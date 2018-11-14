#include "MAX11632_SPI.h"

//Initiate MAX11632 with desired CS PIN
//MISO,MOSI and SCK must be the ones from ICSP interface

MAX11632 MAX_ADC = MAX11632(16);

int val;
int channel = 0;
void setup() {
Serial.begin(115200);
 
MAX_ADC.begin();
}

void loop() {
  
  val = MAX_ADC.readSingleChannel(channel);
  delay(100);

}
