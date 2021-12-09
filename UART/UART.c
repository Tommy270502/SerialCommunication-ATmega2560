/*********************************************************************************\
*
* Filename: UART.c
*
* Projekt  : UART driver
* Hardware : Mocca-Board, ATmega2560v from Atmel
*
* Description:
* Driver code for the UART port.
*
* Date:       Author:           Version
* 09.12.2021  Thomas S. Perri   V1.0
*
\*********************************************************************************/
#include <avr/io.h>
#include "UART.h"
#define F_CPU 16000000UL
#include "AdvancedDriverMocca.h"

#define FOSC 16000000UL// Clock Speed
#define BAUD 9600
#define MYUBRR0 (FOSC/16/BAUD-1)

uint8_t counter = 0;


void initUSART(uint16_t baud)
{
    //Für Register bits datenblatt s218 - s222
    
    /* Set baud rate */
    UBRR0H = (unsigned char)(baud>>8); 
    UBRR0L = (unsigned char)baud;       
    /* Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set frame format: 8data, 2stop bit, 0parity bit */
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
} // USART_Init

void USART_Transmit(uint16_t data)
{
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) )
    ;
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

uint8_t USART_Receive( void )
{
    /* Wait for data to be received */
    while ( !(UCSR0A & (1<<RXC0)) )
    ;
    /* Get and return received data from buffer */
    return UDR0;
}

//Möglichgkeit 1
void USART_Transmit_Txt1(uint8_t*string)
{
    for (; *string; string++)
    {
        USART_Transmit(*string);
    } 
}

//Möglichkeit 2
void USART_Transmit_Txt2(uint8_t*Txt)
{
    while(*Txt != 0)
    {
        USART_Transmit(*Txt);
        Txt++;
    }
}