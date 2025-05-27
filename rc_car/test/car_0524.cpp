#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>

WiFiServer server(6666); // ESP가 열어둘 포트
WiFiClient client;

// WiFi 정보
const char *ssid = "turtle";
const char *password = "turtlebot3";

// 핀 정의
#define IR_RIGHT D0
#define IR_LEFT D1
#define MOTOR_A1 D2
#define MOTOR_A2 D5
#define MOTOR_B1 D6
#define MOTOR_B2 D7

const int defaultSpeed = 200;
const unsigned long lineLostThreshold = 500;
unsigned long lastSeenLineTime = 0;

bool isStopped = false;

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
    analogWriteFreq(25000);

    pinMode(IR_LEFT, INPUT);
    pinMode(IR_RIGHT, INPUT);
    pinMode(MOTOR_A1, OUTPUT);
    pinMode(MOTOR_A2, OUTPUT);
    pinMode(MOTOR_B1, OUTPUT);
    pinMode(MOTOR_B2, OUTPUT);

    stopMotors();
    Serial.println("ESP 라인트레이서 시작 + STOP 명령 대기");

    WiFi.begin(ssid, password);
    Serial.print("WiFi 연결 중");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi 연결 완료!");
    Serial.print("ESP IP 주소: ");
    Serial.println(WiFi.localIP());

    server.begin();
}

void loop()
{
    // STOP 명령 수신 처리
    WiFiClient newClient = server.available();
    if (newClient)
    {
        String cmd = newClient.readStringUntil('\n');
        cmd.trim();
        Serial.println("[수신된 명령] " + cmd);

        if (cmd == "STOP")
        {
            stopMotors();
            isStopped = true;
            Serial.println("🛑 STOP 명령 수신 → RC카 정지됨");
        }
        newClient.stop();
    }

    if (isStopped)
        return; // 멈춘 상태면 라인트레이싱 실행 안 함

    // 라인트레이싱
    int leftIR = digitalRead(IR_LEFT);
    int rightIR = digitalRead(IR_RIGHT);
    unsigned long currentTime = millis();

    if (leftIR == LOW && rightIR == LOW)
    {
        moveForward(defaultSpeed);
        lastSeenLineTime = currentTime;
    }
    else if (leftIR == LOW && rightIR == HIGH)
    {
        turnRight(defaultSpeed);
        lastSeenLineTime = currentTime;
    }
    else if (leftIR == HIGH && rightIR == LOW)
    {
        turnLeft(defaultSpeed);
        lastSeenLineTime = currentTime;
    }
    else if (currentTime - lastSeenLineTime > lineLostThreshold)
    {
        stopMotors();
    }
    else
    {
        moveForward(defaultSpeed);
    }

    delay(70);
}
