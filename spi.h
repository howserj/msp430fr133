/******************************************************************
 * Basic SPI functionality for the MSP430FR133 LaunchPad
 * Created on: Oct 30, 2016
 * Copyright 2016 Jason Howser
 ******************************************************************/
#ifndef SPI_H_
#define SPI_H_
#include <stdint.h>
#include <msp430.h>


typedef enum
{
	RetSpiError,
	RetSpiSuccess
} SPI_STATUS;

// Acceptable values for SPI master side configuration
typedef enum _SpiConfigOptions
{
	OpsNull,  			// do nothing
	OpsWakeUp,			// enable transfer
	OpsInitTransfer,
	OpsEndTransfer,

} SpiConfigOptions;

// char stream definition for
typedef struct _structCharStream
{
	uint8_t* pChar;                                // buffer address that holds the streams
	uint32_t length;                               // length of the stream in bytes
} CharStream;

#define SPI_SR1_WEL				(1 << 1) // Needed by N25Q driver

void init_spi(void);
void spiTxRx(uint8_t data[], uint16_t size);
SPI_STATUS Serialize_SPI(const CharStream* char_stream_send,
                         CharStream* char_stream_recv,
                         SpiConfigOptions optBefore, SpiConfigOptions optAfter);

#endif /* SPI_H_ */
