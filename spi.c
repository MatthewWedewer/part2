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




uint8_t send_command(uint8_t cmd, uint32_t argum)
{
	uint8_t error_flag, send_val, return_val, index;
	if(cmd < 64)
	{
		send_val = 0x40 | cmd;
		error_flag = SPI_transfer(send_val, &return_val);
		index = 24;
		while((error_flag == NO_ERRORS)&(index<25))
		{
			send_val = (uint8_t)(argum >> index);
			error_flag = SPI_transfer(send_val, &return_val);
			index -= 8;
		}
		if(cmd == 0)
			send_val = 0x95;
		else if(cmd == 8)
			send_val = 0x87;
		else
			send_val = 0x01;
		if(error_flag == NO_ERRORS)
			error_flag = SPI_transfer(send_val, &return_val);
		if(error_flag == NO_ERRORS)
			error_flag = SPI_ERROR;
	}
	else
		error_flag = ILLEGAL_COMMAND;
	return error_flag;
}



uint8_t get_response(uint8_t num_bytes, uint8_t *array_out)
{
	uint8_t timeout, error_flag, recieve_value;
	uint8_t index;
	timeout = 0;
	error_flag = NO_ERRORS;
	do
	{
		error_flag = SPI_transfer(0xFF, &recieve_value);
		timeout++;
	}while((timeout!=0)&&(error_flag == NO_ERRORS)&&(recieve_value == 0xFF));
	*array_out = recieve_value;
	if(timeout == 0)
		error_flag = TIMEOUT_ERROR;
	else if(error_flag != NO_ERRORS)
		error_flag = SPI_ERROR;
	else if((recieve_value != 0x01)&&(recieve_value != 0x00))
		error_flag = SPI_ERROR;
	else if(num_bytes > 1)
	{
		for(index = 1; index < num_bytes; index++)
		{
			SPI_transfer(0xFF, &recieve_value);
			array_out[index] = recieve_value;
		}
	}
	if(error_flag == NO_ERRORS)
		error_flag = SPI_transfer(0xFF, &recieve_value);
	return error_flag;
}


uint8_t get_response_no_end(uint16_t num_bytes, uint8_t *array_out)
{
	uint8_t timeout, error_flag, recieve_value;
	uint16_t index;
	timeout = 0;
	error_flag = NO_ERRORS;
	do
	{
		error_flag = SPI_transfer(0xFF, &recieve_value);
		timeout++;
	}while((timeout!=0)&&(error_flag == NO_ERRORS)&&(recieve_value == 0xFF));
	*array_out = recieve_value;
	if(timeout == 0)
		error_flag = TIMEOUT_ERROR;
	else if(error_flag != NO_ERRORS)
		error_flag = SPI_ERROR;
	else if((recieve_value != 0x01)&&(recieve_value != 0x00))
		error_flag = SPI_ERROR;
	else if(num_bytes > 1)
	{
		for(index = 1; index < num_bytes; index++)
		{
			SPI_transfer(0xFF, &recieve_value);
			array_out[index] = recieve_value;
		}
	}
	return error_flag;
}