#ifndef _SDCARD_H
#define _SDCARD_H

uint8_t SDcard_init();

uint8_t trans_CMD0(uint8_t *retun_value);

uint8_t trans_CMD8(uint8_t *retun_value);

uint8_t trans_CMD58(uint8_t *retun_value);

uint8_t trans_ACMD41(uint8_t *retun_value);

uint8_t read_block(uint32_t block_number, uint8_t *block_info);

uint8_t read_block_ISR(uint32_t block_number, uint8_t *block_info);

uint8_t send_command(uint8_t cmd, uint32_t argum);

uint8_t send_command_ISR(uint8_t cmd, uint32_t argum);

uint8_t get_response(uint8_t num_bytes, uint8_t *array_out);

uint8_t get_response_no_end(uint16_t num_bytes, uint8_t *array_out);

uint8_t get_response_no_end_ISR(uint16_t num_bytes, uint8_t *array_out);

uint8_t read_sector(uint32_t block_number, uint8_t *block_info);

uint8_t read_sector_ISR(uint32_t block_number, uint8_t *block_info);


#endif