//
// Created by tangdingyi on 2019/12/25.
//

#include "SocketManager.h"

<<<<<<< HEAD
SocketOnline *socket_io[M][M];
=======
vector<vector<SocketOnline*>> SocketManager::socket_io;

// init function in windows
>>>>>>> dev

void SocketManager::init_windows_socket() {
#ifdef UNIX_PLATFORM
#else
    WSADATA wsaData;
    WSAStartup( MAKEWORD(2, 2), &wsaData);
#endif
}

<<<<<<< HEAD
=======
// exit function in windows

>>>>>>> dev
void SocketManager::exit_windows_socket() {
#ifdef UNIX_PLATFORM
#else
    WSACleanup();
#endif
}

<<<<<<< HEAD
=======
// Initialize server side

>>>>>>> dev
void SocketManager::server_init(SOCK &serv_sock, string ip, int port) {
#ifdef UNIX_PLATFORM
    serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());  //具体的IP地址
    serv_addr.sin_port = htons(port);  //端口

    #ifdef MAC_OS
        ::bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    #else
        bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    #endif
    listen(serv_sock, 20);
#else
    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());  //具体的IP地址
    serv_addr.sin_port = htons(port);  //端口

    bind(serv_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr));

    listen(serv_sock, 20);
#endif
}

<<<<<<< HEAD
=======
// Initialize client side

>>>>>>> dev
void SocketManager::client_init(SOCK &sock, string ip, int port) {
#ifdef UNIX_PLATFORM
    sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());  //具体的IP地址
    serv_addr.sin_port = htons(port);  //端口

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
#else
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());  //具体的IP地址
    serv_addr.sin_port = htons(port);  //端口

    connect(sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr));
#endif
}

<<<<<<< HEAD
=======
// close the socket

>>>>>>> dev
void SocketManager::socket_close(SOCK sock) {
#ifdef UNIX_PLATFORM
    close(sock);
#else
    closesocket(sock);
#endif
}

<<<<<<< HEAD
=======
// call function accept  to accpet serv_sock

>>>>>>> dev
SOCK SocketManager::accept_sock(SOCK serv_sock) {
    SOCK ret;
#ifdef UNIX_PLATFORM
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    ret = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
#else
    SOCKADDR clnt_addr;
    int nSize = sizeof(SOCKADDR);
    ret = accept(serv_sock, (SOCKADDR*)&clnt_addr, &nSize);
#endif
    DBGprint("accept\n");
    return ret;
}

