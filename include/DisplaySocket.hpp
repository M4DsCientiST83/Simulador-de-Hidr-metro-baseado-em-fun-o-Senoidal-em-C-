#ifndef DISPLAY_SOCKET_HPP
#define DISPLAY_SOCKET_HPP

#include "Display.hpp"
#include <string>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif

class DisplaySocket : public Display 
{
    public:

        DisplaySocket(SOCKET clientSocket);

        void exibir(int tempo, double volume) override 
        {
            std::string msg = "tempo=" + std::to_string(tempo) + ";volume=" + std::to_string(volume) + "\n";
            send(socket_fd, msg.c_str(), (int)msg.size(), 0);
        }

    private:

        SOCKET socket_fd;
};


#endif