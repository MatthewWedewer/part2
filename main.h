#ifndef _MAIN_H
#define _MAIN_H


//Did this get over to you Colton?
#include "AT89C51RC2.h" // DEFINES SFR'S OF PROCESSOR

#define OSC_FREQ 				((uint32_t) 18432000) 	// <- UL?
#define OSC_PER_INST  	(12) 					//CLOCK DIVISION
#define BAUD_RATE 			9600
#define NUL 						0


typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;

#define NO_ERRORS					0
#define TIMEOUT_ERROR 		1
#define SPI_ERROR 				2
#define ILLEGAL_COMMAND 	3
#define ILLEGAL_CLOCKRATE 4
#define WRONG_RESPONSE		5
#define FAIL_SDINIT				6
#define WRONG_SDCARD			7
#define SDCARD_ERROR			8
#define VERSION_1_SD			9
#define WRONG_RETURN			10
#define OFFSET_ERROR			11

#endif