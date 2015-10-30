#include "main.h"
#include "port.h"
#include "read_sector.h"


uint8_t read_sector(uint32_t sector_number, uint16_t sector_size, uint8_t *array_for_data)
{
	ncs=0;
	Send_command(17, Sector_number);
	Read_block()bytespersector, array_name);
	ncs=1;
}



uint8_t mount_drive(void)
{
	uint8_t sector[512];
	uint8_t error_flag;
	uint32_t bpb_sector;
	
	error_flag = NO_ERRORS;
	
	read_sector(0, 512, sector);
	if(!(read8(0, sector) == 0xEB || read8(0,sector) == 0xE9))
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
		BPB_SecPerClus `=  read8(0x000D, sector);
		BPB_RsvdSecCnt	= read16(0x000E, sector);
		BPB_NumFATs			=  read8(0x0010, sector);
		BPB_RootEntCnt	= read16(0x0011, sector);
		BPB_TotSec16		= read16(0x0013, sector);
		BPB_FATSz16			= read16(0x0016, sector);
		BPB_HiddenSec		= read32(0x001C, sector);
		BPB_TotSec32		= read32(0x0020, sector);
		BPB_Root_Clus		= read32(0x002C, sector);
		
		
		
		totSec = BPB_TotSec6
		BPB_SecPerClus = read8(0x000D, sector)
}