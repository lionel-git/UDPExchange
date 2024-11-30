
#include <iostream>
#include <string>
#include <inttypes.h>

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

void initLibSockets()
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
#endif
}

void closeLibSockets()
{
#ifdef _WIN32
    // Clean up sockets library
    WSACleanup();
#endif
}

void send_msg(const std::string& msg)
{
   static std::string hostname{ "192.168.0.143" };
   static uint16_t port = 9000;
   
 // Set up connection and send    
    auto sock = ::socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in destination;
    destination.sin_family = AF_INET;
    destination.sin_port = htons(port);
    destination.sin_addr.s_addr = inet_addr(hostname.c_str());
    int n_bytes = ::sendto(sock, msg.c_str(), (int)msg.length(), 0, reinterpret_cast<sockaddr*>(&destination), sizeof(destination));
    std::cout << n_bytes << " bytes sent" << std::endl;
#ifdef _WIN32
    ::closesocket(sock);
#else
    ::close(sock);
#endif
}

int main()
{
  initLibSockets();

  std::string msg = "Jane Doe";
  send_msg(msg);
    
  closeLibSockets();
    
  return 0;
}
