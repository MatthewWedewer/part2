#ifndef _READ_SECTOR_H
#define _READ_SECTOR_H



uint8_t read_sector(uint32_t sector_number, uint16_t sector_size, uint8_t *array_for_data);

uint32_t read_value_32(uint16_t offset_address, uint8_t *array_name);
uint16_t read_value_16(uint16_t offset_address, uint8_t *array_name);
uint8_t read_value_8(uint16_t offset_address, uint8_t *array_name);



#endif