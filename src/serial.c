#define F_CPU   16000000UL             
#define BAUD    9600UL                 
#define BAUD_TOL 2                   

#include <stdio.h>

#include <avr/io.h>
#include <util/setbaud.h>
#include <util/delay.h>


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

void USART0_Transmit(unsigned char data)
{
    while(!(UCSR0A & (1 << UDRE0)));

    UDR0 = data;
}


void USART0_PutString(char *ptr){
 
    while(*ptr){   // Loop until end of string (*s = '\0')
        USART0_Transmit(*ptr++); // Send the character and point to the next one
    }
 
}

unsigned char USART0_Receive(void)
{
    while(!(UCSR0A & (1 << RXC0)));

    return UDR0;
}

char USART0_GetChar(void){
 
    char rxdata;

    if (UCSR0A & (1<<RXC0)){  // checking if USART RX data is available
        rxdata = UDR0;   // reading the received byte (clears RXC0)
        return rxdata;   // return the data
    }

    return 0x00;
}

int get_state( void )
{
    while (!(PINB & (1<<PINB0))){
        continue;
    }

    _delay_us(562.5);
    
    if (!(PINB & (1<<PINB0))){
        USART0_Transmit('0');
        return 0;
    }
    
    _delay_us(562.5);
    _delay_us(562.5);

    if (!(PINB & (1<<PINB0))){
        USART0_Transmit('1');
        return 1;
    } else {
        return 2;
    }
}

void *check_long( void )
{

    int iter = 0;
    /* First wait until the initial pulse is complete */
    while (!(PINB & (1<<PINB0))){
        continue;
    }

    _delay_ms(4.5);

    while ((iter = get_state()) != 2){
        continue;  
    }

    USART0_Transmit('\n');

    return iter;
}

int main( void )
{
    int state;
    char buffer[4];

    USART0_Init();
    
    DDRB &= ~(1<<DDB0); //set PB0 as an input pin
    
    while (1) {
        if (!(PINB & (1<<PINB0))) {
            // USART0_PutString("0\n");
            state = check_long();
            // sprintf(buffer, "%d\n", state);
            // USART0_PutString(buffer);
        }
    }
}