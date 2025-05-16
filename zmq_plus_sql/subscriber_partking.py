# subscriber_parking.py
import zmq
import sqlite3

# ZMQ SUB 연결
context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.connect("tcp://localhost:5556")
socket.setsockopt_string(zmq.SUBSCRIBE, "")  # 모든 메시지 수신

# SQLite 연결
conn = sqlite3.connect("parking.db")
cur = conn.cursor()

print("[수신자 시작] 주차 상태 수신 및 DB 기록 중...")

while True:
    msg = socket.recv_string()  # 예: "PARKED A1" 또는 "LEFT A2"
    print("▶ 받은 메시지:", msg)

    try:
        action, zone = msg.split()
        cur.execute("INSERT INTO parking_log (zone, action) VALUES (?, ?)", (zone, action))
        conn.commit()
    except Exception as e:
        print("⚠️ 메시지 파싱 오류:", e)
