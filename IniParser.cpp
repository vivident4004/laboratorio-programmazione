//
// Created by vivident4004 on 10/04/2025.
//
#include <algorithm>

#include "IniParser.h"
#include <fstream>
#include <iostream>
#include <cctype> // Per ::tolower

IniParser::IniParser() {
    try {
        load(filename);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

std::string IniParser::toLower(const std::string& str)
{
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

bool IniParser::load(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Impossibile aprire il file: " << filename << std::endl;
        return false;
    }

    // Pulisco eventuali dati esistenti
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

            // Converti la chiave in minuscolo per l'archiviazione
            std::string lowerKey = toLower(key);
            data[currentSection][lowerKey] = value;
        }
    }

    file.close();
    return true;
}

bool IniParser::save(const std::string& filename) const {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cout << "Impossibile aprire il file: " << filename << std::endl;
        return false;
    }

    for (const auto& sectionPair : data) // sectionPair.first è il nome sezione (minuscolo), sectionPair.second è la mappa chiavi/valori
    {
        // Stampa il nome della sezione
        file << '[' << sectionPair.first << ']' << std::endl;

        for (const auto& keyPair : sectionPair.second) // keyPair.first è la chiave (minuscolo), keyPair.second è il valore
        {
            // Stampa chiave=valore
            file << keyPair.first << '=' << keyPair.second << std::endl;
        }
        // Aggiungi una linea vuota tra le sezioni per leggibilità
        file << std::endl;
    }

    file.close();
    return true;
}

std::string IniParser::getValue(const std::string& section, const std::string& key) {
    // Controlla se la sezione esiste
    if (data.find(section) != data.end()) {
        // Controlla se la chiave esiste nella sezione
        if (data[section].find(key) != data[section].end()) {
            return data[section][key];
        }
    }

    return ""; // Ritorna stringa vuota se non trovato
}

void IniParser::setValue(const std::string& section, const std::string& key, const std::string& value) {
    data[section][key] = value;
}

std::string IniParser::print(bool print_comments /* ignored in this block */) const
{
    std::string output;

    // Itera attraverso le sezioni (le mappe iterano in ordine alfabetico per chiave, che sono i nomi sezione in minuscolo)
    for (const auto& sectionPair : data) // sectionPair.first = nome sezione (minuscolo), sectionPair.second = mappa chiavi/valori
    {
        // Stampa il nome della sezione tra parentesi quadre
        output += '[' + sectionPair.first + ']' + '\n';

        // Itera attraverso le coppie chiave=valore all'interno della sezione
        for (const auto& keyPair : sectionPair.second) // keyPair.first = chiave (minuscolo), keyPair.second = valore
        {
            // Stampa la coppia chiave=valore
            output += keyPair.first + '=' + keyPair.second + '\n';
        }
        output += '\n'; // Aggiunge una linea vuota tra le sezioni per leggibilità
    }

    return output;
}