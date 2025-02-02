#ifndef SERIAL_H
#define SERIAL_H

#include <avr/io.h>

#define F_CPU       16000000UL
#define BAUD        9600UL
#define BAUD_TOL    2

#include <util/delay.h>
#include <util/setbaud.h>

void USART0_Init(void);

void USART0_Transmit(unsigned char data);

void USART0_PutString(char *ptr);

unsigned char USART0_Receive(void);

char USART0_GetChar(void);

#endif