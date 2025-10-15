#include "DisplaySocket.hpp"
#include <string>
#include <iostream> 
#include <thread>   

void DisplaySocket::exibir(int tempo, double volume) 
{
    // Constrói a mensagem com as informações necessárias ao algoritmo java
    std::string msg = "tempo=" + std::to_string(tempo) + ";volume=" + std::to_string(volume) + "\n";
    
    // Novo log adicionado aqui
    // std::cout << "Thread " << std::this_thread::get_id() << ": Enviando via socket: " << msg;
    
    // Envia a mensagem construída
    send(socket_fd, msg.c_str(), (int)msg.size(), 0);
}