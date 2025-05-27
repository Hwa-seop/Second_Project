# monitor_subscriber.py
import zmq

zones = {
    "A1": False, "A2": False, "B1": False, "B2": False,
    "C1": False, "C2": False, "D1": False, "D2": False
}

context = zmq.Context()

# 첫 번째 PUB 소켓 (sensor_input.py)
socket1 = context.socket(zmq.SUB)
socket1.connect("tcp://localhost:5555")
socket1.setsockopt_string(zmq.SUBSCRIBE, "")

# 두 번째 PUB 소켓 (new_sensor.py)
socket2 = context.socket(zmq.SUB)
socket2.connect("tcp://localhost:5566")
socket2.setsockopt_string(zmq.SUBSCRIBE, "")

# ZMQ Poller로 두 소켓 모두 감시
poller = zmq.Poller()
poller.register(socket1, zmq.POLLIN)
poller.register(socket2, zmq.POLLIN)

def display():
    print("\n🅿️ 실시간 주차장 상태")
    for zone, status in zones.items():
        state = "🟢 비어있음" if not status else "🔴 주차중"
        print(f"{zone}: {state}")
    print("-" * 30)

print("[🛰️ 서버 실행 중] 여러 센서로부터 메시지 수신 대기 중...")

while True:
    socks = dict(poller.poll())
    for sock in [socket1, socket2]:
        if sock in socks:
            msg = sock.recv_string()
            try:
                action, zone = msg.strip().split()
                if action == "PARKED":
                    zones[zone] = True
                elif action == "LEFT":
                    zones[zone] = False
                display()
            except Exception as e:
                print("⚠️ 메시지 오류:", e)
