#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <iostream>


class Display
{
    // Classe base para a construção do DisplaySocket (anteriormente utilizada para gerar print na tela)
    public:
        
        virtual void exibir(int tempo, double volume);
        virtual ~Display() = default;

};

#endif