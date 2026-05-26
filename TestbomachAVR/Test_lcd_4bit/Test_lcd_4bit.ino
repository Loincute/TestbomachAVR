#include <LiquidCrystal.h>

LiquidCrystal lcd(16, 17, 20, 21, 22, 23);

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LCD PORTC
#define DDR DDRC

#define RS PC2
#define EN PC3

void pulse() {
	LCD |= (1<<EN);
	_delay_us(1);
	LCD &= ~(1<<EN);
}

void send4(char d) {
	LCD = (LCD & 0x0F) | (d & 0xF0);
	pulse();
}

void cmd(char c) {
	LCD &= ~(1<<RS);
	send4(c);
	send4(c<<4);
	_delay_ms(2);
}

void data(char c) {
	LCD |= (1<<RS);
	send4(c);
	send4(c<<4);
	_delay_ms(2);
}

void print(char *s) {
	while(*s) data(*s++);
}

void lcd_init() {
	DDR = 0xFC;   // PC2-PC7 output
	_delay_ms(20);

	cmd(0x02);
	cmd(0x28);
	cmd(0x0C);
	cmd(0x06);
	cmd(0x01);
}

int main() {
	lcd_init();

	cmd(0x80);
	print("HELLO ATMEGA32");

	while(1);
}