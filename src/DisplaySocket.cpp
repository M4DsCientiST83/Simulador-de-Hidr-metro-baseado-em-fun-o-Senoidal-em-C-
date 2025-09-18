#include "DisplaySocket.hpp"
#include <string>

DisplaySocket::DisplaySocket(SOCKET clientSocket) : socket_fd(clientSocket) {}

void DisplaySocket::exibir(int tempo, double volume) 
{
    std::string msg = "tempo=" + std::to_string(tempo) + ";volume=" + std::to_string(volume) + "\n";
    
    send(socket_fd, msg.c_str(), (int)msg.size(), 0);
}
