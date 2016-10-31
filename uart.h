/******************************************************************
 * Basic UART functionality for the MSP430FR133 LaunchPad
 * Created on: Oct 30, 2016
 * Copyright 2016 Jason Howser
 ******************************************************************/
#ifndef UART_H_
#define UART_H_
#include <stdint.h>
#include <msp430.h>


uint16_t printString(char *text);
void uartSendByte(char text);
void init_uart(void);
void printByteArray(uint8_t data[], uint16_t size);
void byteToString(uint8_t num, char *string);
void toDecString(uint8_t num, char *string);
char numToHexChar(uint8_t num);
void printTime(uint32_t millis);

#endif /* UART_H_ */
