# subscriber.py
import zmq

context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.connect("tcp://localhost:5556")

# 빈 문자열은 모든 메시지 수신
socket.setsockopt_string(zmq.SUBSCRIBE, "")

print("[구독자 실행 중] 센서 메시지 수신 중...")

while True:
    msg = socket.recv_string()
    print("▶ 받은 센서 정보:", msg)
