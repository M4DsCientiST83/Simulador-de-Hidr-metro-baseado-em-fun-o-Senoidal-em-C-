#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class Config
{
    public:

        static tuple<double, double> carregarDeArquivo(const string& nomeArquivo);
};

#endif