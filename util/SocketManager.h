//
// Created by tangdingyi on 2019/12/25.
//

#ifndef MPC_ML_SOCKETMANAGER_H
#define MPC_ML_SOCKETMANAGER_H

#include "../Constant.h"
#include "SocketOnline.h"

extern int node_type;

class SocketManager {
public:
    static void init_windows_socket();
    static void exit_windows_socket();
    static void server_init(SOCK &sock, string ip, int port);
    static void client_init(SOCK &sock, string ip, int port);
    static void socket_close(SOCK sock);
    static SOCK accept_sock(SOCK sock);
    static void print_socket();
    static void print();
    class SMMLF {
        string ip;
        int port;
        SOCK serv_sock;
        SOCK clnt_sock[M];
        SOCK sock;
        int epoch;
    public:
        SMMLF();
        void init();
        void init(string ip, int port);
        void server();
        void client();
        void server_exit();
        void client_exit();
        void exit_all();
    };
};


#endif //MPC_ML_SOCKETMANAGER_H
