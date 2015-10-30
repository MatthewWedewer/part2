#include "main.h"
#include "port.h"
#include "read_sector.h"


uint8_t Read_Sector(uint32_t sector_number, uint16_t sector_size, uint8_t *array_for_data)
{
	ncs=0;
	Send_command(17, Sector_number);
	Read_block()bytespersector, array_name);
	ncs=1;
}