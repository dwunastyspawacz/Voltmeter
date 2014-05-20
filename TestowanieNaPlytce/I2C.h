
#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#define F_CPU 8000000

#define I2C_ERROR 0

/*  Function takes an address of a device and sends the data to it.
	When an error occurs it returns the signature of the error so it can be easily used
	and sent via USART to PC in order to diagnose the problem.
*/


uint8_t I2C_PCF8574(uint8_t deviceAdr, uint8_t dispIndex)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); 
	while (!(TWCR & (1<<TWINT)));
	
	/*if ((TWSR & 0xF8) != TW_START)
	{
		return TW_START; 
	}*/
	
	TWDR = deviceAdr;	 //Load the i/o expander to TWDR
	
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
	/*if ((TWSR & 0xF8) !=TW_MT_SLA_ACK)
	{
		return TW_MT_SLA_ACK;
	}*/
	
	TWDR = ~dispIndex;
	
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
	/*if ((TWSR & 0xF8) !=TW_MT_DATA_ACK)
	{
		return TW_MT_DATA_ACK;
	}*/
	
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	 
}






uint8_t I2C_PCF8591(uint8_t deviceAdr, uint8_t PCF8591_CONTROL_BYTE, uint8_t *dataPacket)
{
	
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // PRZEBUDOWAC FUNKCJE ABY POBIERA£A POJEDYNCZY ODCZYT Z POJEDYNCZEGO WEJSCIA. (WYLACZYC AUTOINKREMENT W BAJCIE CONTROLBYTE);
	while (!(TWCR & (1<<TWINT)));
	//_delay_ms(10);
	
	TWDR = deviceAdr;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	_delay_us(70);
	
	TWDR = PCF8591_CONTROL_BYTE;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	_delay_us(70);
	
	//Load the i/o expander to TWDR
	TWCR = (1<<TWSTO) | (1<<TWSTA) | (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	_delay_us(70);
	
	TWDR = deviceAdr + 1;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	_delay_us(70);
	
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	*dataPacket = TWDR;
	
	//_delay_ms(10);
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	
	return 0;
}