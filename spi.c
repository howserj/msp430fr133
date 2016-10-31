/******************************************************************
 * Basic SPI functionality for the MSP430FR133 LaunchPad
 * Created on: Oct 30, 2016
 * Copyright 2016 Jason Howser
 ******************************************************************/
#include "spi.h"
#include <stdlib.h>


void init_spi(void) {
	// Configure Chip Select
    P1DIR |= BIT4;
    P1OUT |= BIT4;

    P5OUT &= ~(BIT1 | BIT2 | BIT3);

    // Configure SPI pins
    P5SEL0 |= BIT1 | BIT2 | BIT3;             // Set 3 SPI pin as second function

    // Configure SPI speed
    UCB0CTLW0 |= UCSWRST;                     // Put SPI in reset
    // Set 1MHZ clock and master mode
    UCB0CTLW0 |= UCSSEL__SMCLK | UCMST | UCMSB | UCSYNC | UCCKPH;

    UCB0BRW = 0;                              // Full speed, no divide

    UCB0CTLW0 &= ~UCSWRST;                    // Remove reset
    UCB0IE = UCRXIE | UCTXIE;                 // Enable USCI_B0 RX/TX interrupt
}


SPI_STATUS Serialize_SPI(const CharStream* char_stream_send,
                         CharStream* char_stream_recv,
                         SpiConfigOptions optBefore, SpiConfigOptions optAfter) {
	uint16_t i = 0;

	if (optBefore == OpsWakeUp) {
		/* Set Chip Select */
		P1OUT &= ~BIT4;
	} else if (optBefore == OpsEndTransfer) {
		/* Clear Chip Select */
		P1OUT |= BIT4;
	}

	if(char_stream_send->length > 0) {
		UCB0TXBUF = char_stream_send->pChar[0];
		while(!(UCB0IFG & UCTXIFG));
		UCB0IFG &= ~UCTXIFG;
		for(i = 0; i < char_stream_send->length || i < char_stream_recv->length; i++) {
			if (i < char_stream_send->length) {
				UCB0TXBUF = char_stream_send->pChar[i];
				while(!(UCB0IFG & UCTXIFG));
				UCB0IFG &= ~UCTXIFG;
			} else {
				UCB0TXBUF = 0x00;
				while(!(UCB0IFG & UCTXIFG));
				UCB0IFG &= ~UCTXIFG;
			}

			if (char_stream_recv != NULL && i < char_stream_recv->length) {
				while(!(UCB0IFG & UCRXIFG));
				char_stream_recv->pChar[i] = UCB0RXBUF;
				UCB0IFG &= ~UCRXIFG;
			}
		}
	}

	if (optAfter == OpsWakeUp) {
		/* Set Chip Select */
		P1OUT &= ~BIT4;
	} else if (optAfter == OpsEndTransfer) {
		/* Clear Chip Select */
		P1OUT |= BIT4;
	}
	return RetSpiSuccess;
}

void spiTxRx(uint8_t data[], uint16_t size) {
	uint16_t i = 0;

	if(size > 0) {
		/* Set Chip Select */
		P1OUT &= ~BIT4;
		UCB0TXBUF = data[0];
		while(!(UCB0IFG & UCTXIFG));
		UCB0IFG &= ~UCTXIFG;
		for(i = 0; i < size; i++) {
			UCB0TXBUF = data[i];
			while(!(UCB0IFG & UCTXIFG));
			UCB0IFG &= ~UCTXIFG;

			while(!(UCB0IFG & UCRXIFG));
			data[i] = UCB0RXBUF;
			UCB0IFG &= ~UCRXIFG;
		}
		/* Clear Chip Select */
		P1OUT |= BIT4;
	}
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(UCB0IV,USCI_SPI_UCTXIFG))
    {
        case USCI_NONE: break;
        case USCI_SPI_UCTXIFG: break;
        case USCI_SPI_UCRXIFG: break;
    }
}
