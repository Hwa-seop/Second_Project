#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define TRIG 5 // BCM 24 -> WiringPi 5
#define ECHO 4 // BCM 23 -> WiringPi 4

// 마이크로초 단위의 현재 시간 반환
long long getMicroseconds()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)(tv.tv_sec) * 1000000 + tv.tv_usec;
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

    digitalWrite(TRIG, LOW);
    delay(500); // 초기 안정화 대기

    while (1)
    {
        // 초음파 펄스 발생
        digitalWrite(TRIG, LOW);
        delayMicroseconds(100);
        digitalWrite(TRIG, HIGH);
        delay(3); // 3ms 유지
        digitalWrite(TRIG, LOW);

        // ECHO 핀이 HIGH가 될 때까지 대기
        while (digitalRead(ECHO) == LOW)
        {
            startTime = getMicroseconds();
        }

        // ECHO 핀이 LOW가 될 때까지 대기
        while (digitalRead(ECHO) == HIGH)
        {
            endTime = getMicroseconds();
        }

        period = (double)(endTime - startTime); // 단위: 마이크로초
        dist1 = period / 58.0;
        dist2 = period * 0.017241; // == period / 58.0

        printf("Dist1: %.2f cm, Dist2: %.2f cm\n", dist1, dist2);

        if (dist1 < 10.0 && dist2 < 10.0)
        {
            printf("detect\n");
        }

        delay(500);
    }

    return 0;
}
