/*
 * TestowanieNaPlytce.c
 *
 * Created: 2014-05-06 16:40:19
 *  Author: Radek
 */ 


#include <avr/io.h>
#include <display.h>
#include <I2C.h>
#include <util/twi.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdio.h>

#define F_CPU 8000000

 //<the same as above>

#define BUTTON_ONE 0b10000000 // Monostable buttons
#define BUTTON_TWO 0b01000000 
#define BUTTON_THREE 0b00000001 // Monostable button, pushable preferably.

										 
int main(void)
{
	uint8_t bitMaskArray[4] = {0b00000000, 0b00000001, 0b00000010, 0b00000011}; // bit masks being added to PCF8591_CONTROL_BYTE to switch between channels to be read appropriately 
																				// first element of the array would select the first channel and so on.
	
	const uint8_t PCF8591_CONTROL_BYTE = 0b00000000;		/* Initial control byte configuration. Starting from LSB:
																- 0,1 = combination of both determines the choose of the input to be read. 
																- 2   = auto-increment flag. In this application always zeroed
																- 3   = according to the datasheet, always zeroed
																- 4,5 = combination of those determines: single-ended or differential inputs.
																		In this app - always zeroed.
																- 6   = enables the analog output of the converter which isn't used here. Always zeroed.
																- 7   = according to the datasheet, always zeroed
														    */			
														
	
	 
	int8_t calibrationData[4] = {0};
	 
	
	DDRD = 0xff;
	DDRC = 0xff;
	DDRB = 0xff;
	
	uint8_t i = 201;

	
   while(1)
    {					
		/*uint8_t loopAux_0;
		for(loopAux_0 = 0 ; loopAux_0 < 2 ; loopAux_0++)
		{
			uint8_t selector = 0;
			
			switch(loopAux_0)
			{
				case 0:
					if(PINB & BUTTON_ONE)
						selector = 0;
					else
						selector = 1;
					break;
				
				case 1:
					if(PINB & BUTTON_TWO)
						selector = 2;
					else
						selector = 3;
					break;
			}*/
			uint8_t finalValue[4] = {0};
			uint8_t dataPacket = 0;
			
			
				
			while(!(PINB & BUTTON_THREE)) //Enabling the calibration mode;
			{	
				PORTD |= LED_1;
				
				calibrationData[selector] = calibrationFunction();
				
			}
			
			uint8_t error = I2C_PCF8591(PCF8591ADR, PCF8591_CONTROL_BYTE | bitMaskArray[0], &dataPacket); 
			binaryToVoltageConverter(dataPacket, calibrationData, 0x00, finalValue);
			uint8_t loopAux_1, dispIndex;
			for(loopAux_1 = 0, dispIndex = 1 ; loopAux_1 < 4 ; loopAux_1++, dispIndex = dispIndex*2) //lepiej przesuniêcie bitowe
			{
					displayFunction(finalValue[loopAux_1], I2C_PCF8574, dispIndex);
					_delay_ms(5);
			}
			
			//PORTB = 0xff;
			//PORTC = 0xff;
			
		}
   
}