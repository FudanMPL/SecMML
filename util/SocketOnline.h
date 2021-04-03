//
// Created by tangdingyi on 2019/12/25.
//

#ifndef MPC_ML_SOCKETONLINE_H
#define MPC_ML_SOCKETONLINE_H

#include "../Constant.h"
#include "../core/Mat.h"

#ifdef UNIX_PLATFORM
typedef int SOCK;
#else
typedef SOCKET SOCK;
#endif

class SocketOnline {
public:
    int id;
    SOCK sock;
    char* buffer;
    char* header;
    ll send_num;
    ll recv_num;
    SocketOnline();
    ~SocketOnline();
    SocketOnline(int id, SOCK sock);
    SocketOnline& operator=(const SocketOnline &u);
    void init(int id, SOCK sock);
    void init();
    void reset();
    int send_message(SOCK sock, char* p, int l);
    int send_message_n(SOCK sock, char* p, int l);
    int recv_message(SOCK sock, char* p, int l);
    int recv_message_n(SOCK sock, char* p, int l);
    void send_message(const Mat &a);
    void send_message(Mat *a);
    void send_message(int b);
    Mat recv_message();
    void recv_message(Mat *a);
    void recv_message(Mat &a);
    int recv_message_int();
    void push(const Mat &a);
    void print();
};


#endif //MPC_ML_SOCKETONLINE_H
