import zmq
context = zmq.Context()
socket = context.socket(zmq.REP) #응답(rep)소켓
socket.bind("tcp://*:5555") # 모든 인터페이스의 5555번 포트에 바인딩

print("[서버 실행 중] 기다리는 중..")

while True:
    message = socket.recv_string() #C 클라이언트로부터 문자열 수신
    print("받은 메시지: ", message)
    
    response = "ACK: 메시지 수신 완료"
    socket.send_string("response")