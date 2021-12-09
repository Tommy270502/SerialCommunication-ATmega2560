/*********************************************************************************\
*
* Filename: SPI.c
*
* Projekt  : SPI driver
* Hardware : Mocca-Board, ATmega2560v from Atmel
*
* Description:
* Driver code for the serial port interface.
*
* Date:       Author:           Version
* 09.12.2021  Thomas S. Perri   V1.0
*
\*********************************************************************************/

#include <avr/io.h>
#include "SPI.h"

#define SS 0
#define SCK 1
#define MOSI 2
#define MISO 3


void initSPI(void)
{
    //Set MOSI,SCK, and SS output, all others input
    DDRB = (1<<MOSI)|(1<<SCK)|(1<<SS);
    //Enable SPI, Master, Set clock rate fck/8
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
    SPSR = (1<<SPI2X);
}

uint8_t SPI_transrecive(uint8_t data)
{
    //Start Transmission
    SPDR = data;
    //Wait for transmisson complete
    while(!(SPSR & (1<<SPIF)))
    ;
    //return received data
    return SPDR;
}