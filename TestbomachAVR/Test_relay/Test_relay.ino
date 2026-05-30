// Test Relay dùng chân PA3 - ATmega32

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // PA3 output
    DDRA |= (1 << PA3);

    while (1)
    {
        // Relay ON
        PORTA |= (1 << PA3);
        _delay_ms(1000);

        // Relay OFF
        PORTA &= ~(1 << PA3);
        _delay_ms(1000);
    }
}