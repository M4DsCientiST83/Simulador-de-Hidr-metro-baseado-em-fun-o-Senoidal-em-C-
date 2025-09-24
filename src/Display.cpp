#include "Display.hpp"

void Display::exibir(int tempo, double volume)
{
    // Exibe os valores de volume acumulado e tempo na tela (atualmente apenas útil para debug)
    std::cout << "Hora " << tempo << " -> Volume acumulado: " << volume << " metros cubicos" << std::endl;
}