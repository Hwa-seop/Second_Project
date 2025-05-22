#include <ESP8266WiFi.h>

const char *ssid = "turtle";
const char *password = "turtlebot3";

const char *host = "192.168.0.62"; // 서버 IP
const uint16_t port = 5511;

WiFiClient client;

#define LED_PIN D2 // 또는 D4

void setup()
{
    Serial.begin(115200);
    delay(1000);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH); // OFF (ESP8266에서는 HIGH가 꺼진 상태)

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

    String message = "turn_on"; // 서버에게 LED 켜달라고 요청
    client.print(message);
    Serial.println("메시지 전송: " + message);
}

void loop()
{
    if (client.available())
    {
        String response = client.readStringUntil('\n');
        Serial.println("서버 응답: " + response);

        if (response == "1")
        {
            digitalWrite(LED_PIN, LOW); // 켜기 (ESP8266에서는 LOW가 ON)
            Serial.println("LED ON");
        }
        else if (response == "0")
        {
            digitalWrite(LED_PIN, HIGH); // 끄기
            Serial.println("LED OFF");
        }
    }
    delay(5000);
    String message = "OFF\n"; // 명시적으로 개행 추가
    client.print(message);

    Serial.println("메시지 전송: " + message);
    delay(1000);
}
