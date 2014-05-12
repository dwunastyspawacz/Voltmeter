#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#define REFERENCEVOLTAGE 4096       // reference voltage of 4.096v 
#define CH0_DIV_FACTOR 1	        // 'Channel 0 division factor'
#define CH1_DIV_FACTOR 2
#define CH2_DIV_FACTOR 4
#define CH3_DIV_FACTOR 4
#define LED_REGISTER PORTB

#define ZERO     0   // Assigning specific configurations of pins to the numbers or letters they create on the display
#define ONE		 0				// to make them easier to use in the program
#define TWO	     0
#define THREE    0
#define FOUR     0
#define FIVE     0
#define SIX      0
#define SEVEN    0
#define EIGHT    0
#define NINE     0

#define C		 0			
#define A        0
#define L        0

#define E        0
#define R        0

#define P        0
#define O        0
#define G        0

#define PORTB_BIT_MASK 0b00111110 // Since the control of segments is splitted to two registers bit-mask allows to split one byte of data
#define PORTC_BIT_MASK 0b00000111 // to be displayed, to two registers

#define ENCODER_BUTTON 0b00001000

#define BUTTON_ONE 0b10000000 // Monostable buttons
#define BUTTON_TWO 0b01000000
#define BUTTON_THREE 0b00000001


/* 
	_____0_____
	|		  |
	1		  3
	|____2____|		A visualization of 7-segment display segments arrangement, labeled appropriately to the datasheet of the device with numbers 0-7				
	|		  |
	4		  5
	|____6____|(7)    
*/



void displayFunction(char finalValue, void (*I2C_PCF8574)(uint8_t deviceAdr, uint8_t dispIndex, uint8_t *errorBuffer), uint8_t deviceAdr, uint8_t dispIndex, uint8_t *errorBuffer)
{	
		I2C_PCF8574(deviceAdr, dispIndex, errorBuffer);
		PORTB = (finalValue & PORTB_BIT_MASK); // Maybe consider 
		PORTC = (finalValue & PORTC_BIT_MASK);
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

void binaryToVoltageConverter(uint8_t dataPacket, int8_t *calibrationData, uint8_t channelSelector, char *finalValue) // turns a byte of data (dataPacket) into array of chars making them easier to process 
{
	uint8_t currentDivFactor = 1;
	uint8_t decimalPointShifter = 100; //used to avoid calculating float type integers
	int8_t calibrationFactor = 0;  
	
	switch(channelSelector) // sets a proper division factor (currentDivFactor) and a calibration factor depending on the channel being processed
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
		}
	
	
	uint32_t resultVoltage = ( ( (dataPacket*decimalPointShifter) / 256 ) * (REFERENCEVOLTAGE + calibrationFactor) * currentDivFactor );
								/*	-datapacket = 1 byte of data received from a specific channel of ADC. 
									-decimalPointShifter = a multiplier of a value of 100 which shifts the 
									 decimal point by 3 places to the right, preventing from calculating 
									 float type numbers.
									-deviding by 256, which is the decimal representation of 8bit ADC resolution
									-REFERENCEVOLTAGE = a stable voltage delivered to the ADC by hardware
									-currentDivFactor = a factor used to calculate voltage for different voltage ranges.
									-REFERENCEVOLTAGE isn't equal to 4.096 in this equation. It's 4096 to avoid floats.
								*/

	ltoa(resultVoltage, finalValue, 4);
}
