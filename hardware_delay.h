#ifndef _DELAY_H
#define _DELAY_H


#define PRELOADTHTL (65536- (uint16_t) (OSC_FREQ/(OSC_PER_INST*1020))) // Value needed to run timer for 1 ms for timer 0
#define PRELOAD_1MS_TH PRELOADTHTL/256 // TH value needed for Timer 0 for 1 ms delay
#define PRELOAD_1MS_TL PRELOADTHTL%256 // TL value needed for Timer 0 for 1 ms delay




//Delays code for X amount of time in ms
//Uses Timer 0 and sets timer 0 to mode 1 
void delay_ms(const uint16_t timeout_valms);


//Delays code for X amount of time in ms
//Uses Timer 0 and sets timer 0 to mode 1 
void delay(const uint16_t timeout_valms);


#endif


