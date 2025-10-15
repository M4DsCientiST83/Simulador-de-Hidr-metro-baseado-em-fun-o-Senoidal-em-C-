#include "../include/Control.hpp"
#include "../include/Config.hpp"

Control::Control(const string &arquivoConfig, Display *disp)
{
    // Executa o algoritmo de leitura
    config.carregarDeArquivo(arquivoConfig);

    // Constrói um objeto Hidrômetro utilizando os atributos extraídos do arquivo .json 
    hidrometro = new Hidrometro(config.getdE(), config.getvmFA(), config.gettu(), disp);
}

Control::~Control()
{
    delete hidrometro;
}

void Control::executar()
{
    // Executa a simulação
    hidrometro->simular();
}