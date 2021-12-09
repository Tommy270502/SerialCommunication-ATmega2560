/*********************************************************************************\
*
* Filename: twi.c
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
#include <avr/io.h>
#define F_CPU 16000000UL
#include "AdvancedDriverMocca.h"
#include "twi.h"
#include <util/twi.h>

#define ERROR_START         0b10000000
#define ERROR_STOP          0b00000001
#define ERROR_REP_START     0b01000000
#define ERROR_ADR_TRANSMIT  0b00100000
#define ERROR_DATA_TRANSMIT 0b00010000

void twi_init(void)
{
    //TWPS1 TWPS0 Prescaler Value
    //  0     0          1
    //  0     1          4 
    //  1     0         16
    //  1     1         64
    // Siehe formel datenblatt 2560 seite 242
    TWSR = 1;
    TWBR = 0b00010010;//Dez 18 
}

void twi_start(void)
{
    //Start senden
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    //warten bis Start geschickt wurde
    while (!(TWCR & (1<<TWINT)));
    //Wenn TWSR anderst als START ist ist ein Fehler passiert.
    if ((TWSR & 0xF8) != TW_START)
    {
        twi_error(TWSR); 
    }
}

void twi_repStart(void)
{
    //repStart senden
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    //warten bis repStart geschickt wurde
    while (!(TWCR & (1<<TWINT)));
    //Wenn TWSR anderst als repSTART ist ist ein Fehler passiert.
     if ((TWSR & 0xF8) != TW_REP_START)
     {
         twi_error(TWSR);
     }
}

void twi_stop(void)
{
    //stop senden
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void twi_transmitAdr(uint8_t Adr)
{
    //Adresse im Register schreiben
    TWDR = Adr;
    //clear TWINT and start transmission
    TWCR = (1<<TWINT)|(1<<TWEN);
    //Wait for adress transmitted
    while (!(TWCR & (1<<TWINT)));
    //if ACK not received then error occurred
     if ((TWSR & 0xF8) != TW_MT_SLA_ACK)
     {
         twi_error(TWSR);
     }
}

void twi_receiveAdr(uint8_t Adr)
{
    
}

void twi_sendDataNACK(uint8_t TWData)
{
   //Daten im Register Schreiben
   TWDR = TWData;
   //clear TWINT and start transmission
   TWCR = (1<<TWINT)|(1<<TWEN);
   //Wait for data transmitted
   while (!(TWCR & (1<<TWINT)));
   //if ACK not received then error occurred
    if ((TWSR & 0xF8) != TW_MT_DATA_NACK)
    {
        twi_error(TWSR);
    }  
}

void twi_sendDataACK(uint16_t TWData)
{
    //Daten im Register Schreiben
    TWDR = TWData;
    //clear TWINT and start transmission
    TWCR = (1<<TWINT)|(1<<TWEN);
    //Wait for data transmitted
    while (!(TWCR & (1<<TWINT)));
    //if ACK not received then error occurred
     if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
     {
         twi_error(TWSR);
     }
}



uint8_t twi_readDataNACK(void)
{
    uint8_t TWData = 0;
    
    //clear TWINT and start transmission
    TWCR = (1<<TWINT)|(1<<TWEN);
    //Wait for data transmitted
    while (!(TWCR & (1<<TWINT)));
    //Daten im Register Schreiben
    TWData = TWDR;
    //return data
    return TWData;
}

uint8_t twi_readDataACK(void)
{
    uint8_t TWData = 0;
    
    //clear TWINT and start transmission
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA); 
    //Wait for data transmitted
    while (!(TWCR & (1<<TWINT)));
    //Ack ausschalten
    TWCR = (1<<TWINT)|(1<<TWEN);
    //Daten im Register Schreiben
    TWData = TWDR;
    //return data
    return TWData;
}

 void twi_error(uint8_t ErrorNr)
 {
//      write_text(3,0,"ErrorCode: ");
//      write_zahl(3,12,ErrorNr,4,0,0);
 }

//********************************************
//I2C Treiber V2
//Write Treiber
void twi_write8Bit(uint8_t Adr, uint8_t Reg ,uint8_t Data)
{
    twi_start();
    twi_transmitAdr(Adr);
    twi_sendDataACK(Reg);
    twi_sendDataACK(Data);
    twi_stop();
}

void twi_write16Bit(uint8_t Adr, uint8_t Reg ,uint16_t Data)
{
    //MSB zuerst
    twi_start();
    twi_transmitAdr(Adr);
    twi_sendDataACK(Reg);
    twi_sendDataACK(Data>>8);
    twi_sendDataACK(Data);
    twi_stop();
}

void twi_writeData(uint8_t Adr, uint16_t *ArrayData, uint16_t Size)
{
    uint16_t i = 0;
    
    twi_start();
    twi_transmitAdr(Adr);
    for (i = 0; Size >= i; i++)
    {
        twi_sendDataACK(*ArrayData);
        ArrayData++;
    } 
    twi_stop();
}
//**********************************************
//read Treiber
uint8_t twi_read8Bit(uint8_t Adr, uint8_t Reg)
{
    uint8_t data = 0;
  
    twi_start();
    twi_transmitAdr(Adr);
    twi_sendDataACK(Reg);
    twi_start();
    twi_transmitAdr(Adr|0x01);
    data = twi_readDataNACK();
    twi_stop();
    
    return data;
}

uint16_t twi_read16Bit(uint8_t Adr, uint8_t Reg)
{
    uint16_t dataMSB = 0;
    uint16_t dataLSB = 0;
    uint16_t data = 0;
    
    twi_start();
    twi_transmitAdr(Adr);
    twi_sendDataACK(Reg);
    twi_start();
    twi_transmitAdr(Adr|0x01);
    dataMSB = twi_readDataACK();
    dataLSB = twi_readDataNACK();
    twi_stop();
    
    data = (dataMSB<<8)|dataLSB;
    
    return data; 
}

uint32_t twi_readData(uint8_t Adr, uint8_t *ArrayData, uint32_t Size)
{
    uint32_t data;
    uint16_t i = 0;
    
    twi_start();
    twi_transmitAdr(Adr);
    for (i = 0; Size > i; i++)
    {
        *ArrayData = twi_readDataACK();
        ArrayData++;
    }
    *ArrayData = twi_readDataNACK();
    twi_stop();
    
    data = *ArrayData;
    
    return data;
}
