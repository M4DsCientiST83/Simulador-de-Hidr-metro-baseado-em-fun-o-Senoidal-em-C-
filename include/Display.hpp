#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <iostream>


class Display
{

    public:
        
        virtual void exibir(int tempo, double volume);
        virtual ~Display() = default;

};

#endif