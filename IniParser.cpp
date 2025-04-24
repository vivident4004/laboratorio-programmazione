//
// Created by vivident4004 on 10/04/2025.
//
#include "IniParser.h"
#include <fstream>
#include <iostream>
#include <cctype> // Per ::tolower
#include <algorithm> // Per std::transform

IniParser::IniParser() = default;

IniParser::IniParser(const std::string& filename) {
    if (!load(filename)) {
        std::cerr << "Errore nel caricamento del file nel costruttore: "
                  << filename << std::endl;
    }
}

std::string IniParser::toLower(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

bool IniParser::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Impossibile aprire il file: " << filename << std::endl;
        return false;

    }

    std::string line;
    std::string section;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == ';') {
            continue;
        }

        // Parsing delle sezioni [SectionName]
        if (line[0] == '[' && line.back() == ']') {
            // Estrae il nome senza '[' e ']'
            section = toLower(line.substr(1, line.length() - 2));
            data[section]; // Assicura che la sezione esista
            continue;
        }

        // Parsing delle coppie chiave=valore (solo se siamo dentro una sezione valida)
        if (!section.empty()) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                // Estrae chiave e valore, senza trim
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                std::string lowerKey = toLower(key);

                if (!lowerKey.empty()) { // Ignora chiavi vuote
                    data[section][lowerKey] = value;
                }
            }
        }
    }
    file.close();
    return true;
}

bool IniParser::save(const std::string& filename) const {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Impossibile aprire il file per la scrittura: " << filename << std::endl;
        return false;
    }

    for (const auto& sectionPair : data) {
        file << '[' << sectionPair.first << ']' << std::endl;

        for (const auto& keyPair : sectionPair.second) {
            file << keyPair.first << '=' << keyPair.second << std::endl;
        }
        file << std::endl;
    }

    file.close();
    return !file.fail();
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
    data[toLower(section)][toLower(key)] = value;
}

void IniParser::addSection(const std::string& section) {
    data[toLower(section)];
}

bool IniParser::hasSection(const std::string& section) const {
    return data.find(toLower(section)) != data.end();
}

bool IniParser::hasKey(const std::string& section, const std::string& key) {
    // TODO
    return false;
}

std::vector<std::string> IniParser::hasKey(const std::string& key) {
    // TODO
    return {};
}

bool IniParser::deleteKey(const std::string& section, const std::string& key) {
    // TODO
    return false;
}

std::string IniParser::print(bool print_comments /* ignored */) const {
    std::string output;

    for (const auto& sectionPair : data) {
        output += '[' + sectionPair.first + ']' + '\n';
        for (const auto& keyPair : sectionPair.second) {
            output += keyPair.first + '=' + keyPair.second + '\n';
        }
        output += '\n';
    }

    return output;
}