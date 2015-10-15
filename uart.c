#include "main.h"
#include <stdio.h>
#include "uart.h"

UART_Init()
{
  #define FPER = OSC_FREQ * 6 / OSC_PER_INST
	#define SMOD1 0
	#define SPD 1
	#define BRL_VAL ( (uint8_t) (256-((1+(5*SPD))*(1+(1*SMOD1))*OSC_FREQ)/(32*OSC_PER_INST* (uint32_t)BAUD_RATE)))
	#define TH1 ((uint8_t) 256-((((1+SMOD1)*(OSC_FREQ))/(OSC_PER_INST*32*(uint32_t)BAUD_RATE)))
	BRL = BRL_VAL;
	PCON = PCON | (SMOD1<<7);
	PCON = PCON & (~((~SMOD1)<<7)|0x40);  // The 0x40 is setting the SMOD0 to 0
	SCON = 0x50; // UART MOD1, REN = 1
	BDRCON = 0; // Baud rate controller (Baud genorator must be disabled before setting BRL)
	BRL = BRL_VAL;
	BDRCON = (0x1C|(SPD<<1)); // Enables BRG, selects for RxD and TxD
	TI = 1; // Endables transmitting
	ES = 0; // No serial interrupt
return 0;
}

//UART_PUTCHAR()
//{
//return 0;}
//UART_GETCHAR()
//{
//return 0;}
//	

