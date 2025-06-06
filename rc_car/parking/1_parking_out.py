from mfrc522 import SimpleMFRC522
from gpiozero import Servo
from time import sleep
import zmq
import spidev
from RPLCD.i2c import CharLCD

MAX_RETRIES = 5

#서보모터 설정 및 초기화
servo = Servo(26) # 서보모터 GPIO 26번 사용 (BCM 기준, Physical pin 37)
servo.max()
sleep(1)
servo.min()

# LCD 설정 및 초기화
# LCD 설정 - I2C 주소는 보통 0x27 또는 0x3f
lcd = CharLCD(i2c_expander='PCF8574', address=0x27, port=1, cols=16, rows=2, charmap='A00')

# # 차량 입차 함수
# def car_in(rfid_id):
#     message = f"INSERT INTO parking_log (RFID_num, entry_time, current_status) SELECT {rfid_id}, NOW(), 'In' WHERE NOT EXISTS (SELECT 1 FROM parking_log WHERE RFID_num = {rfid_id} AND current_status = 'In');"
#     socket.send_string(message)  # 요청 전송
#     response = socket.recv_string()  # 응답 수신
#     print(f"Server Response: {response}")

# 차량 출차 함수
def car_out(rfid_id):
    message = f"UPDATE parking_log SET exit_time = NOW(), total_usage_time = SEC_TO_TIME(TIMESTAMPDIFF(SECOND, entry_time, NOW())), current_status = 'Out' WHERE RFID_num = {rfid_id} AND current_status = 'In';"
    socket.send_string(message)  # 요청 전송
    response = socket.recv_string()  # 응답 수신
    print(f"Server Response: {response}")
    
# ZeroMQ 소켓 설정
context = zmq.Context()
print("ZeroMQ 컨텍스트 생성 완료...")
socket = context.socket(zmq.REQ)
for attempt in range(MAX_RETRIES):
    try:
        socket.connect("tcp://192.168.0.54:6513")
        print("ZeroMQ 서버 연결 완료")
        break
    except zmq.ZMQError as e:
        print(f"서버 연결 실패: {e}, 재시도 중... ({attempt+1}/{MAX_RETRIES})")
        sleep(2)
else:
    print("서버 연결 실패, 프로그램 종료")
    exit(1)

reader = SimpleMFRC522()


# RFID 태그 감지 루프
try:
    while True: 
        print("RFID Sensor on. Tag RFID")
        rfid_id, _ = reader.read()
        print(f"Detected RFID: {rfid_id}")
        
        # 서버에 초기 RFID 리스트 요청
        socket.send_string("INIT_REQUEST")  # 서버에 초기 RFID 데이터 요청

        try:
            rfid_list = socket.recv_string()  # 서버로부터 RFID 리스트 수신
            pre_id = rfid_list.split(",") if rfid_list else []
            print(f"초기 RFID 리스트 수신 완료: {pre_id}")
        except zmq.Again:
            print("서버 응답 지연 - 초기 RFID 리스트를 받을 수 없습니다.")
            pre_id = []

        # 서보모터 90도 회전
        servo.max()
        sleep(1)

        # 일정 시간 대기 후 원위치 복귀
        sleep(5)
        servo.min()
        sleep(1)

        # RFID 태그 판별하여 입출차 처리
        # if str(rfid_id) not in pre_id:  # 새로운 태그일 경우 입차
        #     car_in(rfid_id)
        #     pre_id.append(str(rfid_id)) 
        #     print(pre_id)
        if str(rfid_id) in pre_id:  # 태그값이 동일하면 출차 처리
            car_out(rfid_id)
            socket.send_string(f"REQUEST_USAGE_TIME, {rfid_id}")  # 서버에 초기 RFID 데이터 요청
            try:
                usage_time = socket.recv_string()  # 서버로부터 RFID 리스트 수신
                print(f"usage time 수신 완료: {usage_time}")
                lcd.clear()
                lcd.write_string("Parking Fee:")
                lcd.cursor_pos = (1, 0)  # 두 번째 줄
                lcd.write_string(f"{usage_time} WON")
            except zmq.Again:
                print("서버 응답 지연 - usage time을 받을 수 없습니다.")
            pre_id.remove(str(rfid_id))   # 출차 후 ID 초기화
            print(pre_id)
        else:  
            sleep(2)  # 짧은 대기 후 재확인
            print("이미 출차한 RFID")

except KeyboardInterrupt:
    print("\nInterrupt End")