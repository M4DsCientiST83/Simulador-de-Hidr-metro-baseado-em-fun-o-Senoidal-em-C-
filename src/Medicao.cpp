#include "Medicao.hpp"

Medicao::Medicao() : volume(0.0) {}

void Medicao::setVolume(double vol)
{
    volume += vol;
}

double Medicao::vazaoEntrada(double dE, double vmFA) 
{
    double area = PI * (dE * dE / 4.0);

    return area * vmFA * 3600;
}

void Medicao::atualizar(int tempo, double dE, double vmFA)
{
    int horaDoDia = tempo % 24;
    
    double oscillacao = (sin((2* PI * horaDoDia / 24.0) - (PI / 2)) + 1.0) / 2.0;
    double fatorOscilatorio = 0.1 + (oscillacao * 0.9);
    
    double velocidadeOscilatoria = vmFA * fatorOscilatorio;
    double vazaoDestaHora = vazaoEntrada(dE, velocidadeOscilatoria);
    
    setVolume(vazaoDestaHora);
}

double Medicao::getVolume()
{
    return volume;
}