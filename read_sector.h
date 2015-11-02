#ifndef _read_sector_H
#define _read_sector_H

#define FAT12 12
#define FAT16 16
#define FAT32 32

uint8_t read_sector(uint32_t sector_number, uint16_t sector_size, uint8_t *array_for_data);


uint32_t read32(uint16_t offset_address, uint8_t *array_name);
uint16_t read16(uint16_t offset_address, uint8_t *array_name);
uint8_t read8(uint16_t offset_address, uint8_t *array_name);

uint8_t mount_drive();






#endif