<<<<<<< HEAD
void SocketManager::print_socket() {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
=======
// print the connected socket

void SocketManager::print_socket() {
    for (int i = 0; i < Config::config->M; i++) {
        for (int j = 0; j < Config::config->M; j++) {
>>>>>>> dev
            if (i != j) {
                DBGprint("socket %d %d:\n");
                socket_io[i][j]->print();
            }
        }
    }
}

<<<<<<< HEAD
void SocketManager::print() {
    ll tot_send = 0, tot_recv = 0;
    for (int i = 0; i < M; i++) {
=======
// print the number of total bytes sent and receivd by this socket

void SocketManager::print() {
    ll tot_send = 0, tot_recv = 0;
    for (int i = 0; i < Config::config->M; i++) {
>>>>>>> dev
        if (node_type != i) {
            tot_send += socket_io[node_type][i]->send_num;
            tot_recv += socket_io[node_type][i]->recv_num;
        }
    }
    DBGprint("tot_send: %lld tot_recv: %lld\n", tot_send, tot_recv);
}

<<<<<<< HEAD
SocketManager::SMMLF::SMMLF() {}

void SocketManager::SMMLF::init() {
    init(new string[3]{"127.0.0.1","127.0.0.1","127.0.0.1"}, new int[3]{1234,1235,1236});
}

void SocketManager::SMMLF::init(string* ip, int* port) {
    this->ip = ip;
    this->port = port;
    init_windows_socket();
    server();
    client();
}

void SocketManager::SMMLF::server() {
    server_init(serv_sock, *(ip+node_type), *(port + node_type));
    char buffer[101];
    for (int i = node_type + 1; i < M; i++) {
=======
// non-parameters Constructor Function

SocketManager::SMMLF::SMMLF() {}

// non-parameters Function init(default ip and port)

void SocketManager::SMMLF::init() {
    init({"10.176.34.170","10.176.34.170","10.176.34.170"}, {1234,1235,1236});
}

// Function init(get ip and port)

void SocketManager::SMMLF::init(const vector<string> &ip, const vector<int> &port) {
    DBGprint("ip,port init begins\n");
    this->ip = ip;
    this->port = port;
    
    init_windows_socket();
    server();
    client();
    DBGprint("ip,port init ends\n");
}

// construct server side and socket_io[node_type][M-1],[node_type][M-2]……[node_type][node_type+1]

void SocketManager::SMMLF::server() {
    clnt_sock = new SOCK[Config::config->M];
    socket_io.resize(Config::config->M);
    for(int i = 0; i < Config::config->M; i++)
    {
        socket_io[i].resize(Config::config->M);
    }
    server_init(serv_sock, ip[node_type], port[node_type]);
    char buffer[101];
    for (int i = node_type + 1; i < Config::config->M; i++) {
>>>>>>> dev
        sock = accept_sock(serv_sock);
#ifdef UNIX_PLATFORM
        read(sock, buffer, 1);
#else
        recv(sock, buffer, 1, NULL);
#endif
        printf("Message form client %d: %c\n", buffer[0]-'0', buffer[0]);
        socket_io[node_type][buffer[0]-'0'] = new SocketOnline(buffer[0]-'0', sock);
        clnt_sock[buffer[0]-'0'] = sock;
    }
}

<<<<<<< HEAD
void SocketManager::SMMLF::client() {
//    printf(" asdasd %d\n", port);
    for (int i = 0; i < node_type; i++) {
        client_init(sock, *(ip+i), *(port + i));
=======
// construct client side and socket_io[node_type][0],[node_type][1]……[node_type][node_type-1]

void SocketManager::SMMLF::client() {
//    printf(" asdasd %d\n", port);
    for (int i = 0; i < node_type; i++) {
        client_init(sock, ip[i], port[i]);
>>>>>>> dev
//        printf("client %d connect to server %d\n", node_type, i);
//        printf(" port %d\n", port);

        char str[] = "0";
        str[0] += node_type;
//        printf("str: %s %d\n", str, strlen(str));
#ifdef UNIX_PLATFORM
        signal(SIGPIPE, SIG_DFL);
        write(sock, str, strlen(str));
#else
        send(sock, str, strlen(str), NULL);
#endif
        socket_io[node_type][i] = new SocketOnline(i, sock);
        clnt_sock[i] = sock;
    }
}

<<<<<<< HEAD
=======
// close server side

>>>>>>> dev
void SocketManager::SMMLF::server_exit() {
    socket_close(serv_sock);
    exit_windows_socket();
}

<<<<<<< HEAD
void SocketManager::SMMLF::client_exit() {
    for (int i = 0; i < M; i++) {
=======
// close client side

void SocketManager::SMMLF::client_exit() {
    for (int i = 0; i < Config::config->M; i++) {
>>>>>>> dev
        if (i != node_type) {
            socket_close(clnt_sock[i]);
        }
    }
    exit_windows_socket();
}

<<<<<<< HEAD
void SocketManager::SMMLF::exit_all() {
    server_exit();
    client_exit();
=======
// call server_exit() and client_exit()

void SocketManager::SMMLF::exit_all() {
    server_exit();
    client_exit();
}

// Pack socket_io(send message)

void SocketManager::send(int node_type, int target, Mat *a)
{
    socket_io[node_type][target]->send_message(a);
}

void SocketManager::send(int node_type, int target, Mat &a)
{
    socket_io[node_type][target]->send_message(a);
}

// Pack socket_io(receive message)

void SocketManager::receive(int node_type, int from, Mat *a)
{
    socket_io[node_type][from]->recv_message(a);
}

void SocketManager::receive(int node_type, int from, Mat &a)
{
    socket_io[node_type][from]->recv_message(a);
}

Mat SocketManager::receive(int node_type, int from)
{
    return socket_io[node_type][from]->recv_message();
>>>>>>> dev
}