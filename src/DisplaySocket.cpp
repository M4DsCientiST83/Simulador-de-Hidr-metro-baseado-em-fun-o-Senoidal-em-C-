#include "DisplaySocket.hpp"
#include <string>

DisplaySocket::DisplaySocket(SOCKET clientSocket) : socket_fd(clientSocket) {}

void DisplaySocket::exibir(int tempo, double volume) 
{
    // Constrói a mensagem com as informações necessárias ao algoritmo java
    std::string msg = "tempo=" + std::to_string(tempo) + ";volume=" + std::to_string(volume) + "\n";
    
    // Envia a mensagem construída
    send(socket_fd, msg.c_str(), (int)msg.size(), 0);
}
