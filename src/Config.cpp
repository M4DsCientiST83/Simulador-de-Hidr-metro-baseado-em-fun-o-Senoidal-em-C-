#include "Config.hpp"

tuple<double, double> Config::carregarDeArquivo(const string &nomeArquivo)
{
    ifstream arquivo(nomeArquivo);
    double dE = 0.1, vmFA = 1.0; // valores padrão

    if (!arquivo) 
    {
        cerr << "Erro ao abrir arquivo de configuração. Usando valores padrão.\n";
        return {dE, vmFA};
    }

    json j;
    arquivo >> j;

    if (j.contains("diametro do cano de entrada em m")) dE = j["diametro do cano de entrada em m"];
    if (j.contains("velocidade média do fluxo de Água pelo cano de entrada")) vmFA = j["velocidade média do fluxo de Água pelo cano de entrada"];

    return {dE, vmFA};
}