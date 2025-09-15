#include "Config.hpp"

tuple<double, double, int> Config::carregarDeArquivo(const string &nomeArquivo)
{
    ifstream arquivo(nomeArquivo);
    double dE = 0.1, vmFA = 1.0; // valores padrão
    int tu = 2;

    if (!arquivo) 
    {
        cerr << "Erro ao abrir arquivo de configuração. Usando valores padrão.\n";
        return {dE, vmFA, tu};
    }

    json j;
    arquivo >> j;

    if (j.contains("diametro do cano de entrada em m")) dE = j["diametro do cano de entrada em m"];
    if (j.contains("velocidade média do fluxo de Água pelo cano de entrada")) vmFA = j["velocidade média do fluxo de Água pelo cano de entrada"];
    if (j.contains("tempo de atualização do simulador")) tu = j["tempo de atualização do simulador"];

    return {dE, vmFA, tu};
}