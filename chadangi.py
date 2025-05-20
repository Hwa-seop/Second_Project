# parking_sensor.py
import RPi.GPIO as GPIO
import time
import subprocess

# 핀 설정
TRIG = 23
ECHO = 24
LED_RED = 17
LED_GREEN = 27
SERVO_PIN = 18  # 서보모터 제어 핀

GPIO.setmode(GPIO.BCM)

# 출력/입력 설정
GPIO.setup(TRIG, GPIO.OUT)
GPIO.setup(ECHO, GPIO.IN)
GPIO.setup(LED_RED, GPIO.OUT)
GPIO.setup(LED_GREEN, GPIO.OUT)
GPIO.setup(SERVO_PIN, GPIO.OUT)

# 서보 PWM 설정 (50Hz: 일반 서보 모터용)
servo = GPIO.PWM(SERVO_PIN, 50)
servo.start(0)

def set_servo_angle(angle):
    duty = 2 + (angle / 18)  # 각도를 듀티사이클로 변환
    GPIO.output(SERVO_PIN, True)
    servo.ChangeDutyCycle(duty)
    time.sleep(0.5)
    GPIO.output(SERVO_PIN, False)
    servo.ChangeDutyCycle(0)  # 모터 떨림 방지

def get_distance():
    GPIO.output(TRIG, False)
    time.sleep(0.5)

    GPIO.output(TRIG, True)
    time.sleep(0.00001)
    GPIO.output(TRIG, False)

    while GPIO.input(ECHO) == 0:
        pulse_start = time.time()
    while GPIO.input(ECHO) == 1:
        pulse_end = time.time()

    pulse_duration = pulse_end - pulse_start
    distance = pulse_duration * 17150
    return round(distance, 2)

try:
    while True:
        dist = get_distance()
        print(f"Distance: {dist} cm")

        # 서보모터를 센서 작동 시 회전시킴
        set_servo_angle(90)  # 회전 예시
        time.sleep(0.5)
        set_servo_angle(0)   # 원위치

        if dist < 20:  # 주차되어 있음
            GPIO.output(LED_RED, GPIO.HIGH)
            GPIO.output(LED_GREEN, GPIO.LOW)
            parked = 1
        else:          # 비어 있음
            GPIO.output(LED_RED, GPIO.LOW)
            GPIO.output(LED_GREEN, GPIO.HIGH)
            parked = 0

        # C 프로그램 실행 (현재 주석 처리됨)
        # subprocess.run(["./save_parking_status", str(parked)])
        time.sleep(5)

except KeyboardInterrupt:
    print("종료 중...")
    servo.stop()
    GPIO.cleanup()
