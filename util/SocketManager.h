<<<<<<< HEAD
=======
/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-15 20:59:40
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2021-12-16 21:51:05
 */
>>>>>>> dev
//
// Created by tangdingyi on 2019/12/25.
//

#ifndef MPC_ML_SOCKETMANAGER_H
#define MPC_ML_SOCKETMANAGER_H

#include "../Constant.h"
#include "SocketOnline.h"
<<<<<<< HEAD
=======
#include "vector"
>>>>>>> dev

// extern int node_type;

class SocketManager {
public:
<<<<<<< HEAD
=======
    static vector<vector<SocketOnline*>> socket_io;
>>>>>>> dev
    static void init_windows_socket();
    static void exit_windows_socket();
    static void server_init(SOCK &sock, string ip, int port);
    static void client_init(SOCK &sock, string ip, int port);
    static void socket_close(SOCK sock);
    static SOCK accept_sock(SOCK sock);
    static void print_socket();
    static void print();
<<<<<<< HEAD
    class SMMLF {
        string* ip;
        int* port;
        SOCK serv_sock;
        SOCK clnt_sock[M];
=======
    static void send(int node_type, int targrt, Mat *a);
    static void send(int node_type, int target, Mat &a);
    static void receive(int node_type, int from, Mat *a);
    static void receive(int node_type, int from, Mat &a);
    static Mat receive(int node_type, int from);
    class SMMLF {
        vector<string> ip;
        vector<int> port;
        SOCK serv_sock;
        SOCK *clnt_sock;
>>>>>>> dev
        SOCK sock;
        int epoch;
    public:
        SMMLF();
        void init();
<<<<<<< HEAD
        void init(string* ip, int* port);
=======
        void init(const vector<string>& ip, const vector<int>& port);
>>>>>>> dev
        void server();
        void client();
        void server_exit();
        void client_exit();
        void exit_all();
    };
};


#endif //MPC_ML_SOCKETMANAGER_H
