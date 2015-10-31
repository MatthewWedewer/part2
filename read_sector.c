#include "main.h"
#include "port.h"
#include "read_sector.h"
#include "spi.h"
#include "SDcard.h"
#include "stdio.h"


	uint16_t	BPB_BytesPerSec;
	uint8_t   BPB_SecPerClus;
	uint16_t	BPB_RsvdSecCnt;
	uint8_t	  BPB_NumFATs;
	uint16_t	BPB_RootEntCnt;
	uint16_t	BPB_TotSec16;
	uint16_t	BPB_FATSz16;
	uint32_t	BPB_HiddenSec;
	uint32_t	BPB_TotSec32;
	uint32_t	BPB_FATSz32;
	uint32_t	BPB_Root_Clus;




uint8_t read_sector(uint32_t sector_number, uint16_t sector_size, uint8_t *array_name)
{
	
		uint8_t error_flag;
		ncs=0;
		send_command(17, sector_number);
		error_flag = read_block(sector_size, array_name);
		ncs=1;
		return error_flag;
	
}

uint32_t read32(uint16_t offset_address, uint8_t *array_name)
{
	if (offset_address <512)
	{
		uint32_t return_value =0;
		uint8_t temp, index;
				for (index = 0; index < 4; index++)
		{
		temp =*(array_name + offset_address + ( 3 - index));
			return_value= return_value << 8;
			return_value |= temp;
		}
		printf("%lu",return_value);
		return return_value;
	}
	else
		return OFFSET_ERROR;
}

uint16_t read16(uint16_t offset_address, uint8_t *array_name)
{
	if (offset_address <512)
	{
		uint16_t return_value =0;
		uint8_t temp, index;
				for (index = 0; index < 4; index++)
		{
		temp =*(array_name + offset_address + ( 1 - index));
				return_value= return_value << 8;
			return_value |= temp;
		}
		printf("%u",return_value);
		return return_value;
	}else
		return OFFSET_ERROR;
}

uint8_t read8(uint16_t offset_address, uint8_t *array_name)
{
	if (offset_address <512)
	{
		uint8_t return_value =0;
		uint8_t temp, index;
				for (index = 0; index < 4; index++)
		{
		temp =*(array_name + offset_address);
				return_value= return_value << 8;
			return_value |= temp;
		}
		printf("%bu",return_value);
		return return_value;
	}else
		return OFFSET_ERROR;
}

uint8_t mount_drive(void)
{
	uint8_t sector[512];
	uint8_t error_flag;
	uint32_t bpb_sector;
	
	error_flag = NO_ERRORS;
	
	read_sector(0, 512, sector);
	if(!(read8(0, sector) == 0xEB || read8(0,array) == 0xE9))
	{
		bpb_sector = read32(0x01C6, sector);
	}
	else
	{
		error_flag = WRONG_RETURN;
	}
	if(error_flag == NO_ERRORS)
	{
		read_sector(bpb_sector, 512, sector);
		
		BPB_BytesPerSec = read16(0x000B, sector);
		BPB_SecPerClus  =  read8(0x000D, sector);
		BPB_RsvdSecCnt	= read16(0x000E, sector);
		BPB_NumFATs			=  read8(0x0010, sector);
		BPB_RootEntCnt	= read16(0x0011, sector);
		BPB_TotSec16		= read16(0x0013, sector);
		BPB_FATSz16			= read16(0x0016, sector);
		BPB_HiddenSec		= read32(0x001C, sector);
		BPB_TotSec32		= read32(0x0020, sector);
		BPB_FATSz32			= read32(0x0024, sector);
		BPB_Root_Clus		= read32(0x002C, sector);
		
		
		FATSz = BPB_FATSz32;
		totSec = BPB_TotSec32;
		dataSec = totSec - (BPB_RsvdSecCnt + (BPB_NumFATS * FATSz) + Root);
		//dfsdf
		BPB_SecPerClus = read8(0x000D, sector);
	}///I put this here to get rid of the error Im not sure if this is the right spot for it or not./////////
}