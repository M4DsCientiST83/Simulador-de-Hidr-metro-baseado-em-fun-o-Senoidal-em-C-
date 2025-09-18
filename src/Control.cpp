#include "Control.hpp"
#include "Config.hpp"

Control::Control(const string &arquivoConfig, Display *disp)
{
    config.carregarDeArquivo(arquivoConfig);

    // Passa o display para o hidrometro
    hidrometro = new Hidrometro(config.getdE(), config.getvmFA(), config.gettu(), disp);
}

Control::~Control()
{
    delete hidrometro;
}

void Control::executar()
{
    hidrometro->simular();
}