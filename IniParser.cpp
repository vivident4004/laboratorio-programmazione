#include "IniParser.h"
#include <fstream>
#include <iostream>

bool IniParser::loadFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Impossibile aprire il file: " << filename << std::endl;
        return false;
    }

    // Resetta i dati precedenti
    data.clear();

    std::string line;
    std::string currentSection;

    while (std::getline(file, line)) {
        // Ignora le linee vuote
        if (line.empty()) {
            continue;
        }

        // Ignora i commenti
        if (line[0] == ';') {
            continue;
        }

        // Parsing delle sezioni
        if (line[0] == '[') {
            size_t end = line.find(']');
            if (end != std::string::npos) {
                currentSection = line.substr(1, end - 1);
            }
            continue;
        }

        // Parsing delle coppie chiave=valore
        size_t equalPos = line.find('=');
        if (equalPos != std::string::npos) {
            std::string key = line.substr(0, equalPos);
            std::string value = line.substr(equalPos + 1);

            data[currentSection][key] = value;
        }
    }

    file.close();
    return true;
}

std::string IniParser::getValue(const std::string& section, const std::string& key) const {
    // Controlla se la sezione esiste
    auto sectionIt = data.find(section);
    if (sectionIt != data.end()) {
        // Controlla se la chiave esiste nella sezione
        auto keyIt = sectionIt->second.find(key);
        if (keyIt != sectionIt->second.end()) {
            return keyIt->second;
        }
    }

    return ""; // Ritorna stringa vuota se non trovato
}

void IniParser::setValue(const std::string& section, const std::string& key, const std::string& value) {
    data[section][key] = value;
}
