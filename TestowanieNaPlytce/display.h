#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
 
#define PCF8574ADR 0x40 //The older nibble contains a built-in device's address. The younger one consists of 3 user defined bits (set to zero in this case) and R/W bit.
#define PCF8591ADR 0x90


#define REFERENCEVOLTAGE 5090      // reference voltage of 4.096v 

#define CH0_DIV_FACTOR 1	        // 'Channel 0 division factor'
#define CH1_DIV_FACTOR 2
#define CH2_DIV_FACTOR 4
#define CH3_DIV_FACTOR 4
 
#define ENCODER_BUTTON 0b00001000

#define BUTTON_ONE 0b10000000 // Monostable buttons
#define BUTTON_TWO 0b01000000
#define BUTTON_THREE 0b00000001

#define LED_0 0b01000000
#define LED_1 0b10000000

/* 
	_____0_____
	|		  |
	1		  3
	|____2____|		A visualization of 7-segment display segments arrangement, labeled appropriately to the datasheet of the device with numbers 0-7				
	|		  |
	4		  5
	|____6____|(7) <-- decimal point
*/


void waitFunction(uint8_t timeToWait) // When timeToWait is 1 the delay is about 0.5s. timeToWait factor multiplies this delay.
{
	
}


void LEDFunction(uint8_t whichLed, uint8_t blinkingTime, uint8_t blinkingFreq, void (*waitFunction)(uint8_t blinkingFreq))
{
	uint8_t aux;
	for(aux = 0; aux<blinkingTime ; aux++);
	PORTD |= whichLed;
	waitFunction(blinkingFreq);
	PORTD &= ~(0x00 | whichLed); // turning off the diode.
}


uint8_t displayFunction(char finalValue, uint8_t (*I2C_PCF8574)(uint8_t deviceAdr, uint8_t dispIndex), uint8_t dispIndex)
	/* Displays one char (finalValue) on a desired display selected by dispIndex. The switch-case instructions may be
	   changed regarding the pinout of 7-seg. display.                                                                   */

{	
		uint8_t error = I2C_PCF8574(PCF8574ADR, dispIndex); // Turns on the desired display. Returns the error once it occurs. 
		if(error) return error;
		
		switch(finalValue)
		{
			case 0:
			PORTB = ~( _BV(PB2) | _BV(PB3) | _BV(PB4) | _BV(PB5) );
			PORTC = ~( _BV(PC0) | _BV(PC1) | _BV(PC2) );
			break;
			
			case 1:
			PORTB = ~_BV(PB2);
			PORTC = ~_BV(PC2);
			break;
			
			case 2:
			PORTB = ~( _BV(PB1) | _BV(PB3) | _BV(PB4) | _BV(PB5) );
			PORTC = ~( _BV(PC1) | _BV(PC2) );
			break;
			
			case 3:
			PORTB = ~( _BV(PB1) | _BV(PB2) | _BV(PB3) | _BV(PB5) );
			PORTC = ~( _BV(PC1) | _BV(PC2) );
			break;
			
			case 4:
			PORTB = ~( _BV(PB1) | _BV(PB2) | _BV(PB5) );
			PORTC = ~( _BV(PC0) | _BV(PC2) );
			break;
			
			case 5:
			PORTB = ~( _BV(PB1) | _BV(PB2) | _BV(PB3)  | _BV(PB5) );
			PORTC = ~( _BV(PC0) | _BV(PC1) );
			break;
			
			case 6:
			PORTB = ~( _BV(PB1) | _BV(PB2) | _BV(PB3) | _BV(PB4) | _BV(PB5) );
			PORTC = ~( _BV(PC0) | _BV(PC1) );
			break;
			
			case 7:
			PORTB = ~( _BV(PB2) );
			PORTC = ~( _BV(PC2) | _BV(PC1) );
			break;
			
			case 8:
			PORTB = ~( _BV(PB1) | _BV(PB2) | _BV(PB3) | _BV(PB4) | _BV(PB5) );
			PORTC = ~( _BV(PC0) | _BV(PC1) | _BV(PC2) );
			break;
			
			case 9:
			PORTB = ~( _BV(PB1) | _BV(PB2) | _BV(PB3)  | _BV(PB5) );
			PORTC = ~( _BV(PC0) | _BV(PC1) | _BV(PC2) );
			break;
		}
		 
		return 0;
		 
}



uint8_t displayChar(char finalValue, uint8_t (*I2C_PCF8574)(uint8_t deviceAdr, uint8_t dispIndex), uint8_t deviceAdr, uint8_t dispIndex)
{

}








uint8_t calibrationFunction()
{
	int8_t calibrationData = 0;
	uint8_t selector_1, selector_2;
	
	if(PINB & BUTTON_ONE)
	selector_1 = 0;
	else
	selector_1 = 1;
	
	if(PINB & BUTTON_TWO)
	selector_2 = 2;
	else
	selector_2 = 3;
	
	while(PINB & BUTTON_THREE) 
	
	
	
	
	calibrationData = 1 ; //zmiana wartosci enkoderem dopoki nie wycisne przycisku
	
	return calibrationData;
	
}

void binaryToVoltageConverter(uint8_t dataPacket, int8_t *calibrationData, uint8_t channelSelector, uint8_t *finalValue) // turns a byte of data (dataPacket) into array of chars making them easier to process 
	/* Converts one byte of data into final voltage. Function uses channelSelector to decide which division
	   factor and calibration factor needs to be used for the appropriate reading. decimalPointShifter allows to
	   avoid computing float type numbers. The position of the decimal point is set by displayFunction.					 */


{
	uint8_t currentDivFactor = 1;
	uint8_t decimalPointShifter = 100; //used to avoid calculating float type integers
	int8_t calibrationFactor = 0;  
	
	/*switch(channelSelector) // sets a proper division factor (currentDivFactor) and a calibration factor depending on the channel being processed
		{
			case 0x00:
			currentDivFactor = CH0_DIV_FACTOR;
			calibrationFactor = calibrationData[0];
			break;
				
			case 0x01:
			currentDivFactor = CH1_DIV_FACTOR;
			calibrationFactor = calibrationData[1];
			break;
			
			case 0x02:
			currentDivFactor = CH2_DIV_FACTOR;
			calibrationFactor = calibrationData[2];
			break;
			
			case 0x03:
			currentDivFactor = CH3_DIV_FACTOR;
			calibrationFactor = calibrationData[3];
			break;
		}*/
	// wersja uproszczona. operacje na floatach.
	 
	
	uint32_t resultVoltage = (  ( dataPacket*decimalPointShifter  / 255UL ) * REFERENCEVOLTAGE * 2); //currentDivFactor
								/*	-datapacket = 1 byte of data received from a specific channel of ADC. 
									-decimalPointShifter = a multiplier of a value of 100 which shifts the 
									 decimal point by 3 places to the right, preventing from calculating 
									 float type numbers.
									-deviding by 256, which is the decimal representation of an 8bit ADC resolution
									-REFERENCEVOLTAGE = a stable voltage delivered to the ADC by hardware
									-currentDivFactor = a factor used to calculate voltage for different voltage ranges.
									-REFERENCEVOLTAGE isn't equal to 4.096 in this equation. It's 4096 to avoid floats.
								*/
	uint8_t loop;
	uint32_t factor;
	
	for(loop = 0, factor = 1000000; loop < 4 ; loop++, resultVoltage = (resultVoltage % factor), factor = factor/10 )
	{	
		finalValue[loop] = resultVoltage/factor;		
	}
}
