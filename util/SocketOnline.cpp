//
// Created by tangdingyi on 2019/12/25.
//

#include "SocketOnline.h"

SocketOnline::SocketOnline() {}

SocketOnline::~SocketOnline() {
    delete buffer;
    delete header;
}

SocketOnline::SocketOnline(int id, SOCK sock) {
    this->id = id;
    this->sock = sock;
    buffer = new char[BUFFER_MAX];
    header = new char[max(HEADER_LEN, HEADER_LEN_OPT) + 1];
    send_num = 0;
    recv_num = 0;
    init();
}

SocketOnline& SocketOnline::operator=(const SocketOnline &u) {
    id = u.id;
    sock = u.sock;
    buffer = new char[BUFFER_MAX];
    header = new char[max(HEADER_LEN, HEADER_LEN_OPT) + 1];
    send_num = 0;
    recv_num = 0;
}

void SocketOnline::init(int id, SOCK sock) {
    this->id = id;
    this->sock = sock;
    buffer = new char[BUFFER_MAX];
    header = new char[HEADER_LEN + 1];
    send_num = 0;
    recv_num = 0;
#ifdef UNIX_PLATFORM
    int flag, ret_flag, ret_sol;
        flag = 1024*1024*1024;
//        ret_sol = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void*)&flag, sizeof(flag));
        ret_flag = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));
        DBGprint("ret sol: %d flag: %d\n", ret_sol, ret_flag);
#else
    int flag, ret_sol, ret_flag;
    flag = 1;
//    ret_sol = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (const char*)&flag, sizeof(flag));
        ret_flag = setsockopt(sock, SOL_SOCKET, TCP_NODELAY, (const char*)&flag, sizeof(flag));
    DBGprint("ret sol: %d flag: %d\n", ret_sol, ret_flag);
#endif
}

void SocketOnline::init() {
#ifdef UNIX_PLATFORM
    int flag, ret_flag, ret_sol;
        flag = 1024*1024*1024;
//        ret_sol = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void*)&flag, sizeof(flag));
        ret_flag = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));
        DBGprint("ret sol: %d flag: %d\n", ret_sol, ret_flag);
#else
    int flag, ret_sol, ret_flag;
    flag = 1;
//        ret_sol = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (const char*)&flag, sizeof(flag));
    ret_flag = setsockopt(sock, SOL_SOCKET, TCP_NODELAY, (const char*)&flag, sizeof(flag));
    DBGprint("ret sol: %d flag: %d\n", ret_sol, ret_flag);
#endif
}

void SocketOnline::reset() {
    send_num = 0;
    recv_num = 0;
}

int SocketOnline::send_message(SOCK sock, char *p, int l) {
    int ret;
#ifdef UNIX_PLATFORM
    ret = write(sock, p, l);
#else
    ret = send(sock, p, l, NULL);
#endif
    return ret;
}

int SocketOnline::send_message_n(SOCK sock, char *p, int l) {
    int tot = 0, cur = 0;
//    cout<<"send l: "<<l<<endl;
    while (tot < l) {
#ifdef UNIX_PLATFORM
        cur = write(sock, p, l-tot);
#else
        cur = send(sock, p, l - tot, NULL);
#endif
        p += cur;
        tot += cur;
    }
    send_num+=l;
    return tot;
}

int SocketOnline::recv_message(SOCK sock, char *p, int l) {
    int ret;
#ifdef UNIX_PLATFORM
    ret = read(sock, p, l);
#else
    ret = recv(sock, p, l, NULL);
#endif
    return ret;
}

int SocketOnline::recv_message_n(SOCK sock, char *p, int l) {
    int tot = 0, cur = 0;
//    cout<<"recv l: "<<l<<endl;
    while (tot < l) {
#ifdef UNIX_PLATFORM
        cur = read(sock, p, l-tot);
#else
        cur = recv(sock, p, l - tot, NULL);
#endif
        p += cur;
        tot += cur;
    }
    recv_num+=l;
    return tot;
}

void SocketOnline::send_message(const Mat &a) {
    int len_buffer;
    len_buffer = a.toString_pos(buffer);
    Constant::Util::int_to_header(header, len_buffer);
    send_message_n(sock, header, HEADER_LEN);
    send_message_n(sock, buffer, len_buffer);
}

void SocketOnline::send_message(Mat *a) {
    int len_buffer;
    len_buffer = a->toString_pos(buffer);
//    header = new char[max(HEADER_LEN, HEADER_LEN_OPT) + 1];
//    Constant::Util::int_to_header(header, a->rows());
//    Constant::Util::int_to_header(header + 4, a->cols());
    Constant::Util::int_to_header(header, len_buffer);
    send_message_n(sock, header, HEADER_LEN);
//    send_message_n(sock, header, HEADER_LEN_OPT);
    send_message_n(sock, buffer, len_buffer);
}

void SocketOnline::send_message(int b) {
    char* p = buffer;
    Constant::Util::int_to_char(p, b);
    send_message_n(sock, buffer, 4);
}

Mat SocketOnline::recv_message() {
    Mat ret;
    int len_header = recv_message_n(sock, header, HEADER_LEN);
    int l = Constant::Util::header_to_int(header);
    int len_buffer = recv_message_n(sock, buffer, Constant::Util::header_to_int(header));
    char* p = buffer;
    ret.getFrom_pos(p);
    return ret;
}

void SocketOnline::recv_message(Mat *a) {
    recv_message_n(sock, header, HEADER_LEN);
    recv_message_n(sock, buffer, a->getStringLen());
    char* p = buffer;
    a->addFrom_pos(p);
//    cout << "--------------" << endl;
}

void SocketOnline::recv_message(Mat &a) {
    // add this to receive share, added by curious 2020.6.3
    recv_message_n(sock, header, HEADER_LEN);
//    int r = Constant::Util::char_to_int(header);
//    int c = Constant::Util::char_to_int(header);
    recv_message_n(sock, buffer, Constant::Util::header_to_int(header));
//    recv_message_n(sock, header, HEADER_LEN_OPT);
//    recv_message_n(sock, buffer, a.getStringLen());
    char* p = buffer;
    a.getFrom_pos(p);
}

int SocketOnline::recv_message_int() {
    recv_message_n(sock, buffer, 4);
    char* p = buffer;
    int ret = Constant::Util::char_to_int(p);
    return ret;
}

void SocketOnline::push(const Mat &a) {}

void SocketOnline::print() {
    DBGprint("socket online\n");
}