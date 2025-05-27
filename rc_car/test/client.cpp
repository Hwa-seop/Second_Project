#include <ESP8266WiFi.h>

const char *ssid = "turtle";
const char *password = "turtlebot3";

const char *host = "192.168.0.54"; // 서버 IP
const uint16_t port = 5511;

WiFiClient client;

#define LED_RED_PIN D5
#define LED_GREEN_PIN D6

void setup()
{
    Serial.begin(115200);
    delay(1000);

    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);

    digitalWrite(LED_RED_PIN, LOW);   // OFF (ESP8266에서는 HIGH가 꺼진 상태)
    digitalWrite(LED_GREEN_PIN, LOW); // OFF (ESP8266에서는 HIGH가 꺼진 상태)

    WiFi.begin(ssid, password);
    Serial.print("WiFi 연결 중");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi 연결 완료!");
    Serial.print("ESP IP: ");
    Serial.println(WiFi.localIP());

    if (!client.connect(host, port))
    {
        Serial.println("서버 연결 실패!");
        return;
    }
    Serial.println("서버 연결 성공!");

    // String message = "turn_on"; // 서버에게 LED 켜달라고 요청
    // client.print(message);
    // Serial.println("메시지 전송: " + message);
}

void loop()
{
    // WiFi 연결 확인
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("에러: WiFi 연결 끊어짐!");
        return;
    }

    // 서버 연결 확인
    if (!client.connected())
    {
        Serial.println("에러: 서버 연결 끊어짐! 재연결 시도...");
        if (!client.connect(host, port))
        {
            Serial.println("서버 재연결 실패!");
            return;
        }
        Serial.println("서버 재연결 성공!");
    }

    // 메시지 전송
    String message = "2request";
    client.print(message);
    Serial.println("메시지 전송: " + message);

    // 서버 응답 대기
    unsigned long startTime = millis();
    while (!client.available())
    {
        if (millis() - startTime > 3000)
        {
            Serial.println("에러: 서버 응답 없음! 연결 재설정...");
            client.stop(); // 연결 초기화
            return;
        }
    }
    digitalWrite(LED_RED_PIN, HIGH);   // 켜기 (ESP8266에서는 LOW가 ON)
    digitalWrite(LED_GREEN_PIN, HIGH); // 켜기 (ESP8266에서는 LOW가 ON)

    // 응답 처리
    String response = client.readStringUntil('\n');
    response.trim();
    Serial.println("서버 응답: " + response);

    if (response.startsWith("full"))
    {
        digitalWrite(LED_RED_PIN, LOW); // 켜기 (ESP8266에서는 LOW가 ON)
        Serial.println("LED RED ON");
        delay(1000);
    }
    else if (response.startsWith("empty"))
    {
        digitalWrite(LED_GREEN_PIN, LOW); // 끄기
        Serial.println("LED GREEN ON");
        delay(1000);
    }
    else
    {
        digitalWrite(LED_RED_PIN, LOW);
        digitalWrite(LED_GREEN_PIN, LOW);
        Serial.println("LED OFF");
    }
    response = "";

    delay(5000);
}
