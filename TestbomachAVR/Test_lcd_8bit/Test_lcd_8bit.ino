#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define RS PC2
#define EN PC3

// Hàm gửi dữ liệu/lệnh 8-bit chia ra 2 PORT
void lcd_send(unsigned char d) {
    // 1. Xử lý PORTA (D0-D3 nối vào PA4-PA7)
    // Xóa sạch 4 bit cao của PORTA, giữ nguyên 4 bit thấp
    PORTA &= 0x0F;
    // Lấy 4 bit thấp của d, dịch trái 4 vị trí để đưa lên PA4-PA7
    PORTA |= (d << 4) & 0xF0;

    // 2. Xử lý PORTC (D4-D7 nối vào PC4-PC7)
    // Xóa sạch 4 bit cao của PORTC, giữ nguyên các chân RS (PC2) và EN (PC3)
    PORTC &= 0x0F;
    // Lấy 4 bit cao của d đưa thẳng vào PC4-PC7
    PORTC |= (d & 0xF0);

    // 3. Tạo xung Enable kích hoạt nhận dữ liệu
    PORTC |= (1 << EN);
    _delay_us(1);
    PORTC &= ~(1 << EN);
    _delay_ms(2); // Chờ LCD xử lý xong lệnh
}

// Hàm gửi lệnh (RS = 0)
void cmd(unsigned char c) {
    PORTC &= ~(1 << RS); // Kéo RS xuống 0
    lcd_send(c);
}

// Hàm gửi dữ liệu hiển thị (RS = 1)
void data(unsigned char c) {
    PORTC |= (1 << RS);  // Kéo RS lên 1
    lcd_send(c);
}

// Hàm in một chuỗi ký tự
void print(char *s) {
    while (*s) {
        data(*s++);
    }
}

// Hàm khởi tạo LCD ở chế độ 8-bit
void lcd_init() {
    // Cấu hình chân Output: PA4-PA7 (0xF0) và PC2-PC7 (0xFC)
    DDRA |= 0xF0;
    DDRC |= 0xFC;

    _delay_ms(20); // Chờ nguồn ổn định

    cmd(0x38); // Lệnh cấu hình: 8-bit, hiển thị 2 dòng, font 5x8
    cmd(0x0C); // Bật hiển thị màn hình, tắt con trỏ nhấp nháy
    cmd(0x06); // Tự động tăng con trỏ sang phải khi ghi ký tự
    cmd(0x01); // Xóa sạch màn hình
    _delay_ms(2);
}

int main() {
    // Khởi tạo hệ thống LCD
    lcd_init();

    // Di chuyển con trỏ về dòng 1, cột 1 (Địa chỉ 0x80)
    cmd(0x80);

    // In chuỗi kết quả
    print("LCD 8 BIT");

    // Vòng lặp vô tận giữ nguyên màn hình
    while (1);
}