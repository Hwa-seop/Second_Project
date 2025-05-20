#define TRIG D6
#define ECHO D5
#define RED  D1
#define GREEN D2

void setup() {
  Serial.begin(9600);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);

  digitalWrite(RED, LOW);
  digitalWrite(GREEN, HIGH); // 기본 상태 녹색
}

void loop() {
  long duration;
  float distance;

  // 초음파 펄스 발생
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // ECHO 핀에서 HIGH가 유지된 시간 측정
  duration = pulseIn(ECHO, HIGH); // 마이크로초 단위
  distance = duration / 58.0;     // cm로 변환

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // 거리 판단
  if (distance < 10.0) {
    Serial.println("detect");
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
  } else {
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);
  }

  delay(500);
}
