# parking_sensor.py
import RPi.GPIO as GPIO
import time
import subprocess

# 핀 설정
TRIG = 23
ECHO = 24
LED_RED = 17
LED_GREEN = 27

GPIO.setmode(GPIO.BCM)
GPIO.setup(TRIG, GPIO.OUT)
GPIO.setup(ECHO, GPIO.IN)
GPIO.setup(LED_RED, GPIO.OUT)
GPIO.setup(LED_GREEN, GPIO.OUT)

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

        if dist < 20:  # 주차되어 있음
            GPIO.output(LED_RED, GPIO.HIGH)
            GPIO.output(LED_GREEN, GPIO.LOW)
            parked = 1
        else:          # 비어 있음
            GPIO.output(LED_RED, GPIO.LOW)
            GPIO.output(LED_GREEN, GPIO.HIGH)
            parked = 0

        # C 프로그램 실행: parking_db 저장
        subprocess.run(["./save_parking_status", str(parked)])
        time.sleep(5)

except KeyboardInterrupt:
    print("종료 중...")
    GPIO.cleanup()
