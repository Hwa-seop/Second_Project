# pub_server.py
import zmq
import time
import random

context = zmq.Context()
socket = context.socket(zmq.PUB)
socket.bind("tcp://*:5556")

zones = ["A1", "A2", "B1", "B2"]
actions = ["PARKED", "LEFT"]

while True:
    msg = f"{random.choice(actions)} {random.choice(zones)}"
    print(f"전송: {msg}")
    socket.send_string(msg)
    time.sleep(3)  # 3초 간격으로 메시지 발행
