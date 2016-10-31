/******************************************************************
 * Basic Timer functionality for the MSP430FR133 LaunchPad
 * Created on: Oct 30, 2016
 * Copyright 2016 Jason Howser
 ******************************************************************/
#ifndef TIMER_H_
#define TIMER_H_
#include "stdint.h"

#define TICKS_PER_SECOND (1000000L)
#define TICKS_PER_MS (TICKS_PER_SECOND / 1000)

#define MILLIS_PER_SECOND (1000)
#define SECONDS_PER_MINUTE (60)
#define MINUTES_PER_HOURS (60)
#define HOURS_PER_DAY (24)

void init_timer(uint16_t count);


#endif /* TIMER_H_ */
