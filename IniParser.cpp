//
// Created by vivident4004 on 10/04/2025.
//
#include "IniParser.h"
#include <fstream>
#include <iostream>
#include <cctype> // Per ::tolower
#include <algorithm> // Per std::transform

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
        std::cerr << "Impossibile aprire \"" << filename << "\"" << std::endl;
        return false;
    }

    this->filename = filename;
    data.clear();
    paramComments.clear();

    std::string line;
    std::string currentSection;
    std::string lastParamKey; // Per associare commenti al parametro precedente

    while (std::getline(file, line)) {
        // Rimuovi spazi bianchi iniziali e finali
        line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

        if (line.empty()) {
            lastParamKey.clear(); // Resetta dopo una linea vuota
            continue;
        }

        // Parsing dei commenti (linee che iniziano con ';' o '#')
        if (line[0] == ';' || line[0] == '#') {
            if (!currentSection.empty() && !lastParamKey.empty()) {
                // Assumiamo che il commento si riferisca all'ultimo parametro letto nella sezione corrente
                // Rimuovi il carattere di commento e lo spazio iniziale
                std::string commentContent = line.substr(1);
                commentContent.erase(0, commentContent.find_first_not_of(" \t"));

                // Se c'è già un commento, lo accodiamo. Altrimenti lo impostiamo.
                if (paramComments[currentSection].count(lastParamKey)) {
                    paramComments[currentSection][lastParamKey] += '\n' + commentContent;
                } else {
                    paramComments[currentSection][lastParamKey] = commentContent;
                }
            }
            // Altrimenti è un commento generico o di sezione, per ora lo ignoriamo se non è associabile a un parametro
            continue;
        }

        // Parsing delle sezioni [SectionName]
        if (line[0] == '[' && line.back() == ']') {
            currentSection = toLower(line.substr(1, line.length() - 2));
            data[currentSection]; // Assicura che la sezione esista
            paramComments[currentSection]; // Assicura che la sezione esista per i commenti
            lastParamKey.clear(); // Resetta quando cambia sezione
            continue;
        }

        // Parsing delle coppie chiave=valore
        if (!currentSection.empty()) {
            size_t delimiterPos = line.find('=');
            if (delimiterPos != std::string::npos) {
                std::string key = line.substr(0, delimiterPos);
                std::string value = line.substr(delimiterPos + 1);

                // Rimuovi spazi bianchi da chiave e valore
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);

                std::string lowerKey = toLower(key);
                if (!lowerKey.empty()) {
                    data[currentSection][lowerKey] = value;
                    lastParamKey = lowerKey; // Salva questa chiave come l'ultima letta
                } else {
                    lastParamKey.clear();
                }
            } else {
                lastParamKey.clear(); // Linea non valida come K=V
            }
        }
    }
    file.close();
    return true;
}


bool IniParser::save(const std::string& filename) const {
    std::ofstream file(filename.empty() ? this->filename : filename);
    if (!file.is_open()) {
        std::cerr << "Impossibile aprire il file per la scrittura: "
                  << (filename.empty() ? this->filename : filename) << std::endl;
        return false;
    }

    for (const auto& sectionPair : data) {
        file << '[' << sectionPair.first << ']' << std::endl;

        const auto& sectionCommentsIt = paramComments.find(sectionPair.first);

        for (const auto& keyPair : sectionPair.second) {
            // Scrivi il commento per questo parametro, se esiste
            if (sectionCommentsIt != paramComments.end()) {
                const auto& paramCommentMap = sectionCommentsIt->second;
                const auto& commentIt = paramCommentMap.find(keyPair.first);
                if (commentIt != paramCommentMap.end() && !commentIt->second.empty()) {
                    // Scrivi ogni riga del commento preceduta da ';'
                    std::string fullComment = commentIt->second;
                    size_t startPos = 0;
                    while((startPos = fullComment.find_first_not_of('\n', startPos)) != std::string::npos) {
                        size_t endPos = fullComment.find_first_of('\n', startPos);
                        file << "; " << fullComment.substr(startPos, endPos - startPos) << std::endl;
                        if (endPos == std::string::npos) break;
                        startPos = endPos +1;
                    }
                }
            }
            file << keyPair.first << '=' << keyPair.second << std::endl;
        }
        file << std::endl;
    }

    file.close();
    return !file.fail();
}


std::string IniParser::getValue(const std::string& section, const std::string& key) const {
    // Controlla se la sezione esiste
    if (const auto sectionIt = data.find(section); sectionIt != data.end()) {
        // Si usa ->second per accedere al valore dall'iteratore (const correct)
        const auto& innerMap = sectionIt->second;

        if (const auto keyIt = innerMap.find(key); keyIt != innerMap.end()) {
            return keyIt->second;
        }
    }

    return ""; // Ritorna stringa vuota se non è stato trovato nessun valore
}

void IniParser::setValue(const std::string& section, const std::string& key, const std::string& value) {
    data[toLower(section)][toLower(key)] = value;
}

