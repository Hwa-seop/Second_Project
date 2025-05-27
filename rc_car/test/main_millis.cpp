#include <Arduino.h>

#define IR_RIGHT D0
#define IR_LEFT D1

#define MOTOR_A1 D2
#define MOTOR_A2 D5
#define MOTOR_B1 D6
#define MOTOR_B2 D7

const unsigned long lineLostThreshold = 500; // 0.5초 이상 선을 못 보면 정지
unsigned long lastSeenLineTime = 0;          // 마지막으로 검은선 감지된 시간
const int defaultSpeed = 180;                // 모터 속도 (0~255)

void stopMotors()
{
    analogWrite(MOTOR_A1, 0);
    analogWrite(MOTOR_A2, 0);
    analogWrite(MOTOR_B1, 0);
    analogWrite(MOTOR_B2, 0);
}

void moveForward(int speed)
{
    analogWrite(MOTOR_A1, speed);
    analogWrite(MOTOR_A2, 0);
    analogWrite(MOTOR_B1, speed);
    analogWrite(MOTOR_B2, 0);
}

void turnLeft(int speed)
{
    analogWrite(MOTOR_A1, 0);
    analogWrite(MOTOR_A2, speed);
    analogWrite(MOTOR_B1, speed);
    analogWrite(MOTOR_B2, 0);
}

void turnRight(int speed)
{
    analogWrite(MOTOR_A1, speed);
    analogWrite(MOTOR_A2, 0);
    analogWrite(MOTOR_B1, 0);
    analogWrite(MOTOR_B2, speed);
}

void setup()
{
    Serial.begin(115200);
    analogWriteFreq(25000); // PWM 주파수를 25kHz로 설정

    pinMode(IR_LEFT, INPUT);
    pinMode(IR_RIGHT, INPUT);

    pinMode(MOTOR_A1, OUTPUT);
    pinMode(MOTOR_A2, OUTPUT);
    pinMode(MOTOR_B1, OUTPUT);
    pinMode(MOTOR_B2, OUTPUT);

    stopMotors();
    Serial.println("시작됨: PWM 기반 라인트레이서");
}

unsigned long previousMillis = 0;   // 마지막 업데이트 시간 저장
const unsigned long interval = 500; // 반복 실행 간격 (0.5초)

void loop()
{
    unsigned long currentMillis = millis(); // 현재 시간 가져오기

    // 일정 시간(500ms)마다 코드 실행
    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis; // 마지막 실행 시간 갱신

        int leftIR = digitalRead(IR_LEFT);
        int rightIR = digitalRead(IR_RIGHT);

        Serial.print("IR_LEFT: ");
        Serial.print(leftIR);
        Serial.print(" IR_RIGHT: ");
        Serial.print(rightIR);
        Serial.print(" | lastSeen: ");
        Serial.print(currentMillis - lastSeenLineTime);
        Serial.println("ms");

        if (leftIR == LOW && rightIR == LOW) // 직진
        {
            Serial.println("검은선 중앙 → 직진");
            moveForward(defaultSpeed);
            lastSeenLineTime = currentMillis;
        }
        else if (leftIR == LOW && rightIR == HIGH) // 오른쪽 회전
        {
            Serial.println("검은선 왼쪽에 있음 → 오른쪽 회전");
            turnRight(defaultSpeed);
            lastSeenLineTime = currentMillis;
        }
        else if (leftIR == HIGH && rightIR == LOW) // 왼쪽 회전
        {
            Serial.println("검은선 오른쪽에 있음 → 왼쪽 회전");
            turnLeft(defaultSpeed);
            lastSeenLineTime = currentMillis;
        }
        else if (currentMillis - lastSeenLineTime > lineLostThreshold) // 둘 다 감지 안됨
        {
            Serial.println("선 감지 안 됨 (1초 이상) → 정지");
            stopMotors();
        }
        else
        {
            Serial.println("선 감지 안 됨 → 최근 방향 유지 (직진)");
            moveForward(defaultSpeed);
        }
    }
}
