#ifndef CONTROL_HPP
#define CONTROL_HPP

#include "Hidrometro.hpp"

class Control
{
    public:

        Control(const string &arquivoConfig);

        ~Control();

        void executar();


    private:

        Hidrometro *hidrometro;

};

#endif
