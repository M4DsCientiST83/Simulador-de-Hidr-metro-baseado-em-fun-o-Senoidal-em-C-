#ifndef HIDROMETRO_HPP
#define HIDROMETRO_HPP

#include "Display.hpp"
#include "Medicao.hpp"
#include <chrono>
#include <thread>


class Hidrometro
{

    public: 
    
        Hidrometro(double dE, double vmFA);
        void simular();

        double getDiamEnt();
        double getvelmediaFluxoAgua();


    private:

        double diam_entrada, velmediaFluxoAgua;
        Display display;
        Medicao medicao;

};


#endif