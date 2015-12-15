#ifndef _MAIN_H
#define _MAIN_H



#include "AT89C51RC2.h" // DEFINES SFR'S OF PROCESSOR

#define OSC_FREQ 				((uint32_t) 18432000) 	// <- UL?
#define OSC_PER_INST  	(6) 					//CLOCK DIVISION
#define BAUD_RATE 			9600
#define NUL 						0



#define T2_100US 150 // 11 ms

#define TIMER2HL (65536- (uint16_t) (OSC_FREQ*T2_100US)/(OSC_PER_INST*10000)) // Value needed to run timer for 11 ms for timer 2

//#define TIMER0HL (65536- (uint16_t) ((OSC_FREQ*(T2_100US-10))/(OSC_PER_INST*10000))) // Value needed to run timer for 11 ms for timer 2

#define TIMER2H TIMER2HL / 256
#define TIMER2L TIMER2HL % 256

//#define TIMER0H TIMER0HL / 256
//#define TIMER0L TIMER0HL % 256

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;


#define ACTIVE						0
#define INACTIVE					1

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
#define WRONG_FAT_TYPE		11
#define OFFSET_ERROR			12
#define PRINT_ERROR				13
#define SCL_ZERO_TOO_LONG	14
#define BUS_BUSY_ERROR		15
#define NACK_ERROR				16






#define LOAD_BUFFER_1			1
#define LOAD_BUFFER_2			2
#define FIND_CLUSTER_1		3
#define FIND_CLUSTER_2		4
#define DATA_SEND_1				5
#define DATA_SEND_2				6
#define DATA_IDLE_1				7
#define DATA_IDLE_2				8

#define sw1 (1<<0)
#define sw2 (1<<1)
#define sw3 (1<<2)
#define sw4 (1<<3)


typedef enum { none, not_pressed, debounce, pressed, hold, r_debounce} sw_state_t;

typedef struct
{
	sw_state_t sw_state;
	uint8_t debounce_time;
} sw_globals_t;

#endif