void IniParser::addSection(const std::string& section) {
    const std::string lowerSection = toLower(section);
    data[lowerSection];
    paramComments[lowerSection];

}

bool IniParser::hasSection(const std::string& section) const {
    return data.find(toLower(section)) != data.end();
}

bool IniParser::hasKey(const std::string& section, const std::string& key) const {
    const auto sectionIt = data.find(toLower(section));

    if (sectionIt == data.end()) {
        return false;
    }

    return sectionIt->second.find(toLower(key)) != sectionIt->second.end();
}

std::vector<std::string> IniParser::hasKey(const std::string& key) const {
    std::vector<std::string> sections;
    const std::string lowerKey = toLower(key);

    for (const auto& sectionPair : data) {
        if (sectionPair.second.find(lowerKey) != sectionPair.second.end()) {
            sections.push_back(sectionPair.first);
        }
    }

    return sections;
}

std::vector<std::string> IniParser::findSectionsContainingWord(const std::string& word) const {
    std::vector<std::string> foundSections;
    if (word.empty()) { // Se la parola è vuota, non cercare nulla.
        return foundSections;
    }
    const std::string lowerWord = toLower(word);

    for (const auto& sectionPair : data) {
        // sectionPair.first è il nome della sezione
        if (toLower(sectionPair.first).find(lowerWord) != std::string::npos) {
            foundSections.push_back(sectionPair.first); // Restituisce il nome della sezione così com'è memorizzato
        }
    }
    return foundSections;
}

void IniParser::addCommentToParam(const std::string &section, const std::string& param, const std::string &comment) {
    const std::string lowerSection = toLower(section);
    const std::string lowerParam = toLower(param);

    if (data.count(lowerSection) && data.at(lowerSection).count(lowerParam)) {
        paramComments[lowerSection][lowerParam] = comment;
    } else {
        std::cerr << "Attenzione: sezinoe '" << section << "' o parametro '" << param << "' non trovati. Nessun commento aggiunto." << std::endl;
    }
}

std::string IniParser::getCommentFromParam(const std::string& section, const std::string& param) const {
    const std::string lowerSection = toLower(section);
    const std::string lowerParam = toLower(param);

    const auto sectionIt = paramComments.find(lowerSection);
    if (sectionIt != paramComments.end()) {
        const auto paramIt = sectionIt->second.find(lowerParam);
        if (paramIt != sectionIt->second.end()) {
            return paramIt->second;
        }
    }
    return "";
}

bool IniParser::deleteCommentFromParam(const std::string& section, const std::string& param) {
    const std::string lowerSection = toLower(section);
    const std::string lowerParam = toLower(param);

    const auto sectionIt = paramComments.find(lowerSection);
    if (sectionIt != paramComments.end()) {
        return sectionIt->second.erase(lowerParam) > 0;
    }
    return false;
}

bool IniParser::deleteKey(const std::string& section, const std::string& key) {
    const std::string lowerSection = toLower(section);
    const std::string lowerKey = toLower(key);

    const auto sectionIt = data.find(lowerSection);
    if (sectionIt != data.end()) {
        bool erasedFromData = sectionIt->second.erase(lowerKey) > 0;
        // Elimina anche il commento associato, se presente
        const auto commentSectionIt = paramComments.find(lowerSection);
        if (commentSectionIt != paramComments.end()) {
            commentSectionIt->second.erase(lowerKey);
        }
        return erasedFromData;
    }
    return false;

}

bool IniParser::deleteSection(const std::string& section) {
    const std::string lowerSection = toLower(section);

    const bool erasedFromData = data.erase(lowerSection) > 0;
    const bool erasedFromComments = paramComments.erase(lowerSection) > 0;
    return erasedFromData || erasedFromComments; // Ritorna true se almeno una mappa è stata modificata

}

std::string IniParser::toString() const {
    std::string output;

    // Itera su tutte le sezioni presenti nella mappa 'data'
    for (const auto& sectionPair : data) {
        const std::string& sectionName = sectionPair.first;
        output += '[' + sectionName + ']' + '\n';

        // Cerca se ci sono commenti associati alle chiavi di questa sezione
        auto itSectionComments = paramComments.find(sectionName);

        for (const auto& keyPair : sectionPair.second) {
            const std::string& keyName = keyPair.first;
            const std::string& keyValue = keyPair.second;

            // Se è stata trovata una mappa di commenti per questa sezione
            if (itSectionComments != paramComments.end()) {
                const auto& keyCommentMap = itSectionComments->second;
                // Cerca un commento specifico per la chiave corrente
                auto itKeyComment = keyCommentMap.find(keyName);
                if (itKeyComment != keyCommentMap.end() && !itKeyComment->second.empty()) {
                    output += "; " + itKeyComment->second + '\n';
                }
            }
            output += keyName + '=' + keyValue + '\n';
        }
        output += '\n';
    }
    return output;
}