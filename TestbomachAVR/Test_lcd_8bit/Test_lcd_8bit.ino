#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define RS PC2
#define EN PC3

void pulse() {
	PORTC |= (1<<EN);
	_delay_us(1);
	PORTC &= ~(1<<EN);
	_delay_ms(2);
}

void lcd_send(unsigned char d) {

	// D0-D3 -> PA4-PA7
	PORTA &= 0x0F;
	PORTA |= (d & 0x0F) << 4;

	// D4-D7 -> PC4-PC7
	PORTC &= 0x0F;
	PORTC |= (d & 0xF0);

	pulse();
}

void cmd(unsigned char c) {
	PORTC &= ~(1<<RS);
	lcd_send(c);
}

void data(unsigned char c) {
	PORTC |= (1<<RS);
	lcd_send(c);
}

void print(char *s) {
	while(*s) data(*s++);
}

void lcd_init() {

	DDRA |= 0xF0;
	DDRC |= 0xFC;

	_delay_ms(20);

	cmd(0x38);
	cmd(0x06);
	cmd(0x01);
}

int main() {

	lcd_init();

	cmd(0x80);

	print("LCD 8 BIT");

	while(1);
}