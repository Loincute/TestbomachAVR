#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

int main()
{
  DDRB |= (1<<PB0);     // LED output
  DDRD &= ~(1<<PD2);    // Button input

  PORTD |= (1<<PD2);    // Pull-up

  uint8_t state = 0;

  while(1)
  {
    if(!(PIND & (1<<PD2))) // Button pressed
    {
      _delay_ms(20);

      if(!(PIND & (1<<PD2)))
      {
        state ^= 1;

        if(state)
           PORTB |= (1<<PB0);
        else
          PORTB &= ~(1<<PB0);

           while(!(PIND & (1<<PD2)));
      }
    }
  }
}