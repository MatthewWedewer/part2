#include "main.h"
#include "port.h"
#include "lcd.h"
#include "hardware_delay.h"

void LCD_Write(bit RegSelect, uint8_t LCD_Data)
{
	//LCD_DATA_OUT = LCD_Data; // ? LCD_MODULE_DATA_PORT
	RS = RegSelect;
	E = 1;
	delay_ms(10);
	LCD_DATA_OUT = LCD_Data; // ? LCD_MODULE_DATA_PORT
	E = 0;
	delay_ms(1);
	LCD_DATA_OUT = 0xFF; // Make sure if other things attach they wont short (not needed)
}

void LCD_Print(uint8_t num_bytes, uint8_t *array_name)
{	
	uint8_t index;
	if(num_bytes == 0)
	{
		index = 0;
		while(*(array_name + index) != NULL)
		{
			LCD_Write(DDRAM, *(array_name + index));
			index++;
		}	
	}
	else
	{
		for(index = 0; index < num_bytes; index++)
		{
			LCD_Write(DDRAM, *(array_name + index));
		}
	}

}

void LCD_Init()
{
	#define FUNCTION_SET 	0x38
	#define DISPLAY_CONTROL	0x0F
	#define DISPLAY_CLEAR	0x01
	#define ENTRY_MODE		0x06

	delay_ms(30);
	LCD_Write(COMMAND, FUNCTION_SET);
	delay_ms(5);
	LCD_Write(COMMAND, FUNCTION_SET);
//	delay_ms(1);
	LCD_Write(COMMAND, FUNCTION_SET);
//	delay_ms(1);
	LCD_Write(COMMAND, DISPLAY_CONTROL);
//	delay_ms(1);
	LCD_Write(COMMAND, DISPLAY_CLEAR);
	delay_ms(2);
	LCD_Write(COMMAND, ENTRY_MODE);
//	delay_ms(1);
}



void LCD_Clear()
{
	LCD_Write(0,0x01);
	delay_ms(2);
}

