#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

// 초음파 센서 핀
#define TRIG 5 // BCM 24 -> WiringPi 5
#define ECHO 4 // BCM 23 -> WiringPi 4

// RGB LED 핀 (공통 음극 기준)
#define RED 0
#define GREEN 2
// BLUE는 사용 안 함

// 마이크로초 단위의 현재 시간 반환
long long getMicroseconds()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)(tv.tv_sec) * 1000000 + tv.tv_usec;
}

// RGB LED 색상 설정 함수
void setLEDColor(int redVal, int greenVal)
{
    digitalWrite(RED, redVal);
    digitalWrite(GREEN, greenVal);
}

int main(void)
{
    long long startTime, endTime;
    double period, dist1, dist2;

    if (wiringPiSetup() == -1)
    {
        printf("WiringPi Setup failed\n");
        return 1;
    }

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);

    digitalWrite(TRIG, LOW);
    delay(500); // 센서 안정화 대기

    while (1)
    {
        // 초음파 펄스 발생
        digitalWrite(TRIG, LOW);
        delayMicroseconds(100);
        digitalWrite(TRIG, HIGH);
        delay(3);
        digitalWrite(TRIG, LOW);

        // ECHO 핀이 HIGH 될 때까지 대기
        while (digitalRead(ECHO) == LOW)
        {
            startTime = getMicroseconds();
        }

        // ECHO 핀이 LOW 될 때까지 대기
        while (digitalRead(ECHO) == HIGH)
        {
            endTime = getMicroseconds();
        }

        period = (double)(endTime - startTime);
        dist1 = period / 58.0;
        dist2 = period * 0.017241;

        printf("Dist1: %.2f cm, Dist2: %.2f cm\n", dist1, dist2);

        if (dist1 < 10.0 && dist2 < 10.0)
        {
            printf("detect\n");
            setLEDColor(HIGH, LOW); // 빨간색
        }
        else
        {
            setLEDColor(LOW, HIGH); // 녹색
        }

        delay(500);
    }

    return 0;
}
