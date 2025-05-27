#include <Arduino.h>

#define IR_RIGHT D0
#define IR_LEFT D1

#define MOTOR_A1 D2
#define MOTOR_A2 D5
#define MOTOR_B1 D6
#define MOTOR_B2 D7

const unsigned long lineLostThreshold = 500; // 0.5초 이상 선을 못 보면 정지
unsigned long lastSeenLineTime = 0;          // 마지막으로 검은선 감지된 시간

void stopMotors()
{
    digitalWrite(MOTOR_A1, LOW);
    digitalWrite(MOTOR_A2, LOW);
    digitalWrite(MOTOR_B1, LOW);
    digitalWrite(MOTOR_B2, LOW);
}

void moveForward()
{
    digitalWrite(MOTOR_A1, HIGH);
    digitalWrite(MOTOR_A2, LOW);
    digitalWrite(MOTOR_B1, HIGH);
    digitalWrite(MOTOR_B2, LOW);
}

void turnLeft()
{
    digitalWrite(MOTOR_A1, LOW);
    digitalWrite(MOTOR_A2, HIGH);
    digitalWrite(MOTOR_B1, HIGH);
    digitalWrite(MOTOR_B2, LOW);
}

void turnRight()
{
    digitalWrite(MOTOR_A1, HIGH);
    digitalWrite(MOTOR_A2, LOW);
    digitalWrite(MOTOR_B1, LOW);
    digitalWrite(MOTOR_B2, HIGH);
}

void setup()
{
    Serial.begin(115200);

    pinMode(IR_LEFT, INPUT);
    pinMode(IR_RIGHT, INPUT);

    pinMode(MOTOR_A1, OUTPUT);
    pinMode(MOTOR_A2, OUTPUT);
    pinMode(MOTOR_B1, OUTPUT);
    pinMode(MOTOR_B2, OUTPUT);

    stopMotors();
    Serial.println("시작됨: 양방향 IR 기반 라인트레이서");
    Serial.println(analogRead(IR_LEFT)); // 센서 값 확인
}

void loop()
{
    int leftIR = digitalRead(IR_LEFT);
    int rightIR = digitalRead(IR_RIGHT);
    unsigned long currentTime = millis();

    Serial.print("IR_LEFT: ");
    Serial.print(leftIR);
    Serial.print("IR_RIGHT: ");
    Serial.print(rightIR);
    Serial.print("lastSeen: ");
    Serial.print(currentTime - lastSeenLineTime);
    Serial.println("ms");

    if (leftIR == LOW && rightIR == LOW) // 직진
    {
        Serial.println("검은선 중앙 → 직진");
        moveForward();
        lastSeenLineTime = currentTime; // 감지 시간 갱신
    }
    else if (leftIR == LOW && rightIR == HIGH) // 오른쪽회전
    {
        Serial.println("검은선 왼쪽에 있음 → 오른쪽 회전");
        turnRight();
        lastSeenLineTime = currentTime;
    }
    else if (leftIR == HIGH && rightIR == LOW) // 왼쪽회전
    {
        Serial.println("검은선 오른쪽에 있음 → 왼쪽 회전");
        turnLeft();
        lastSeenLineTime = currentTime;
    }
    else if (currentTime - lastSeenLineTime > lineLostThreshold) // 둘다 감지 안됨
    {
        Serial.println("선 감지 안 됨 (1초 이상) → 정지");
        stopMotors();
    }
    else
    {
        Serial.println("선 감지 안 됨 → 최근 방향 유지 (직진)");
        moveForward();
    }

    delay(500);
}