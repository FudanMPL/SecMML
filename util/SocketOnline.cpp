//
// Created by tangdingyi on 2019/12/25.
//

#include "SocketOnline.h"

<<<<<<< HEAD
SocketOnline::SocketOnline() {}

SocketOnline::~SocketOnline() {
=======
// non-parameter Constructor Function

SocketOnline::SocketOnline() {}

// Destructor Function

SocketOnline::~SocketOnline()
{
>>>>>>> dev
    delete buffer;
    delete header;
}

<<<<<<< HEAD
SocketOnline::SocketOnline(int id, SOCK sock) {
    this->id = id;
    this->sock = sock;
    buffer = new char[BUFFER_MAX];
    header = new char[max(HEADER_LEN, HEADER_LEN_OPT) + 1];
=======
// Parameterized Constructor

SocketOnline::SocketOnline(int id, SOCK sock)
{
    this->id = id;
    this->sock = sock;
    buffer = new char[Config::config->BUFFER_MAX];
    header = new char[max(Config::config->HEADER_LEN, Config::config->HEADER_LEN_OPT) + 1];
>>>>>>> dev
    send_num = 0;
    recv_num = 0;
    init();
}

<<<<<<< HEAD
SocketOnline& SocketOnline::operator=(const SocketOnline &u) {
    id = u.id;
    sock = u.sock;
    buffer = new char[BUFFER_MAX];
    header = new char[max(HEADER_LEN, HEADER_LEN_OPT) + 1];
=======
// Overload Operator '='

SocketOnline &SocketOnline::operator=(const SocketOnline &u)
{
    id = u.id;
    sock = u.sock;
    buffer = new char[Config::config->BUFFER_MAX];
    header = new char[max(Config::config->HEADER_LEN, Config::config->HEADER_LEN_OPT) + 1];
>>>>>>> dev
    send_num = 0;
    recv_num = 0;
}

<<<<<<< HEAD
void SocketOnline::init(int id, SOCK sock) {
    this->id = id;
    this->sock = sock;
    buffer = new char[BUFFER_MAX];
    header = new char[HEADER_LEN + 1];
=======
// Initialization Function (call setsockopt function to set socket)

void SocketOnline::init(int id, SOCK sock)
{
    this->id = id;
    this->sock = sock;
    buffer = new char[Config::config->BUFFER_MAX];
    header = new char[Config::config->HEADER_LEN + 1];
>>>>>>> dev
    send_num = 0;
    recv_num = 0;
#ifdef UNIX_PLATFORM
    int flag, ret_flag, ret_sol;
<<<<<<< HEAD
        flag = 1024*1024*1024;
//        ret_sol = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void*)&flag, sizeof(flag));
        ret_flag = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));
        DBGprint("ret sol: %d flag: %d\n", ret_sol, ret_flag);
#else
    int flag, ret_sol, ret_flag;
    flag = 1;
//    ret_sol = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (const char*)&flag, sizeof(flag));
        ret_flag = setsockopt(sock, SOL_SOCKET, TCP_NODELAY, (const char*)&flag, sizeof(flag));
=======
    flag = 1024 * 1024 * 1024;
    //        ret_sol = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void*)&flag, sizeof(flag));
    ret_flag = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));
    DBGprint("ret sol: %d flag: %d\n", ret_sol, ret_flag);
#else
    int flag, ret_sol, ret_flag;
    flag = 1;
    //    ret_sol = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (const char*)&flag, sizeof(flag));
    ret_flag = setsockopt(sock, SOL_SOCKET, TCP_NODELAY, (const char *)&flag, sizeof(flag));
>>>>>>> dev
    DBGprint("ret sol: %d flag: %d\n", ret_sol, ret_flag);
#endif
}

<<<<<<< HEAD
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
=======
// non-parameters Initialization Function

void SocketOnline::init()
{
#ifdef UNIX_PLATFORM
    int flag, ret_flag, ret_sol;
    flag = 1024 * 1024 * 1024;
    //        ret_sol = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void*)&flag, sizeof(flag));
    ret_flag = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));
    DBGprint("ret sol: %d flag: %d\n", ret_sol, ret_flag);
#else
    int flag, ret_sol, ret_flag;
    flag = 1;
    //        ret_sol = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (const char*)&flag, sizeof(flag));
    ret_flag = setsockopt(sock, SOL_SOCKET, TCP_NODELAY, (const char *)&flag, sizeof(flag));
>>>>>>> dev
    DBGprint("ret sol: %d flag: %d\n", ret_sol, ret_flag);
#endif
}

