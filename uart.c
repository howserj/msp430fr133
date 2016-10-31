/******************************************************************
 * Basic UART functionality for the MSP430FR133 LaunchPad
 * Created on: Oct 30, 2016
 * Copyright 2016 Jason Howser
 ******************************************************************/
#include "uart.h"
#include "timer.h"

void init_uart(void) {
//    P4DIR |= BIT0;
//    P4OUT &= ~BIT0;

    // Configure UART pins
    P1SEL0 |= BIT0 | BIT1;                    // enable secondary pin function (UART)

    // Configure UART
    UCA0CTLW0 |= UCSWRST;                     // Put UART in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;

    // Set baud
    UCA0BR0 = 8;                              // 115200 speed settings from SLAU445G
    UCA0MCTLW = 0xD600;
    UCA0BR1 = 0;

    UCA0CTLW0 &= ~UCSWRST;                    // Remove Reset
    UCA0IE |= UCTXCPTIE;                      // Enable USCI_A0 TX interrupt
}

uint16_t printString(char *text) {
	uint16_t numChars = 0;
	while(*text != '\0') {
		uartSendByte(*text);
		text++;
		numChars++;
	}
	return numChars;
}

void uartSendByte(char text) {
	UCA0TXBUF = text;
	while(!(UCA0IFG & UCTXCPTIFG));
	UCA0IFG &= ~UCTXCPTIFG;
}

void printByteArray(uint8_t data[], uint16_t size) {
	uint16_t i = 0;
	char hex[3];
	for(i = size; i > 0; i--) {
		byteToHexString(data[size - i], hex);
		printString(hex);
	}
}

void byteToHexString(uint8_t num, char *string) {
	string[0] = numToHexChar(num / 16);
	string[1] = numToHexChar(num % 16);
	string[2] = '\0';
}

void toDecString(uint8_t num, char *string) {
	string[0] = numToHexChar(num / 10);
	string[1] = numToHexChar(num % 10);
	string[2] = '\0';
}

char numToHexChar(uint8_t num) {
	if(num < 10) {
		return '0' + num;
	} else {
		return 'A' + (num - 10);
	}
}

void printTime(uint32_t millis) {
	char dec[3];
	/* Remove Day Wraparound */
	uint32_t current_time = millis % (MILLIS_PER_SECOND * SECONDS_PER_MINUTE * MINUTES_PER_HOURS * HOURS_PER_DAY);

	/* Pring hours since midnight */
	toDecString((uint8_t)(current_time / (MILLIS_PER_SECOND * SECONDS_PER_MINUTE * MINUTES_PER_HOURS)), dec);
	printString(dec);
	printString(":");

	/* Calculate minutes since last hour */
	current_time %= (MILLIS_PER_SECOND * SECONDS_PER_MINUTE * MINUTES_PER_HOURS);
	/* Print minutes since last hour */
	toDecString((uint8_t)(current_time / (MILLIS_PER_SECOND * SECONDS_PER_MINUTE)), dec);
	printString(dec);
	printString(":");

	/* Calculate seconds since last hour */
	current_time %= (MILLIS_PER_SECOND * SECONDS_PER_MINUTE);
	/* Print seconds since last hour */
	toDecString((uint8_t)(current_time / MILLIS_PER_SECOND), dec);
	printString(dec);
	printString(".");

	/* Calculate milliseconds since last hour */
	current_time %= (MILLIS_PER_SECOND);
	/* Print milliseconds since last hour */
	toDecString((uint8_t)(current_time / 10), dec);
	printString(dec);
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(UCA0IV,USCI_UART_UCTXCPTIFG))
    {
        case USCI_NONE: break;
/*        case USCI_UART_UCRXIFG:
              UCA0IFG &=~ UCRXIFG;            // Clear interrupt
              RXData = UCA0RXBUF;             // Clear buffer
              break;*/
        case USCI_UART_UCTXIFG: break;
        case USCI_UART_UCSTTIFG: break;
        case USCI_UART_UCTXCPTIFG: break;
    }
}
