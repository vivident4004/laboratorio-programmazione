#include "IniParser.h"
#include <iostream>

int main() {
    std::cout << "Parser di file INI - Versione base" << std::endl;

    IniParser parser;

    // Crea un file INI semplice in memoria
    parser.setValue("general", "name", "INI Parser App");
    parser.setValue("general", "version", "1.0");
    parser.setValue("settings", "language", "Italian");

    // Mostra i valori
    std::cout << "Name: " << parser.getValue("general", "name") << std::endl;
    std::cout << "Version: " << parser.getValue("general", "version") << std::endl;
    std::cout << "Language: " << parser.getValue("settings", "language") << std::endl;

    return 0;
}
