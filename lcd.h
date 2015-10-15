#ifndef _LCD_H
#define _LCD_H

#define COMMAND 0
#define DDRAM   1
#define LINE1	0x80
#define LINE2	0xC0

void LCD_Init();
void LCD_Write(bit RegSelect, uint8_t LCD_Data);

void LCD_Print(uint8_t num_bytes, uint8_t *array_name);

void LCD_Clear();






#endif


