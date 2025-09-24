#ifndef MEDICAO_HPP
#define MEDICAO_HPP
#define PI 3.14159265358979323846

#include <iostream>
#include <cmath>

using std::sin;


class Medicao
{
    // Classe responsável por realizar os cálculos e operações para obtenção de valores de volume por hora
    public:
        
        Medicao();

        double vazaoEntrada(double dE, double vmFA);

        void atualizar(int tempo, double dE, double vmFA);

        double getVolume();

        void setVolume(double vol);
        

    private:
        
        double volume;

};

#endif