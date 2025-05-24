#include <Arduino.h>

#define IR_RIGHT D0
#define IR_LEFT D1
#define OBSTACLE_SENSOR D3 // HW-201 센서 연결 (D3 → GPIO0)

#define MOTOR_A1 D2
#define MOTOR_A2 D5
#define MOTOR_B1 D6
#define MOTOR_B2 D7

const unsigned long lineLostThreshold = 500; // 0.5초 이상 선을 못 보면 정지
unsigned long lastSeenLineTime = 0;          // 마지막으로 검은선 감지된 시간
const int defaultSpeed = 210;                // 모터 속도 (0~255)

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
    pinMode(OBSTACLE_SENSOR, INPUT); // HW-201 센서 입력 설정

    pinMode(MOTOR_A1, OUTPUT);
    pinMode(MOTOR_A2, OUTPUT);
    pinMode(MOTOR_B1, OUTPUT);
    pinMode(MOTOR_B2, OUTPUT);

    stopMotors();
    Serial.println("시작됨: PWM 기반 라인트레이서 + 장애물 감지 추가");
}

void loop()
{
    int leftIR = digitalRead(IR_LEFT);
    int rightIR = digitalRead(IR_RIGHT);
    int obstacleDetected = digitalRead(OBSTACLE_SENSOR); // 장애물 감지

    unsigned long currentTime = millis();

    Serial.print("IR_LEFT: ");
    Serial.print(leftIR);
    Serial.print(" IR_RIGHT: ");
    Serial.print(rightIR);
    Serial.print(" | 장애물 감지: ");
    Serial.print(obstacleDetected);
    Serial.print(" | lastSeen: ");
    Serial.print(currentTime - lastSeenLineTime);
    Serial.println("ms");

    // 장애물 감지 시 즉시 정지
    if (obstacleDetected == LOW)
    {
        Serial.println("장애물 감지됨 → 정지!");
        stopMotors();
        delay(500); // 0.5초 대기 후 계속 체크
        return;
    }

    // 라인트레이싱 동작
    if (leftIR == LOW && rightIR == LOW) // 직진
    {
        Serial.println("검은선 중앙 → 직진");
        moveForward(defaultSpeed);
        lastSeenLineTime = currentTime;
    }
    else if (leftIR == LOW && rightIR == HIGH) // 오른쪽회전
    {
        Serial.println("검은선 왼쪽에 있음 → 오른쪽 회전");
        turnRight(defaultSpeed);
        lastSeenLineTime = currentTime;
    }
    else if (leftIR == HIGH && rightIR == LOW) // 왼쪽회전
    {
        Serial.println("검은선 오른쪽에 있음 → 왼쪽 회전");
        turnLeft(defaultSpeed);
        lastSeenLineTime = currentTime;
    }
    else if (currentTime - lastSeenLineTime > lineLostThreshold) // 둘 다 감지 안됨
    {
        Serial.println("선 감지 안 됨 (1초 이상) → 정지");
        stopMotors();
    }
    else
    {
        Serial.println("선 감지 안 됨 → 최근 방향 유지 (직진)");
        moveForward(defaultSpeed);
    }

    delay(70);
}
