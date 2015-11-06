#include "main.h"
#include "port.h"
#include "spi.h"


uint8_t SPI_master_init(uint32_t SCLR_FREQ)
{
	uint8_t divider, return_val;
	return_val = NO_ERRORS;
	divider = (uint8_t)(((OSC_FREQ/OSC_PER_INST)*6)/SCLR_FREQ);
	if(divider <=2)
		SPCON = 0x70;
	else if(divider <=4)
		SPCON = 0x71;
	else if(divider <=8)
		SPCON = 0x72;
	else if(divider <=16)
		SPCON = 0x73;
	else if(divider <=32)
		SPCON = 0xF0;	
	else if(divider <=64)
		SPCON = 0xF1;	
	else if(divider <=128)
		SPCON = 0xF2;	
	else // divider > 128
		return_val = ILLEGAL_CLOCKRATE;
	return return_val;
}



uint8_t SPI_transfer(uint8_t data_input, uint8_t *data_output)
{
	uint8_t test, timeout;
	timeout = 0;
	SPDAT = data_input;
	do
	{
		test = SPSTA;
		timeout++;
	}while(((test & 0x80) != 0x80)&&(timeout!=0));
	if(timeout != 0)
	{
		*data_output = SPDAT;
		timeout = (test & 0x70);
	}
	else
	{
		timeout = TIMEOUT_ERROR;
	}
	return timeout;
}




