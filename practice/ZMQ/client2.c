#include <zmq.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ); //요청(rep) 소켓
    zmq_connect(requester, "tcp://localhost:5555"); //python 서버에 연결

    char msg[] = "안녕 나는 두번째 클라이언트야~~";
    zmq_send(requester, msg, strlen(msg), 0); //메세지 전송


    // 응답 수신    
    for (int i = 0; i < 5; i++) {
        char msg[128];
        sprintf(msg, "센서값: %d", i * 10);
        zmq_send(requester, msg, strlen(msg), 0);
    

        zmq_recv(requester, msg, 127, 0);
        printf("응답: %s\n", msg);
        sleep(1);
    }
    

    zmq_close(requester);
    zmq_ctx_destroy(context);
    return 0;
}