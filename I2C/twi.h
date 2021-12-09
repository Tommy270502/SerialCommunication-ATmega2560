/*********************************************************************************\
*
* Filename: twi.h
*
* Projekt  : TWI - Zwo Wire Interface (I2C) driver
* Hardware : Mocca-Board, ATmega2560v from Atmel
*
* Description:
* Driver code for the two wire interface on the ATmega2560.
*
* Date:       Author:           Version
* 09.12.2021  Thomas S. Perri   V1.0
*
\*********************************************************************************/

#ifndef TWI_H_
#define TWI_H_

void twi_init(void);

void twi_write8Bit(uint8_t Adr, uint8_t Reg ,uint8_t Data);
void twi_write16Bit(uint8_t Adr, uint8_t Reg ,uint16_t Data);
void twi_writeData(uint8_t Adr, uint16_t *ArrayData, uint16_t Size);

uint8_t twi_read8Bit(uint8_t Adr, uint8_t Reg);
uint16_t twi_read16Bit(uint8_t Adr, uint8_t Reg);
uint32_t twi_readData(uint8_t Adr, uint8_t *ArrayData, uint32_t Size);
//****************************************************


#endif /* TWI_H_ */