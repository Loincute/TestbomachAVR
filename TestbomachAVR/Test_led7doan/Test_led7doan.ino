#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

const uint8_t seg_code[10] =
{
    0b00111111, //0
    0b00000110, //1
    0b01011011, //2
    0b01001111, //3
    0b01100110, //4
    0b01101101, //5
    0b01111101, //6
    0b00000111, //7
    0b01111111, //8
    0b01101111  //9
};

void off_all_digit()
{
    PORTD &= ~((1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5));
}

void display_digit(uint8_t digit, uint8_t number)
{
    off_all_digit();

    PORTB = seg_code[number];

    switch(digit)
    {
        case 0:
            PORTD |= (1<<PD2);
            break;

        case 1:
            PORTD |= (1<<PD3);
            break;

        case 2:
            PORTD |= (1<<PD4);
            break;

        case 3:
            PORTD |= (1<<PD5);
            break;
    }

    _delay_ms(2);
}

void display_number(uint16_t number)
{
    uint8_t d0, d1, d2, d3;

    d0 = number / 1000;
    d1 = (number / 100) % 10;
    d2 = (number / 10) % 10;
    d3 = number % 10;

    display_digit(0, d0);
    display_digit(1, d1);
    display_digit(2, d2);
    display_digit(3, d3);
}

int main(void)
{
    uint16_t count = 0;

    DDRB = 0xFF;

    DDRD |= (1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5);

    off_all_digit();

    while(1)
    {
        for(count = 0; count <= 9999; count++)
        {
            for(uint16_t i = 0; i < 100; i++)
            {
                display_number(count);
            }
        }
    }
}