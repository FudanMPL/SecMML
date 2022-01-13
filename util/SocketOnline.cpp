//
// Created by tangdingyi on 2019/12/25.
//

#include "SocketOnline.h"

// non-parameter Constructor Function

SocketOnline::SocketOnline() {}

// Destructor Function

SocketOnline::~SocketOnline()
{
    delete buffer;
    delete header;
}

// Parameterized Constructor

SocketOnline::SocketOnline(int id, SOCK sock)
{
    this->id = id;
    this->sock = sock;
    buffer = new char[Config::config->BUFFER_MAX];
    header = new char[max(Config::config->HEADER_LEN, Config::config->HEADER_LEN_OPT) + 1];
    send_num = 0;
    recv_num = 0;
    init();
}

// Overload Operator '='

SocketOnline &SocketOnline::operator=(const SocketOnline &u)
{
    id = u.id;
    sock = u.sock;
    buffer = new char[Config::config->BUFFER_MAX];
    header = new char[max(Config::config->HEADER_LEN, Config::config->HEADER_LEN_OPT) + 1];
    send_num = 0;
    recv_num = 0;
}

// Initialization Function (call setsockopt function to set socket)

void SocketOnline::init(int id, SOCK sock)
{
    this->id = id;
    this->sock = sock;
    buffer = new char[Config::config->BUFFER_MAX];
    header = new char[Config::config->HEADER_LEN + 1];
    send_num = 0;
    recv_num = 0;
#ifdef UNIX_PLATFORM
    int flag, ret_flag, ret_sol;
    flag = 1024 * 1024 * 1024;
    //        ret_sol = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void*)&flag, sizeof(flag));
    ret_flag = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));
    DBGprint("ret sol: %d flag: %d\n", ret_sol, ret_flag);
#else
    int flag, ret_sol, ret_flag;
    flag = 1;
    //    ret_sol = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (const char*)&flag, sizeof(flag));
    ret_flag = setsockopt(sock, SOL_SOCKET, TCP_NODELAY, (const char *)&flag, sizeof(flag));
    DBGprint("ret sol: %d flag: %d\n", ret_sol, ret_flag);
#endif
}

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
    DBGprint("ret sol: %d flag: %d\n", ret_sol, ret_flag);
#endif
}

// Function reset

void SocketOnline::reset()
{
    send_num = 0;
    recv_num = 0;
}

// call function write to write l bytes from *p to sock

int SocketOnline::send_message(SOCK sock, char *p, int l)
{
    int ret;
#ifdef UNIX_PLATFORM
    ret = write(sock, p, l);
#else
    ret = send(sock, p, l, NULL);
#endif
    return ret;
}

// call function write to write l bytes from *p to sock(use while to avoid l > max(bytes the function write can write))

int SocketOnline::send_message_n(SOCK sock, char *p, int l)
{
    int tot = 0, cur = 0;
    //    cout<<"send l: "<<l<<endl;
    while (tot < l)
    {
#ifdef UNIX_PLATFORM
        cur = write(sock, p, l - tot);
#else
        cur = send(sock, p, l - tot, NULL);
#endif
        p += cur;
        tot += cur;
    }
    send_num += l;
    return tot;
}

// call function read to read l bytes from *p to sock

int SocketOnline::recv_message(SOCK sock, char *p, int l)
{
    int ret;
#ifdef UNIX_PLATFORM
    ret = read(sock, p, l);
#else
    ret = recv(sock, p, l, NULL);
#endif
    return ret;
}

// call function write to write l bytes from *p to sock(use while to avoid l > max(bytes the function write can write))

int SocketOnline::recv_message_n(SOCK sock, char *p, int l)
{
    int tot = 0, cur = 0;
    //    cout<<"recv l: "<<l<<endl;
    while (tot < l)
    {
#ifdef UNIX_PLATFORM
        cur = read(sock, p, l - tot);
#else
        cur = recv(sock, p, l - tot, NULL);
#endif
        p += cur;
        tot += cur;
    }
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
    Constant::Util::int_to_char(p, b);
    send_message_n(sock, buffer, 4);
}

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
    int ret = Constant::Util::char_to_int(p);
    return ret;
}

void SocketOnline::push(const Mat &a) {}

void SocketOnline::print()
{
    DBGprint("socket online\n");
}