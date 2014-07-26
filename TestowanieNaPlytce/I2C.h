
#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>


/*  Function takes an address of a device and sends the data to it.
	When an error occurs it returns the signature of the error so it can be easily used
	and sent via USART to PC in order to diagnose the problem.
*/


uint8_t I2C_PCF8574(uint8_t deviceAdr, uint8_t dispIndex)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); 
	while (!(TWCR & (1<<TWINT)));
	
	if ((TWSR & 0xF8) != TW_START)
	{
		return TWSR & 0xF8; 
	}
	
	TWDR = deviceAdr;	 //Load the pcf8574 adress to TWDR
	
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
	if ((TWSR & 0xF8) !=TW_MT_SLA_ACK)
	{
		return TWSR & 0xF8;
	}
	
	TWDR = ~dispIndex; // dispIndex is a byte of data to configure outputs of the expander. The invertion of this byte is caused by
					   // the usage of pnp transistors on board
	
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
	if ((TWSR & 0xF8) !=TW_MT_DATA_ACK)
	{
		return TWSR & 0xF8;
	}
	
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	
	return 0;
	 
}

uint8_t I2C_PCF8591(uint8_t deviceAdr, uint8_t PCF8591_CONTROL_BYTE, uint8_t *dataPacket)
{
	_delay_us(15);
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	 
	TWDR = deviceAdr;   // loads the address of ADC to TWDR
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	 
	TWDR = PCF8591_CONTROL_BYTE; // loads the configuration byte for ADC
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	 
	TWCR = (1<<TWSTO) | (1<<TWSTA) | (1<<TWINT) | (1<<TWEN); // restarts the transmission
	while (!(TWCR & (1<<TWINT)));
	_delay_us(15);
	
	TWDR = deviceAdr + 1; // loads the (address + 1) to indicate a demand for sending data from ADC
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	_delay_us(350);
	
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	*dataPacket = TWDR; // reads the data from ADC
	
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	
	return 0;
}