<<<<<<< HEAD
void SocketOnline::reset() {
=======
// Function reset

void SocketOnline::reset()
{
>>>>>>> dev
    send_num = 0;
    recv_num = 0;
}

<<<<<<< HEAD
int SocketOnline::send_message(SOCK sock, char *p, int l) {
=======
// call function write to write l bytes from *p to sock

int SocketOnline::send_message(SOCK sock, char *p, int l)
{
>>>>>>> dev
    int ret;
#ifdef UNIX_PLATFORM
    ret = write(sock, p, l);
#else
    ret = send(sock, p, l, NULL);
#endif
    return ret;
}

<<<<<<< HEAD
int SocketOnline::send_message_n(SOCK sock, char *p, int l) {
    int tot = 0, cur = 0;
//    cout<<"send l: "<<l<<endl;
    while (tot < l) {
#ifdef UNIX_PLATFORM
        cur = write(sock, p, l-tot);
=======
// call function write to write l bytes from *p to sock(use while to avoid l > max(bytes the function write can write))

int SocketOnline::send_message_n(SOCK sock, char *p, int l)
{
    int tot = 0, cur = 0;
    //    cout<<"send l: "<<l<<endl;
    while (tot < l)
    {
#ifdef UNIX_PLATFORM
        cur = write(sock, p, l - tot);
>>>>>>> dev
#else
        cur = send(sock, p, l - tot, NULL);
#endif
        p += cur;
        tot += cur;
    }
<<<<<<< HEAD
    send_num+=l;
    return tot;
}

int SocketOnline::recv_message(SOCK sock, char *p, int l) {
=======
    send_num += l;
    return tot;
}

// call function read to read l bytes from *p to sock

int SocketOnline::recv_message(SOCK sock, char *p, int l)
{
>>>>>>> dev
    int ret;
#ifdef UNIX_PLATFORM
    ret = read(sock, p, l);
#else
    ret = recv(sock, p, l, NULL);
#endif
    return ret;
}

<<<<<<< HEAD
int SocketOnline::recv_message_n(SOCK sock, char *p, int l) {
    int tot = 0, cur = 0;
//    cout<<"recv l: "<<l<<endl;
    while (tot < l) {
#ifdef UNIX_PLATFORM
        cur = read(sock, p, l-tot);
=======
// call function write to write l bytes from *p to sock(use while to avoid l > max(bytes the function write can write))

int SocketOnline::recv_message_n(SOCK sock, char *p, int l)
{
    int tot = 0, cur = 0;
    //    cout<<"recv l: "<<l<<endl;
    while (tot < l)
    {
#ifdef UNIX_PLATFORM
        cur = read(sock, p, l - tot);
>>>>>>> dev
#else
        cur = recv(sock, p, l - tot, NULL);
#endif
        p += cur;
        tot += cur;
    }
<<<<<<< HEAD
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
=======
    recv_num += l;
    return tot;
}

// convert matrix(&a) to char* and send by socket

void SocketOnline::send_message(const Mat &a)
{
    int len_buffer;
    int size = a.size() * 8 + 3 * 4;
    Constant::Util::int_to_header(header, size);
    send_message_n(sock, header, Config::config->HEADER_LEN);
    if (size < Config::config->BUFFER_MAX)
    {
        len_buffer = a.toString_pos(buffer);
        send_message_n(sock, buffer, len_buffer);
    }
    else
    {
        a.toBuffer_pos(buffer);
        int i = (Config::config->BUFFER_MAX - 3 * 4) / 8;

        cout << i << endl;
        send_message_n(sock, buffer, i * 8 + 12);
        while (i + Config::config->BUFFER_MAX / 8 < a.size())
        {
            char *p = buffer;
            a.toBuffer(p, i);
            send_message_n(sock, buffer, (Config::config->BUFFER_MAX / 8) * 8);
            i += Config::config->BUFFER_MAX / 8;
        }
        char *p = buffer;
        a.to_Buffer(p, i);
        send_message_n(sock, buffer, (a.size() - i) * 8);
    }
}

// convert matrix(*a) to char* and send by socket

void SocketOnline::send_message(Mat *a)
{
    int len_buffer;
    int size = a->size() * 8 + 3 * 4;
    Constant::Util::int_to_header(header, size);
    send_message_n(sock, header, Config::config->HEADER_LEN);
    if (size < Config::config->BUFFER_MAX)
    {
        len_buffer = a->toString_pos(buffer);
        send_message_n(sock, buffer, len_buffer);
    }
    else
    {
        a->toBuffer_pos(buffer);
        int i = (Config::config->BUFFER_MAX - 3 * 4) / 8;
        send_message_n(sock, buffer, i * 8 + 12);
        while (i + Config::config->BUFFER_MAX / 8 < a->size())
        {
            char *p = buffer;
            a->toBuffer(p, i);
            send_message_n(sock, buffer, (Config::config->BUFFER_MAX / 8) * 8);
            i += Config::config->BUFFER_MAX / 8;
        }
        char *p = buffer;
        a->to_Buffer(p, i);
        send_message_n(sock, buffer, (a->size() - i) * 8);
    }
}

// convert int to char* and send by socket

void SocketOnline::send_message(int b)
{
    char *p = buffer;
>>>>>>> dev
    Constant::Util::int_to_char(p, b);
    send_message_n(sock, buffer, 4);
}

<<<<<<< HEAD
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
=======
// receive message from socket and convet to matrix, then return

Mat SocketOnline::recv_message()
{
    Mat ret;
    int len_header = recv_message_n(sock, header, Config::config->HEADER_LEN);
    int l = Constant::Util::header_to_int(header);
    if (l < Config::config->BUFFER_MAX)
    {
        int len_buffer = recv_message_n(sock, buffer, Constant::Util::header_to_int(header));
        char *p = buffer;
        ret.getFrom_pos(p);
        return ret;
    }
    else
    {
        int i = (Config::config->BUFFER_MAX - 3 * 4) / 8;
        recv_message_n(sock, buffer, i * 8 + 12);
        char *p = buffer;
        ret.getFrom_buf(p);
        while (i + Config::config->BUFFER_MAX / 8 < ret.size())
        {
            recv_message_n(sock, buffer, (Config::config->BUFFER_MAX / 8) * 8);
            char *p = buffer;
            ret.getBuffer(p, i);
            i += Config::config->BUFFER_MAX / 8;
        }
        recv_message_n(sock, buffer, (ret.size() - i) * 8);
        char *pos = buffer;
        ret.get_Buffer(pos, i);
        return ret;
    }
}

// receive message from socket and convet to matrix, add this matrix to a

void SocketOnline::recv_message(Mat *a)
{
    int len_header = recv_message_n(sock, header, Config::config->HEADER_LEN);
    int l = Constant::Util::header_to_int(header);
    if (l < Config::config->BUFFER_MAX)
    {
        int len_buffer = recv_message_n(sock, buffer, Constant::Util::header_to_int(header));
        char *p = buffer;
        a->addFrom_pos(p);
    }
    else
    {
        int i = (Config::config->BUFFER_MAX - 3 * 4) / 8;
        recv_message_n(sock, buffer, i * 8 + 12);
        char *p = buffer;
        a->addFrom_buf(p);
        while (i + Config::config->BUFFER_MAX / 8 < a->size())
        {
            recv_message_n(sock, buffer, (Config::config->BUFFER_MAX / 8) * 8);
            char *p = buffer;
            a->addBuffer(p, i);
            i += Config::config->BUFFER_MAX / 8;
        }
        recv_message_n(sock, buffer, (a->size() - i) * 8);
        char *pos = buffer;
        a->add_Buffer(pos, i);
    }
}

// receive message from socket and convet to matrix, assign this matrix to a

void SocketOnline::recv_message(Mat &a)
{
    int len_header = recv_message_n(sock, header, Config::config->HEADER_LEN);
    int l = Constant::Util::header_to_int(header);
    if (l < Config::config->BUFFER_MAX)
    {
        int len_buffer = recv_message_n(sock, buffer, Constant::Util::header_to_int(header));
        char *p = buffer;
        a.getFrom_pos(p);
    }
    else
    {
        int i = (Config::config->BUFFER_MAX - 3 * 4) / 8;
        recv_message_n(sock, buffer, i * 8 + 12);
        char *p = buffer;
        a.getFrom_buf(p);
        while (i + Config::config->BUFFER_MAX / 8 < a.size())
        {
            recv_message_n(sock, buffer, (Config::config->BUFFER_MAX / 8) * 8);
            char *p = buffer;
            a.getBuffer(p, i);
            i += Config::config->BUFFER_MAX / 8;
        }
        recv_message_n(sock, buffer, (a.size() - i) * 8);
        char *pos = buffer;
        a.get_Buffer(pos, i);
    }
}

// receive messgage from socket and convert to int, then return

int SocketOnline::recv_message_int()
{
    recv_message_n(sock, buffer, 4);
    char *p = buffer;
>>>>>>> dev
    int ret = Constant::Util::char_to_int(p);
    return ret;
}

void SocketOnline::push(const Mat &a) {}

<<<<<<< HEAD
void SocketOnline::print() {
=======
void SocketOnline::print()
{
>>>>>>> dev
    DBGprint("socket online\n");
}