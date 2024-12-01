#include <iostream>
#include <string>
#include <inttypes.h>

#include <chrono>


#ifdef _WIN32
#define  _WINSOCK_DEPRECATED_NO_WARNINGS 1
#include <winsock2.h>
#else
#include <arpa/inet.h> // htons, inet_addr
#include <netinet/in.h> // sockaddr_in
#include <sys/types.h> // uint16_t
#include <sys/socket.h> // socket, sendto
#include <unistd.h> // close
#endif

constexpr std::string SEP() { return ","; }

int initLibSockets()
{
#ifdef _WIN32
    // Initialise Winsock DLL
    // See https://beej.us/guide/bgnet/html/#windows 
    WSADATA wsaData;
    // MAKEWORD(1,1) for Winsock 1.1, MAKEWORD(2,0) for Winsock 2.0
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        exit(1);
    }
    atexit([]() { WSACleanup(); });
#endif
    return 1;
}

static std::string server_hostname{ "192.168.0.143" };
static uint16_t port = 9000;

std::string get_hostname()
{
    char buffer[256]{};
    if (gethostname(buffer, sizeof(buffer)) == 0)
        return buffer;
    return "N/A";
}

std::string get_username()
{
    char buffer[256]{};
#ifdef _WIN32
    DWORD len = 256;
    if (GetUserNameA(buffer, &len))
        return buffer;
#else
    if (getlogin_r(buffer, sizeof(buffer)) == 0)
        return buffer;
#endif
    return "N/A";
}

std::string get_utc_time()
{
    const auto now = std::chrono::system_clock::now();
    //auto zt_local = std::chrono::zoned_time{ std::chrono::current_zone(), now };
    std::ostringstream oss;
    oss << now; // zt_local;
    return oss.str();
}

std::string get_msg()
{
    static std::string static_info = get_hostname() + SEP() + get_username() + SEP();
    return static_info + get_utc_time() + SEP();
}

int send_msg(const std::string& context)
{
    static int initLib = initLibSockets();
    std::string msg = get_msg() + context;
    auto sock = ::socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in destination;
    destination.sin_family = AF_INET;
    destination.sin_port = htons(port);
    destination.sin_addr.s_addr = inet_addr(server_hostname.c_str());
    int n_bytes = ::sendto(sock, msg.c_str(), (int)msg.length(), 0, reinterpret_cast<sockaddr*>(&destination), sizeof(destination));
    std::cout << "Message: " << msg << std::endl;
    std::cout << n_bytes << " bytes sent" << std::endl;

#ifdef _WIN32
    ::closesocket(sock);
#else
    ::close(sock);
#endif
    return n_bytes;
}

void test_from1()
{
    static int canary = send_msg(__FUNCTION__);
   // std::cout << "in " << __FUNCTION__ << std::endl;
}

void test_from2()
{
    static int canary = send_msg(__FUNCTION__);
   // std::cout << "in " << __FUNCTION__ << std::endl;
}

void test_from3()
{
    static int canary = send_msg(__FUNCTION__);
    //std::cout << "in " << __FUNCTION__ << std::endl;
}

void test_from4()
{
    static int canary = send_msg(__FUNCTION__);
    //std::cout << "in " << __FUNCTION__ << std::endl;
}

int main(int argc, char** argv)
{
    if (argc >= 2)
        server_hostname = argv[1];

    std::cout << "== Wil send message to: " << server_hostname << ":" << port << " ===" << std::endl;

    for (int i = 0; i < 10; ++i)
    {
        test_from1();
        test_from2();
        test_from3();
        test_from4();
    }

    return 0;
}
