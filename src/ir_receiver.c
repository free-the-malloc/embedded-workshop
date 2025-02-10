
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <avr/io.h>

#include <serial.h>

#define BIN_LEN (64)
#define BUFF_LEN (256)

#define INPUT_HIGH  (PINB & (1<<PINB0))
#define INPUT_LOW   (!(PINB & (1<<PINB0)))

#define BURST_PERIOD (562.2)
#define CHECK_PERIOD (BURST_PERIOD * 2)


/**
 * IR Receiver used: ZD1592
 * IR Protocol: NEC
 * Input Pin: B0
 * 
 * The ZD1592 works by pushing current through the specified input 
 * pin, breaking that current when IR is received. As such, when IR 
 * is received, the input on the pin should be read as low. 
 * 
 * The NEC IR communication protocol builds a message frame by the 
 * following steps: 
 *  1. A 9 millisecond burst of IR followed by a 4.5 millisecond low 
 * period. 
 * 
 *  2. Now the actual message is constructed. A '0' is signalled by a
 * 562.2 microsecond burst of IR followed by a 562.2 microsecond low 
 * period. A '1' is signalled by a 562.2 microsecond burst of IR, 
 * followed by 1,687 microsecond (or 1.687 millisecond) low period. 
 * 
 *  3. Typically, the 8-bit address of the device followed by it's 
 * logical inverse is transmitted after the initial pulse burst and 
 * low period. That is followed by the 8-bit message and it's logical
 * inverse. 
 * 
 * Function writes the binary message read into the buffer supplied. 
 * Returns 1 if unsuccessful and 0 if successful. 
 */
int NEC_read_transmission( char *buffer )
{
    unsigned int index = 0;

    _delay_ms(9);

    if (INPUT_LOW){ /* check if input is still high (it shouldn't be) */
        return 1;
    }

    _delay_ms(4.5); /* 4.5ms low period */

    /**
     * Begin reading the message. Each iteration of the loop 
     * should begin with a pulse of IR, hence the initial if 
     * statement. 
     */
    while (1){
        if (INPUT_HIGH){
            return 1;
        }

        _delay_us(CHECK_PERIOD);
        if (INPUT_LOW){
            buffer[index] = '0';
            index++;
            continue;
        }

        _delay_us(CHECK_PERIOD);
        if (INPUT_LOW){
            buffer[index] = '1';
            index++;
            continue;
        } 

        break;
    }
    buffer[index] = '\0';
    return 0;
}


int main( void )
{
    char binary[BIN_LEN], *endptr, buffer[BUFF_LEN];
    unsigned long hexval;

    USART0_Init();
    
    DDRB &= ~(1<<DDB0); //set PB0 as an input pin
    PORTB |= (1<<PORTB0);
    
    _delay_ms(500);

    while (1) {
        if (INPUT_LOW) {
            if (NEC_read_transmission(binary)){
                _delay_ms(90);
                continue;
            }
            hexval = strtol(binary, &endptr, 2);
            snprintf(buffer, 256, "binary: %s\nhex: %06lx\n", binary, (unsigned long) hexval & 0xFFFFFFUL);
            USART0_PutString(buffer);
        }
    }

    return 1;
}
