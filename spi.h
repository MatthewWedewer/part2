#ifndef _SPI_H
#define _SPI_H

#define CMD0 0x40
#define CMD8 0x48


// Sets SPCON based on SCLR_FREQ
uint8_t SPI_master_init(uint32_t SCLR_FREQ);

uint8_t SPI_transfer(uint8_t data_input, uint8_t *data_output);


#endif