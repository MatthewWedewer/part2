#ifndef _Directory_Functions_H
#define _Directory_Functions_H

#include "Main.h"

//------- Public Constants  -----------------------------------------
#define FAT32 (4)
#define FAT16 (2)
#define FAT12 (12)
#define FAT32_shift (2)
#define FAT16_shift (1)
#define Disk_Error (0xF0)
#define No_Disk_Error (0)
#define more_entries (0x8000)   
#define no_entry_found (0x80000000)  // msb set to indicate error
#define directory_bit  (0x10000000)  // lsb of first nibble (bit28)

 

// ------ Public function prototypes -------------------------------
uint8_t read8(uint16_t offset_address, uint8_t * array_name);

uint16_t read16(uint16_t offset_address, uint8_t * array_name);

uint32_t read32(uint16_t offset_address, uint8_t * array_name);

uint8_t mount_drive(void);

uint32_t first_sector (uint32_t Cluster_num);

uint16_t Print_Directory(uint32_t Sector_num, uint8_t xdata * array_in);

uint32_t Read_Dir_Entry(uint32_t Sector_num, uint16_t Entry, uint8_t xdata * array_in);

uint32_t find_next_clus(uint32_t Cluster_num, uint8_t xdata * array_name);

uint8_t Open_File(uint32_t Cluster, uint8_t xdata * array_in);

#endif