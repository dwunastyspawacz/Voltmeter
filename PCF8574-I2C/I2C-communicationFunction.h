#include <avr/io.h>
#include <twi.h>

#define I2C_ERROR 0

/*  Function takes an address of a device and sends the data to it.
	When an error occurs it returns the signature of the error so it can be easily used
	and sent via USART to PC in order to diagnose the problem.
*/


void I2C_PCF8574(uint8_t deviceAdr, uint8_t dispIndex, uint8_t *errorBuffer)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); 
	while (!(TWCR & (1<<TWINT)));
	
	if ((TWSR & 0xF8) != TW_START)
	{
		*errorBuffer = TW_START; 
	}
	
	TWDR = deviceAdr;	 //Load the i/o expander to TWDR
	
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
	if ((TWSR & 0xF8) !=TW_MT_SLA_ACK)
	{
		*errorBuffer = TW_MT_SLA_ACK;
	}
	
	TWDR = dispIndex;
	
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
	if ((TWSR & 0xF8) !=TW_MT_DATA_ACK)
	{
		*errorBuffer = TW_MT_DATA_ACK;
	}
	
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}





uint8_t I2C_PCF8591(uint8_t deviceAdr, uint8_t PCF8591_controlByte, uint8_t *errorBuffer) //a controlByte configures the pcf8591
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // PRZEBUDOWAC FUNKCJE ABY POBIERA£A POJEDYNCZY ODCZYT Z POJEDYNCZEGO WEJSCIA. (WYLACZYC AUTOINKREMENT W BAJCIE CONTROLBYTE);
	while (!(TWCR & (1<<TWINT)));
	
	/*if ((TWSR & 0xF8) != TW_START)
	{
		return TW_START;
	}*/
	
	TWDR = deviceAdr + TW_WRITE;	 //Load the i/o expander to TWDR
	
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
	/*if ((TWSR & 0xF8) !=TW_MT_SLA_ACK)
	{
		return TW_MT_SLA_ACK;
	}*/
	
	TWDR = PCF8591_controlByte;	    //Load the configuration byte to TWDR
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
	/*if ((TWSR & 0xF8) !=TW_MT_DATA_ACK)
	{
		return TW_MT_DATA_ACK;
	}*/
	
	TWDR = deviceAdr + TW_READ; // send an address of a device and an information (TW_READ) of waiting for data to receive
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
	/*if((TWSR & 0xF8) !=TW_MR_SLA_ACK)
	{
		return TW_MR_SLA_ACK;
	}*/
	
	uint8_t aux = 0;
	uint8_t dataPacket;
	for(aux ; aux<2 ; aux++)
	{
		if(aux==1)   // czwarty bajt danych bedzie ostatnim, wyslanie NOTACK
		{
			dataPacket = TWDR;
			TWCR = (1<<TWINT) | (0<<TWEA);
			while(!(TWCR & (1<<TWINT)));
			break;
		}
		else // pierwsze 3 bajty pobrane, wysylanie odpowiedzi ACK
		{
			dataPacket = TWDR; // obtaining data from a device
			TWCR = (1<<TWINT) | (1<<TWEA);
			while (!(TWCR & (1<<TWINT)));
	
		/*	if((TWSR & 0xF8) !=TW_MR_DATA_ACK)
			{
				return TW_MR_DATA_ACK;
			}*/
	}
	aux = 0;
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	

	return dataPacket;
	
}
	}