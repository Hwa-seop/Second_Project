# sensor_input.py
import zmq

# 주차 구역 목록
zones = ["A1", "A2", "B1", "B2"]
occupied_zones = set()

# ZMQ PUB 설정
context = zmq.Context()
socket = context.socket(zmq.PUB)
socket.bind("tcp://*:5555")

def display_available_zones():
    available = [z for z in zones if z not in occupied_zones]
    print("\n🅿️ 현재 빈 주차 구역:", ", ".join(available) if available else "없음")

print("🚗 주차장 시뮬레이터 실행")
print("▶ 구역 이름 입력 시 주차 (예: A1)")
print("▶ 'q 구역명' 입력 시 출차 (예: q A1)")

while True:
    display_available_zones()
    user_input = input("입력: ").strip().upper()

    # 출차 명령
    if user_input.startswith("Q "):
        _, zone = user_input.split(maxsplit=1)
        if zone not in zones:
            print("❗존재하지 않는 구역입니다.")
        elif zone not in occupied_zones:
            print(f"⚠️ {zone} 구역은 이미 비어 있습니다.")
        else:
            socket.send_string(f"LEFT {zone}")
            occupied_zones.remove(zone)
            print(f"⬅️ {zone} 구역에서 출차 완료")

    # 주차 명령
    elif user_input in zones:
        if user_input in occupied_zones:
            print(f"⚠️ {user_input} 구역은 이미 주차 중입니다.")
        else:
            socket.send_string(f"PARKED {user_input}")
            occupied_zones.add(user_input)
            print(f"✅ {user_input} 구역에 주차 완료")

    else:
        print("❗올바른 입력 형식: 'A1', 'q A1'")

