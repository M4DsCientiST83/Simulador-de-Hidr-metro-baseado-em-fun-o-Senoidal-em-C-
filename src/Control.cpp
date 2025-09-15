#include "Control.hpp"
#include "Config.hpp"

Control::Control(const string &arquivoConfig)
{
    auto [dE,vmFA, tu] = Config::carregarDeArquivo(arquivoConfig);
    hidrometro = new Hidrometro(dE, vmFA, tu);
}

Control::~Control()
{
    delete hidrometro;
}

void Control::executar()
{
    hidrometro->simular();
}