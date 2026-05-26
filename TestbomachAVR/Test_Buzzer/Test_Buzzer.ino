#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

int main()
{
    DDRD |= (1<<PD7); // Buzzer output

    while(1)
    {
        PORTD |= (1<<PD7); // ON
        _delay_ms(1000);

        PORTD &= ~(1<<PD7); // OFF
        _delay_ms(1000);
    }
}