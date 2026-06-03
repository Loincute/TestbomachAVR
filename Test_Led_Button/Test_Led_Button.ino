#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // LED output
    DDRB |= 0x0F;                    // PB0..PB3
    DDRA |= (1<<PA4)|(1<<PA5)|(1<<PA6)|(1<<PA7);

    // Button input
    DDRD &= ~((1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5)|
              (1<<PD6)|(1<<PD7));

    DDRB &= ~((1<<PB6)|(1<<PB7));

    // Pull-up
    PORTD |= (1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5)|
             (1<<PD6)|(1<<PD7);

    PORTB |= (1<<PB6)|(1<<PB7);

    while(1)
    {
        // Tắt tất cả LED
        PORTB &= ~((1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3));
        PORTA &= ~((1<<PA4)|(1<<PA5)|(1<<PA6)|(1<<PA7));

        // Button 1 -> LED 1
        if(!(PIND & (1<<PD2)))
            PORTB |= (1<<PB0);

        // Button 2 -> LED 2
        if(!(PIND & (1<<PD3)))
            PORTB |= (1<<PB1);

        // Button 3 -> LED 3
        if(!(PIND & (1<<PD4)))
            PORTB |= (1<<PB2);

        // Button 4 -> LED 4
        if(!(PIND & (1<<PD5)))
            PORTB |= (1<<PB3);

        // Button 5 -> LED 5
        if(!(PIND & (1<<PD6)))
            PORTA |= (1<<PA4);

        // Button 6 -> LED 6
        if(!(PIND & (1<<PD7)))
            PORTA |= (1<<PA5);

        // Button 7 -> LED 7
        if(!(PINB & (1<<PB6)))
            PORTA |= (1<<PA6);

        // Button 8 -> LED 8
        if(!(PINB & (1<<PB7)))
            PORTA |= (1<<PA7);
    }
}