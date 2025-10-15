#include "Hidrometro.hpp"
#include <iostream>

Hidrometro::Hidrometro(double dE, double vmFA, int tu, Display* disp) : diam_entrada(dE), velmediaFluxoAgua(vmFA), tempo_update(tu), medicao()
{
    if(disp) 
        display = disp;

    else 
        display = new Display(); // padrão: console
}

Hidrometro::~Hidrometro() 
{
    delete display;
}

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
    // Utiliza a classe medição para realizar os cálculos e a função exibir sobrescrita da classe DisplaySocket para atualizar as informações a serem exibidas na tela
    int hora = 0;
    auto thread_id = std::this_thread::get_id(); // Pega o ID da thread

    while(true) 
    {
        medicao.atualizar(hora, diam_entrada, velmediaFluxoAgua);
        
        // Novo log adicionado aqui
        std::cout << "Thread " << thread_id << ": [Hora " << hora << "] Atualizando display com volume " << medicao.getVolume() << std::endl;
        
        display->exibir(hora, medicao.getVolume());
        std::this_thread::sleep_for(std::chrono::seconds(tempo_update)); // sleep para alterar a velocidade com que o simulador roda
        hora++;
    }
}