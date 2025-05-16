# monitor_subscriber.py
import zmq
import time

# 주차 구역 상태 관리
zones = {"A1": False, "A2": False, "B1": False, "B2": False}

context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.connect("tcp://localhost:1004")
print("[서버 실행 중] 기다리는 중..")
socket.setsockopt_string(zmq.SUBSCRIBE, "")

def display():
    print("\n🅿️ 실시간 주차장 상태")
    for zone, status in zones.items():
        state = "🟢 비어있음" if not status else "🔴 주차중"
        print(f"{zone}: {state}")
    print("-" * 30)

while True:
    msg = socket.recv_string()
    try:
        action, zone = msg.strip().split()
        if action == "PARKED":
            zones[zone] = True
        elif action == "LEFT":
            zones[zone] = False

        display()
    except Exception as e:
        print("⚠️ 메시지 오류:", e)
