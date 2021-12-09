/*********************************************************************************\
*
* Filename: UART.h
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


#ifndef UART_H_
#define UART_H_

#include <stdint.h>
void initUSART();
void USART_Transmit(uint16_t data);
uint8_t USART_Receive(void);
void USART_Transmit_Txt1(uint8_t*string);
void USART_Transmit_Txt2(uint8_t*Txt)

#endif /* UART_H_ */