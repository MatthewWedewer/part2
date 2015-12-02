#ifndef _I2C_H
#define _I2C_H


#define cont 	4
#define stp		7

#define I2C_RELOADHL (65536- (uint16_t) (OSC_FREQ/(OSC_PER_INST*23))) // Value needed to run timer for 1 ms for timer 0
#define I2C_RELOADH I2C_RELOADHL/256 //Need to still calculate 44.1 kHz
#define I2C_RELOADL I2C_RELOADHL%256 //Need to still calculate 44.1 kHz

uint8_t I2C_Write(uint8_t device_addr, uint8_t num_bytes, uint8_t *array_name);

uint8_t I2C_Read(uint8_t device_addr, uint8_t num_bytes, uint8_t *array_name);

void I2C_Clock_Delay(uint8_t control);

void I2C_Clock_Start();








#endif