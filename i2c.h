#ifndef _I2C_H
#define _I2C_H


#define cont 	4
#define stp		7


#define I2C_RELOADH 00 //Need to still calculate
#define I2C_RELOADL 00 //Need to still calculate


uint8_t I2C_Write(uint8_t device_addr, uint8_t num_bytes, uint8_t *array_name);

uint8_t I2C_Read(uint8_t device_addr, uint8_t num_bytes, uint8_t *array_name);

void I2C_Clock_Delay(uint8_t control);

void I2C_Clock_Start();








#endif