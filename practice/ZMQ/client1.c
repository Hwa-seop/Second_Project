#include <zmq.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ); //요청(rep) 소켓
    zmq_connect(requester, "tcp://localhost:5555"); //python 서버에 연결

    char buffer[] = "안녕 나는 첫번째 클라이언트야~~";
    zmq_send(requester, buffer, strlen(buffer), 0); //메세지 전송
    zmq_send(requester, "센서값: 40", 12, 0);
    zmq_recv(requester, buffer, 127, 0);
    
    char buffer[128]={0};
    zmq_recv(requester, buffer, 127, 0);  // 응답 수신
    printf("서버 응답: 안녕 클라이언트1~ %s\n", buffer);

    zmq_close(requester);
    zmq_ctx_destroy(context);
    return 0;
}