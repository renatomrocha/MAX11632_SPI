/*****************************************************
Library for SPI communication with MAX11632 ADC using the arduino ICSP pins interface(MISO, MOSI and SCK).

Adapted from library for MAX31856 developed by Limor Fried/Ladyada for Adafruit Industries.

Author: Renato Rocha

e-mail: renatomrocha93@gmail.com

Last Update: 14-11-2018
********************************************************/


#include "MAX11632_SPI.h"
#ifdef __AVR
  #include <avr/pgmspace.h>
#elif defined(ESP8266)
  #include <pgmspace.h>
#endif

#include <stdlib.h>
#include <SPI.h>

static SPISettings max11632_spisettings = SPISettings(500000, MSBFIRST, SPI_MODE0);


// Hardware SPI init
MAX11632::MAX11632(int8_t spi_cs) {
  _sclk = _miso = _mosi = -1;
  _cs = spi_cs;
  }

//Initiates MAX11632 
boolean MAX11632::begin(void) {

  pinMode(EOC, INPUT);
  pinMode(_cs,OUTPUT);
  
  digitalWrite(_cs,HIGH);

    SPI.begin();

  return true;
}

//Function to send conf (see header file for available setups)
void MAX11632::send_conf(void) {

 requestRead(MAX11632_SETUP1);
	
}

//Reset FIFO
void MAX11632::reset_fifo(void) {

  requestRead(MAX11632_RESET_FIFO);
  
}

//Reads single channel, returns 16 bits with 12 bit conversion
uint16_t MAX11632::readSingleChannel(uint8_t n_channel) {
	
	max11632_channels command;
	command = selectChannel(n_channel,false);

	uint8_t buffer[2]={0,0};
	uint16_t An_val;
	float V_val;
	requestRead(command);
	
	SingleRead(buffer);
	
	An_val = buffer[0] << 8 | buffer[1];
	return An_val;
}


//Scans channels from 0 to n_channel
//Fills buffer with values read from FIFO
void MAX11632::readMultipleChannels(uint16_t buffer[], uint8_t n_channel) {

	max11632_channels command;
	
	command = selectChannel(n_channel , true);
	
	requestRead(command);
	
	MultipleRead(buffer , n_channel);

}





/********************************************** BASIC FUNCTIONS *******************************************************************************/

//Sends byte to MAX11632 to request channel reading
void MAX11632::requestRead(uint8_t data) {

  //Initiate SPI interface
  SPI.beginTransaction(max11632_spisettings); 

  //Activate chip select
  digitalWrite(_cs,LOW);
  
  
  delay(5);
  
  //Send data to registers
  spiSend(data); 

  
  //Deactivate CS
  digitalWrite(_cs,HIGH);
  
  //wait for EOC to go LOW (not working properly)
  while(EOC==HIGH){Serial.println("Converting...");}
  
  // Terminate CLK	
  SPI.endTransaction();

  }

  
//Reads set of 16 bits (single channel conversion = 0000 + 12 bits)
void MAX11632::SingleRead(uint8_t buffer[2]) {
   uint16_t rep;
  
   SPI.beginTransaction(max11632_spisettings); // Inicia ciclo de clock


  // Efetua chip Select
  digitalWrite(_cs,LOW);
  
  
  delay(5);
  
  spiReadSingle(buffer); // Envio de dados
	
 
   digitalWrite(_cs,HIGH);
   SPI.endTransaction();
   
  
}

//Reads multiple bytes from fifo according to number of channels requested
void MAX11632::MultipleRead(uint16_t buffer[], uint8_t n_channel) {
   uint16_t rep;
  
   SPI.beginTransaction(max11632_spisettings); // Inicia ciclo de clock


   // Efetua chip Select
  digitalWrite(_cs,LOW);
  
  delay(5);
  
  spiReadMultiple(buffer,n_channel); // Envio de dados
	
 
 digitalWrite(_cs,HIGH);
   SPI.endTransaction();
   
  
}

//Low level SPI read for single channel request
void MAX11632::spiReadSingle(uint8_t buffer[]){
	
	
	uint16_t mask = 0x0FFF;
	
	for(uint8_t i = 0; i<2;i++){
	buffer[i]= SPI.transfer(0x00);
	}
	


}  

//Low level SPI read for multiple channel request
void MAX11632::spiReadMultiple(uint16_t buffer[], uint8_t n_channel) {
	uint8_t var[2] = {0,0};
	
	uint16_t mask = 0x0FFF;
	for(uint8_t j=0; j<=n_channel;j++){
		for(uint8_t i = 0; i<2;i++){
		var[i]= SPI.transfer(0x00);
		}
	buffer[j]= var[0] << 8 | var[1];
	
	}
	

}  



//////////////////////////////////////////////////////////////////////////////////////////////////////////

//SPI byte send
void MAX11632::spiSend(uint8_t x) {
   
	SPI.transfer(x);
  
  }
 
// Function for conversion of channel number into proper register value 
 max11632_channels MAX11632::selectChannel(uint8_t channel, bool mult){
	max11632_channels command;
	
	if (!mult){
	switch(channel){
		case 0:
			command = MAX11632_RC_0;
			break;
		case 1:
			command = MAX11632_RC_1;
			break;
		case 2:
			command = MAX11632_RC_2;
			break;
		case 3:
			command = MAX11632_RC_3;
			break;
		case 4:
			command = MAX11632_RC_4;
			break;
		case 5:
			command = MAX11632_RC_5;
			break;
		case 6:
			command = MAX11632_RC_6;
			break;
		case 7:
			command = MAX11632_RC_7;
			break;
		case 8:
			command = MAX11632_RC_8;
			break;
		case 9:
			command = MAX11632_RC_9;
			break;
		case 10:
			command = MAX11632_RC_10;
			break;
		case 11:
			command = MAX11632_RC_11;
			break;
		case 12:
			command = MAX11632_RC_12;
			break;
		case 13:
			command = MAX11632_RC_13;
			break;
		case 14:
			command = MAX11632_RC_4;
			break;
	
	}
	}
	else{
		switch(channel){
		case 1:
			command = MAX11632_RCU_1;
			break;
		case 2:
			command = MAX11632_RCU_2;
			break;
		case 3:
			command = MAX11632_RCU_3;
			break;
		case 4:
			command = MAX11632_RCU_4;
			break;
		case 5:
			command = MAX11632_RCU_5;
			break;
		case 6:
			command = MAX11632_RCU_6;
			break;
		case 7:
			command = MAX11632_RCU_7;
			break;
		case 8:
			command = MAX11632_RCU_8;
			break;
		case 9:
			command = MAX11632_RCU_9;
			break;
		case 10:
			command = MAX11632_RCU_10;
			break;
		case 11:
			command = MAX11632_RCU_11;
			break;
		case 12:
			command = MAX11632_RCU_12;
			break;
		case 13:
			command = MAX11632_RCU_13;
			break;
		case 14:
			command = MAX11632_RCU_4;
			break;
	
	}
		
		
	}
	return command;
	
}


