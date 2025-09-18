#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <string>
using std::string;

#include "Hidrometro.hpp"
#include "Config.hpp"

class Control
{
    public:

        Control(const string &arquivoConfig, Display *disp);

        ~Control();

        void executar();


    private:

        Hidrometro *hidrometro;
        Config config;

};

#endif
