#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// ── LCD 4-bit ──
#define LCD_RS (1<<PC2)
#define LCD_E  (1<<PC3)

void lcd_pulse()
{
    PORTC |=  LCD_E;
    _delay_us(1);
    PORTC &= ~LCD_E;
    _delay_us(50);
}

void lcd_nibble(uint8_t n)
{
    PORTC = (PORTC & 0x0F) | (n & 0xF0);
    lcd_pulse();
}

void lcd_cmd(uint8_t cmd)
{
    PORTC &= ~LCD_RS;
    lcd_nibble(cmd);
    lcd_nibble(cmd << 4);
    _delay_ms(2);
}

void lcd_data(uint8_t data)
{
    PORTC |= LCD_RS;
    lcd_nibble(data);
    lcd_nibble(data << 4);
    _delay_us(50);
}

void lcd_init()
{
    DDRC = 0xFF;
    _delay_ms(20);
    PORTC &= ~LCD_RS;
    lcd_nibble(0x30); _delay_ms(5);
    lcd_nibble(0x30); _delay_ms(1);
    lcd_nibble(0x30); _delay_ms(1);
    lcd_nibble(0x20); _delay_ms(1);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
    _delay_ms(2);
}

void lcd_goto(uint8_t row, uint8_t col)
{
    lcd_cmd((row == 0 ? 0x80 : 0xC0) + col);
}

void lcd_print(const char *s)
{
    while (*s) lcd_data(*s++);
}

// ── I2C ──
void i2c_init()
{
    TWSR = 0;
    TWBR = 32;
}

void i2c_start()
{
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
}

void i2c_stop()
{
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
    _delay_us(10);
}

void i2c_write(uint8_t data)
{
    TWDR = data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
}

uint8_t i2c_read_ack()
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while(!(TWCR & (1<<TWINT)));
    return TWDR;
}

uint8_t i2c_read_nack()
{
    TWCR = (1<<TWINT)|(1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    return TWDR;
}

// ── DS3231 ──
#define DS3231_ADDR 0x68

uint8_t bcd2dec(uint8_t b) { return (b>>4)*10 + (b&0x0F); }
uint8_t dec2bcd(uint8_t d) { return ((d/10)<<4) | (d%10); }

struct RTC_Time {
    uint8_t sec, min, hour, day, month;
    uint16_t year;
};

void ds3231_set(uint8_t sec, uint8_t min, uint8_t hour,
                uint8_t day, uint8_t month, uint8_t year)
{
    i2c_start();
    i2c_write(DS3231_ADDR << 1);
    i2c_write(0x00);
    i2c_write(dec2bcd(sec));
    i2c_write(dec2bcd(min));
    i2c_write(dec2bcd(hour));
    i2c_write(0x01);             // day of week (bỏ qua)
    i2c_write(dec2bcd(day));
    i2c_write(dec2bcd(month));
    i2c_write(dec2bcd(year));    // 2 chữ số cuối năm
    i2c_stop();
}

struct RTC_Time ds3231_read()
{
    struct RTC_Time dt;
    i2c_start();
    i2c_write(DS3231_ADDR << 1);
    i2c_write(0x00);
    i2c_start();
    i2c_write((DS3231_ADDR << 1) | 1);
    dt.sec   = bcd2dec(i2c_read_ack());
    dt.min   = bcd2dec(i2c_read_ack());
    dt.hour  = bcd2dec(i2c_read_ack());
    i2c_read_ack();
    dt.day   = bcd2dec(i2c_read_ack());
    dt.month = bcd2dec(i2c_read_ack());
    dt.year  = 2000 + bcd2dec(i2c_read_nack());
    i2c_stop();
    return dt;
}

int main()
{
    char s[17];
    struct RTC_Time dt;

    i2c_init();
    lcd_init();

    // Đặt thời gian: sec, min, hour, day, month, year(2 số cuối)
    ds3231_set(0, 15, 14, 4, 6, 26);  // 08:30:00 - 04/06/2026

    while(1)
    {
        dt = ds3231_read();

        lcd_goto(0, 0);
        sprintf(s, "Time:%02d:%02d:%02d", dt.hour, dt.min, dt.sec);
        lcd_print(s);

        lcd_goto(1, 0);
        sprintf(s, "Date:%02d/%02d/%04d", dt.day, dt.month, dt.year);
        lcd_print(s);

        _delay_ms(1000);
    }
}