#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define I2C_ADDR 0x27 // 또는 0x3F: i2cdetect -y 1로 확인
#define LCD_CHR 1     // 문자 출력 모드
#define LCD_CMD 0     // 명령어 모드

#define LINE1 0x80 // LCD 첫 번째 줄
#define LINE2 0xC0 // LCD 두 번째 줄

#define ENABLE 0b00000100 // Enable 비트
#define BACKLIGHT 0x08    // 백라이트 켜기
#define LCD_WIDTH 16

int fd;

void lcd_toggle_enable(int data)
{
    usleep(500);
    wiringPiI2CWrite(fd, data | ENABLE);
    usleep(500);
    wiringPiI2CWrite(fd, data & ~ENABLE);
    usleep(500);
}

void lcd_send_byte(int bits, int mode)
{
    int high = mode | (bits & 0xF0) | BACKLIGHT;
    int low = mode | ((bits << 4) & 0xF0) | BACKLIGHT;
    wiringPiI2CWrite(fd, high);
    lcd_toggle_enable(high);
    wiringPiI2CWrite(fd, low);
    lcd_toggle_enable(low);
}

void lcd_init()
{
    lcd_send_byte(0x33, LCD_CMD); // 초기화
    lcd_send_byte(0x32, LCD_CMD); // 4비트 모드
    lcd_send_byte(0x28, LCD_CMD); // 2줄 모드, 5x8 도트
    lcd_send_byte(0x0C, LCD_CMD); // 화면 ON, 커서 OFF
    lcd_send_byte(0x06, LCD_CMD); // 문자 입력 시 오른쪽 이동
    lcd_send_byte(0x01, LCD_CMD); // 화면 클리어
    usleep(2000);
}

void lcd_message(const char *msg, int line)
{
    lcd_send_byte(line, LCD_CMD);
    for (int i = 0; i < LCD_WIDTH; i++)
    {
        if (msg[i])
        {
            lcd_send_byte(msg[i], LCD_CHR);
        }
        else
        {
            lcd_send_byte(' ', LCD_CHR); // 남은 공간 공백으로 채우기
        }
    }
}

int main()
{
    if (wiringPiSetup() == -1)
    {
        printf("WiringPi 초기화 실패\n");
        return 1;
    }

    fd = wiringPiI2CSetup(I2C_ADDR);
    if (fd == -1)
    {
        printf("I2C LCD 연결 실패. 주소 확인하세요.\n");
        return 1;
    }

    lcd_init();
    lcd_message("Hello, World!", LINE1);
    lcd_message("sexsex", LINE2);

    printf("test");

    return 0;
}
