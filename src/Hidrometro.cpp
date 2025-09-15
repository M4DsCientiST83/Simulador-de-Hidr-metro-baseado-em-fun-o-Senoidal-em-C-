#include "Hidrometro.hpp"

Hidrometro::Hidrometro(double dE, double vmFA, int tu) : diam_entrada(dE), velmediaFluxoAgua(vmFA), tempo_update(tu), medicao(), display() {}

double Hidrometro::getDiamEnt()
{
    return diam_entrada;
}

double Hidrometro::getvelmediaFluxoAgua()
{
    return velmediaFluxoAgua;
}

int Hidrometro::getTempoUpdate()
{
    return tempo_update;
}

void Hidrometro::simular()
{
    int hora = 0;

    while(1)
    {
        medicao.atualizar(hora, getDiamEnt(), getvelmediaFluxoAgua());
        display.exibir(hora, medicao.getVolume());



        this_thread::sleep_for(chrono::seconds(getTempoUpdate())); // 1h simulada = 2s reais

        hora++;

    }
           
}