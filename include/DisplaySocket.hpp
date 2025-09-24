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
    // Classe responsável pelo envio dos valores de volume medidos pelo hidrômetro ao algoritmo java para exibição na tela
    public:

        DisplaySocket(SOCKET clientSocket);

        void exibir(int tempo, double volume) override;
        
    private:

        SOCKET socket_fd;
};


#endif