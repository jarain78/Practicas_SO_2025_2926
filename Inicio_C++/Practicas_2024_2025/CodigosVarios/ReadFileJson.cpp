#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main() {
    std::ifstream file("salida.json");
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string json = buffer.str();

    std::cout << "Contenido JSON crudo:\n" << json << "\n";

    // Búsqueda simple de valores (no recomendado para JSON complejo)
    std::size_t nombre_pos = json.find("\"nombre\"");
    if (nombre_pos != std::string::npos) {
        auto start = json.find("\"", nombre_pos + 9) + 1;
        auto end = json.find("\"", start);
        std::string nombre = json.substr(start, end - start);
        std::cout << "Nombre encontrado: " << nombre << "\n";
    }

    return 0;
}




