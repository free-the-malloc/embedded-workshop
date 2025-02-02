#include <stdio.h>

#include <serial.h>


/* https://developerhelp.microchip.com/xwiki/bin/view/products/mcu-mpu/8-bit-avr/peripherals/usart/8-bit-avr-usart-mega-configuration/#HCodeExample */

void USART0_Init(void)
{

    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    #if USE_2X                       
    UCSR0A |= (1 << U2X0);
    #else
    UCSR0A &= (1 << U2X0);
    #endif


    UCSR0C = 0x06;

    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

}

/* Transmit a single byte of data */
void USART0_Transmit(unsigned char data)
{
    while(!(UCSR0A & (1 << UDRE0)));

    UDR0 = data;
}

/* Write a string to port */
void USART0_PutString(char *ptr)
{
    while(*ptr){   // Loop until end of string (*s = '\0')
        USART0_Transmit(*ptr++); // Send the character and point to the next one
    }
}

unsigned char USART0_Receive(void)
{
    while(!(UCSR0A & (1 << RXC0)));

    return UDR0;
}

char USART0_GetChar(void)
{
 
    char rxdata;

    if (UCSR0A & (1<<RXC0)){  // checking if USART RX data is available
        rxdata = UDR0;   // reading the received byte (clears RXC0)
        return rxdata;   // return the data
    }

    return 0x00; /* if nothing read, return a null byte */
}
