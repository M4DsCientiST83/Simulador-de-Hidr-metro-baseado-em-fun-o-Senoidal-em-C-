#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include "json.hpp"

using std::string;


class Config
{
    public:

        Config();
        void carregarDeArquivo(const string& nomeArquivo);

        ~Config() = default;

        double getdE();
        double getvmFA();
        int gettu();

    private:

        double diamEnt; 
        double velmedFA;
        int tempoatt; 
};

#endif