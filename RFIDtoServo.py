from mfrc522 import SimpleMFRC522
from gpiozero import Servo
from time import sleep
from signal import pause

servo = Servo(26) # 서보모터 GPIO 26번 사용 (BCM 기준, Physical pin 37)

reader = SimpleMFRC522()

print("RFID Sensor on. Tag RFID")

try:
    while True:
        id, _ = reader.read()
        print(f"Detected: {id}")

        # 서보모터 90도 회전
        servo.max()
        sleep(1)

        # 5초 대기 후 원위치
        sleep(5)
        servo.min()  # 다시 0도로 복귀
        sleep(1)

except KeyboardInterrupt:
    print("\nInterrupt End")
