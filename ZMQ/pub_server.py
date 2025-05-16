# pub_server.py
import zmq

context = zmq.Context()

# REQ/REP: 센서 클라이언트로부터 받기
rep_socket = context.socket(zmq.REP)
rep_socket.bind("tcp://*:5555")

# PUB/SUB: TUI 클라이언트에게 보내기
pub_socket = context.socket(zmq.PUB)
pub_socket.bind("tcp://*:5556")

print("[서버 실행 중] 센서 수신 + PUB 전송")

while True:
    message = rep_socket.recv_string()
    print("받은 센서 메시지:", message)

    # 응답 전송 (REQ/REP용)
    rep_socket.send_string("서버 수신 완료")

    # 구독자에게 브로드캐스트
    pub_socket.send_string(message)
