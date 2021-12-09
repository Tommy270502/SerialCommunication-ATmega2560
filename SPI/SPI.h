/*********************************************************************************\
*
* Filename: SPI.h
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


#ifndef SPI_H_
#define SPI_H_

//Standardisierte Datentypen
#include <stdint.h>
void initSPI(void);
uint8_t SPI_transrecive(uint8_t data);




#endif /* SPI_H_ */