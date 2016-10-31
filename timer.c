/******************************************************************
 * Basic Timer functionality for the MSP430FR133 LaunchPad
 * Created on: Oct 30, 2016
 * Copyright 2016 Jason Howser
 ******************************************************************/
#include <msp430.h>
#include "timer.h"

volatile uint32_t timeNow = 0;

void init_timer(uint16_t count) {
	/* Disable timer */
	TA0CTL = TACLR;

	/* Start at 0 */
	TA0R = 0;

	/* Set count to value */
	TA0CCR0 = count;

	TA0CCTL0 = SCS | CCIE | CCIS0;

	/* Enable count up mode and use SMCLK */
	TA0CTL |= TASSEL__SMCLK | MC__UP;
	/* Set Divide by 1 */
	TA0EX0 = TAIDEX_0;

	/* Enable interrupt */
	TA0CTL |= TAIE;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) TIMER0_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
	if (TA0CCTL0 & CCIFG) {
		timeNow++;
	}
	TA0CCTL0 &= ~CCIFG;
	TA0CTL &= ~TAIFG;
}
