#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void put(char c)
{
    while(!(UCSRA&(1<<UDRE)));
    UDR=c;
}

void print(char *s)
{
    while(*s) put(*s++);
}

int main()
{
    char s[20];

    UBRRL=51;
    UCSRB=(1<<TXEN);
    UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);

    ADMUX=(1<<REFS0)|(1<<MUX1); // ADC2 = PA2
    ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);

    while(1)
    {
        ADCSRA|=(1<<ADSC);
        while(ADCSRA&(1<<ADSC));

        sprintf(s,"LDR=%u\r\n",ADC);

        print(s);

        _delay_ms(500);
    }
}