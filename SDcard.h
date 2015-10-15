#ifndef _SDCARD_H
#define _SDCARD_H

uint8_t SDcard_init();

uint8_t trans_CMD0(uint8_t *retun_value);

uint8_t trans_CMD8(uint8_t *retun_value);

uint8_t trans_CMD58(uint8_t *retun_value);

uint8_t trans_ACMD41(uint8_t *retun_value);

uint8_t read_block(uint32_t block_number, uint8_t *block_info);



#endif