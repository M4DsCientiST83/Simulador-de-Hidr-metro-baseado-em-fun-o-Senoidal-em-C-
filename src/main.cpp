#include "Control.hpp"
#include <filesystem>

namespace fs = std::filesystem;

int main()
{
    fs::path caminhoBase = fs::current_path().parent_path();
    fs::path configPath = caminhoBase / "data" / "config.json";

    Control control(configPath.string());

    control.executar();

    return 0;
}