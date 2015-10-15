#include "main.h"
#include "port.h"
#include "hardware_delay.h"


//Delays code for X amount of time in ms
//Uses Timer 0 and sets timer 0 to mode 1 
//Time delay ranges from 1 to 65536 by intergers with some error
void delay_ms(const uint16_t timeout_val_ms)
{
	uint16_t num_ms;
	TMOD &= 0xF0; //clear mode in Tmod
	TMOD |= 0x01; //set T0 to mode 1
	ET0 = 0;
	for(num_ms = 0; num_ms < timeout_val_ms; num_ms++)
	{
		TH0 = PRELOAD_1MS_TH; // Loads Timer 0 with values to delay for 1 ms
		TL0 = PRELOAD_1MS_TL;
		TF0 = 0; // Clear overflow flag
		TR0 = 1; // Starts timer
		while(TF0 == 0); // Waits for overflow flag
		TR0 = 0; // Stops Timer 0
	}
}


void delay(const uint16_t timeout_val_ms)
{
	uint16_t num_ms;
	TMOD &= 0xF0; //clear mode in Tmod
	TMOD |= 0x01; //set T0 to mode 1
	ET0 = 0;
	for(num_ms = 0; num_ms < timeout_val_ms; num_ms++)
	{
		TH0 = PRELOAD_1MS_TH; // Loads Timer 0 with values to delay for 1 ms
		TL0 = PRELOAD_1MS_TL;
		TF0 = 0; // Clear overflow flag
		TR0 = 1; // Starts timer
		while(TF0 == 0); // Waits for overflow flag
		TR0 = 0; // Stops Timer 0
	}
}
