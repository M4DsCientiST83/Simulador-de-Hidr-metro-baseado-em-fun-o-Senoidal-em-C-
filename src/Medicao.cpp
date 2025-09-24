#include "Medicao.hpp"

Medicao::Medicao() : volume(0.0) {}

void Medicao::setVolume(double vol)
{
    volume += vol;
}

double Medicao::vazaoEntrada(double dE, double vmFA) 
{
    // Realia um cálculo de vazão da forma: Vazão = (pi * diametro_entrada² * volume_medio_fluxo_agua * 3600) / 4
    double area = PI * (dE * dE / 4.0);

    return area * vmFA * 3600; // o 3600 representa um cálculo ajustado para um padrão em horas, ao invés de segundos
}

void Medicao::atualizar(int tempo, double dE, double vmFA)
{
    int horaDoDia = tempo % 24; // Atualiza o tempo para horas do dia em um padrão de 00h a 23h
    
    double oscillacao = (sin((2* PI * horaDoDia / 24.0) - (PI / 2)) + 1.0) / 2.0; 
    double fatorOscilatorio = 0.1 + (oscillacao * 0.9); // Calcula uma oscilação para simular a variação do consumo de água ao longo do dia
    
    double velocidadeOscilatoria = vmFA * fatorOscilatorio; // Aplica a oscilação na velocidade média de fluxo de água 
    double vazao = vazaoEntrada(dE, velocidadeOscilatoria);
    
    setVolume(vazao);
}

double Medicao::getVolume()
{
    return volume;
}