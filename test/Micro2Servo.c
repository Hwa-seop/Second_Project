#include <softPwm.h>  // 소프트웨어 PWM 라이브러리 헤더 (서보 모터 제어)
#include <stdio.h>    // 표준 입출력 라이브러리 헤더
#include <stdlib.h>   // 표준 라이브러리 헤더
#include <sys/time.h> // 시간 관련 함수 헤더 파일 (gettimeofday)
#include <unistd.h>   // UNIX 표준 함수 헤더 파일 (delay, delayMicroseconds 등)
#include <wiringPi.h> // wiringPi 라이브러리 헤더 파일 (GPIO 제어)

#define TRIG 5   // 초음파 TRIG 핀 라즈베리 24 -> WiringPi 5
#define ECHO 4   // 초음파 ECHO 핀 라즈베리 23 -> WiringPi 4
#define SERVO 25 // 서보모터 제어 핀 라즈베리 26 -> WiringPi 25 (서보 제어 핀)

long long getMicroseconds()
{
    struct timeval tv;
    gettimeofday(&tv, NULL); // time과 비슷하지만 마이크로초단위까지 반환함
    return (long long)(tv.tv_sec) * 1000000 + tv.tv_usec;
} // 마이크로초 단위로 현재 시간 반환

void rotateServo(int angle)
{
    // 각도(0~180)를 PWM 듀티사이클로 변환 (서보에 따라 다름, 5~25 범위가 일반적)
    int pwm = (angle * 20 / 180) + 5;
    softPwmWrite(SERVO, pwm);
    delay(500); // 회전 시간에 지연해야 함(오류 방지)
} // 서보를 특정 각도로 회전시키는 함수

int main(void)
{
    long long startTime;                   // 초음파 센서에서 방출하는거 시작시간
    long long endTime;                     // 초음파 센서에서 방출하는거 종료시간
    double period, distPractice, distReal; // 펄스 지속 시간, 계산 거리 저장

    if (wiringPiSetup() == -1)
    {
        printf("wiringPi Error\n");
        return 1;
    } // wiringPi 오류 예외처리

    pinMode(TRIG, OUTPUT); // TRIG 핀 출력모드
    pinMode(ECHO, INPUT);  // ECHO 핀 입력모드

    digitalWrite(TRIG, LOW); // TRIG핀 LOW로 초기화(안하면 측정값 이상할 수 있음)
    delay(500);              // 초기화 후 잠시 대기 상태 부여

    // 서보모터 PWM 핀 설정
    softPwmCreate(SERVO, 0, 200); // 0~200 범위 (정밀도에 따라 조정 가능)

    while (1)
    {
        // 초음파 펄스 발생
        digitalWrite(TRIG, LOW);
        delayMicroseconds(100);
        digitalWrite(TRIG, HIGH); // 초음파 발사
        delay(3);                 // 3ms 유지
        digitalWrite(TRIG, LOW);  // 초음파 종료

        // ECHO 핀이 HIGH가 될 때까지 대기
        while (digitalRead(ECHO) == LOW)
        {
            startTime = getMicroseconds(); // ECHO Low인 동안 starttime 갱신함
        }

        // ECHO 핀이 LOW가 될 때까지 대기
        while (digitalRead(ECHO) == HIGH)
        {
            endTime = getMicroseconds(); // ECHO High인 동안 endTime 갱신
        }

        period = (double)(endTime - startTime); // 마이크로초 단위로 갱신
        distPractice = period / 58.0;
        distReal = period * 0.017241; // 초음파 거리 계산하기 (마이크로초 * 음속 * 0.0001)
        // distPractice는 실무에서 사용되는 거리 계산 공식 (센서 공식 문서피셜), distReal은 실제 물리적으로 더 정확한 방법
        // 디버그용으로 두 개 다 써봤음 굳이 둘 다 쓸 필요는 없다

        printf("DistPractice: %.2f cm, distReal: %.2f cm\n", distPractice, distReal); // 디버그용 거리 출력 코드

        if (distPractice < 10.0 && distReal < 10.0)
        {
            printf("detect\n"); // 디버그용 print

            rotateServo(90); // 서보모터 90도 회전

            delay(1000);    // 좀 짧긴함
            rotateServo(0); // 잠시 유지 후 원위치
        }

        delay(500); // 원위치 후 잠시 대기
    }

    return 0; // 무한루프라 안 쓰이긴한데 혹시 몰라서 씀
}
