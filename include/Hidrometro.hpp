#ifndef HIDROMETRO_HPP
#define HIDROMETRO_HPP

#include "DisplaySocket.hpp"
#include "Medicao.hpp"
#include <chrono>
#include <thread>


class Hidrometro
{

    public: 
    
        Hidrometro(double dE, double vmFA, int tu, Display* disp = nullptr);
        ~Hidrometro();

        void simular();

        double getDiamEnt();
        double getvelmediaFluxoAgua();
        int getTempoUpdate();


    private:

        double diam_entrada, velmediaFluxoAgua;
        int tempo_update;
        Display* display;
        Medicao medicao;

};


#endif