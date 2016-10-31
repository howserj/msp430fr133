/******************************************************************
 * Basic Project to SPI Flash on a MSP430FR133 LaunchPad
 * Created on: Oct 30, 2016
 * Copyright 2016 Jason Howser
 ******************************************************************/
#include <msp430.h>
#include "stdint.h"
#include "N25Q.h"
#include "uart.h"
#include "spi.h"
#include "timer.h"

extern volatile uint32_t timeNow;

int main(void)
{
	const uint32_t delay = 1000;
	volatile uint32_t count = 0;
	const uint8_t size = 20;
	uint8_t data[20];

    WDTCTL = WDTPW | WDTHOLD;                 // Disable watchdog

    PM5CTL0 &= ~LOCKLPM5;                     // Enable I/O, get out of Power Management
    init_uart();
    init_spi();
    init_timer(TICKS_PER_MS);

	printString("Reading Micron Flash Memory:\r\n");

	FLASH_DEVICE_OBJECT *flash;
	ReturnType result;

	result = Driver_Init(flash);
	NMX_uint32 flashData[64];

    //Enter LPM0
 //   __bis_SR_register(GIE);


    while (1)
    {
//    	data[0] = 0x9E;
//    	spiTxRx(data, size);

    	printTime(timeNow);
    	result = FlashReadDeviceIdentification(flashData);

    	printString(" Data: ");
    	printByteArray((uint8_t*)flashData, 4);
    	printString("\r\n");
    	for(count = delay; count > 0; count--);

        __no_operation();                     // Allow for debug
    }
}


