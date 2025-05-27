# new_sensor.py
import zmq

# 다른 주차 구역
zones = ["C1", "C2", "D1", "D2"]
occupied_zones = set()

# PUB 소켓 생성 (다른 포트 사용)
context = zmq.Context()
socket = context.socket(zmq.PUB)
socket.bind("tcp://*:5566")

print("🚘 보조 센서 시뮬레이터 실행 (C1~D2)")
print("▶ 구역 이름 입력 시 주차 (예: C1)")
print("▶ 'q 구역명' 입력 시 출차 (예: q C1)")

def display_available_zones():
    available = [z for z in zones if z not in occupied_zones]
    print("\n🅿️ 현재 빈 주차 구역:", ", ".join(available) if available else "없음")

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
        print("❗올바른 입력 형식: 'C1', 'q C1'")
