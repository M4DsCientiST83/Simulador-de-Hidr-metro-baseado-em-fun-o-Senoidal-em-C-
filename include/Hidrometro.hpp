#ifndef HIDROMETRO_HPP
#define HIDROMETRO_HPP

#include "Display.hpp"
#include "Medicao.hpp"
#include <chrono>
#include <thread>


class Hidrometro
{

    public: 
    
        Hidrometro(double dE, double vmFA, int tu);
        void simular();

        double getDiamEnt();
        double getvelmediaFluxoAgua();
        int getTempoUpdate();


    private:

        double diam_entrada, velmediaFluxoAgua;
        int tempo_update;
        Display display;
        Medicao medicao;

};


#endif