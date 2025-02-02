

#include <avr/io.h>

#include <serial.h>


/**
 * 0 is represented by a 562.2 microsecond burst followed by a 562.2
 * microsecond low period and a 1 is represented by a 562.2 microsecond 
 * burst followed by a 1,687 microsecond low period. 
 */
void read_transmission( char *buffer )
{
    unsigned int state = 0;
    unsigned int index = 0;

    /* first wait out the initial transmission */
    while (!(PINB & (1<<PINB0)));

    /* wait for the 4.5 seconds of silence */
    while (PINB & (1<<PINB0));

    /* now read the message */
    while (state <= 4){
        if (state > 0){
            _delay_us(565);
            if (!(PINB & (1<<PINB0))){
                buffer[index] = '0';
                state = 0;
                index++;
                continue;
            }

            _delay_us(1130);
            if (!(PINB & (1<<PINB0))){
                buffer[index] = '1';
                state = 0;
                index++;
                continue;
            } else {
                break;
            }
        }

        if (!(PINB & (1<<PINB0))){
            while (!(PINB & (1<<PINB0)));

            state = 1;
            continue;
        }
        break;
    }
    buffer[index] = '\0';
}

int main( void )
{
    int state;
    char buffer[33];

    USART0_Init();
    
    DDRB &= ~(1<<DDB0); //set PB0 as an input pin
    
    while (1) {
        if (!(PINB & (1<<PINB0))) { /* if pin B0 is low */
            read_transmission(buffer);
            USART0_PutString(buffer);
            USART0_Transmit('\n');
        }
    }
}
