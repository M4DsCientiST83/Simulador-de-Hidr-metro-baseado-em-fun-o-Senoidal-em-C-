#include "Control.hpp"
#include "Config.hpp"

Control::Control(const string &arquivoConfig)
{
    auto [dE,vmFA] = Config::carregarDeArquivo(arquivoConfig);
    hidrometro = new Hidrometro(dE, vmFA);
}

Control::~Control()
{
    delete hidrometro;
}

void Control::executar()
{
    hidrometro->simular();
}