#include "Hidrometro.hpp"

Hidrometro::Hidrometro(double dE, double vmFA) : diam_entrada(dE), velmediaFluxoAgua(vmFA), medicao(), display() {}

double Hidrometro::getDiamEnt()
{
    return diam_entrada;
}

double Hidrometro::getvelmediaFluxoAgua()
{
    return velmediaFluxoAgua;
}

void Hidrometro::simular()
{
    int hora = 0;

    while(1)
    {
        medicao.atualizar(hora, getDiamEnt(), getvelmediaFluxoAgua());
        display.exibir(hora, medicao.getVolume());



        this_thread::sleep_for(chrono::seconds(2)); // 1h simulada = 2s reais

        hora++;

    }
           
}