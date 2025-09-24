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
    // Classe para leitura e extração dos dados do arquivo json de configuração 
    public:

        Config();
        ~Config() = default;

        void carregarDeArquivo(const string& nomeArquivo);
        double getdE();
        double getvmFA();
        int gettu();

    private:
        // Atributos armazenados pela classe para que sejam passados para o Hidrômetro
        double diamEnt; 
        double velmedFA;
        int tempoatt; 
};

#endif