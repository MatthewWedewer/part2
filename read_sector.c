#include "main.h"
#include "port.h"
#include "read_sector.h"
#include "spi.h"
#include "SDcard.h"
#include <stdio.h>
#include "stdio.h"
#include "print_bytes.h"
#include "Directory_Functions.h"


	



uint8_t read_sector(uint32_t block_number, uint16_t sector_size, uint8_t *block_info)
{
	uint8_t error_flag, timeout;
	uint8_t return_value[5];
	ncs = 0;
	timeout = 0;
	//printf("%lu ",block_number);
	error_flag = send_command(17, block_number);
	do
	{
		timeout++;
		if(error_flag == NO_ERRORS)
			error_flag = get_response_no_end(1, &return_value);
	}while(return_value[0] != 0x00 && timeout != 0);


	if(timeout == 0)
	{
		error_flag = TIMEOUT_ERROR;
	}
	timeout = 0;
	do
	{
		timeout++;
		if(error_flag == NO_ERRORS)
			error_flag = get_response_no_end(1, &return_value);
	}while(return_value[0] != 0xFE    && timeout != 0 && error_flag == NO_ERRORS);			    //&& (return_value[0] & 0xF0) != 0x00
	

	
	if(timeout == 0)
	{
		error_flag = TIMEOUT_ERROR;
	}
	if((return_value[0] & 0xF0 )== 0x00)
	{
		error_flag = SDCARD_ERROR;
	}
	if(error_flag == NO_ERRORS)
	{
		error_flag = get_response_no_end(sector_size, block_info);
	}
	if(error_flag == NO_ERRORS)
	{
		error_flag = get_response_no_end(3, &return_value);
//		CRC16 = return_value[0] * 256 + return_value[1]; Check sum, dont care
	}
	ncs = 1;
	//print_memory(return_value, BPB_BytesPerSec);
	return error_flag;
	
}







