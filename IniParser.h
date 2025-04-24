//
// Created by vivident4004 on 10/04/2025.
//
#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <string>
#include <map>
#include <vector>

// Semplice parser per file INI
class IniParser {
public:
    IniParser();
    explicit IniParser(const std::string& filename);
    // Carica un file INI
    bool load(const std::string& filename);

    // Salva i dati attuali su un file.
    // Sovrascrive il file se esiste.
    // Lancia std::runtime_error in caso di errori di I/O.
    bool save(const std::string& filename) const;
    //bool save() const;

    // Ottiene il valore associato a una chiave in una sezione.
    // Le sezioni e le chiavi sono gestite in modo case-insensitive.
    // Ritorna una stringa vuota se la sezione o la chiave non esistono.
    std::string getValue(const std::string& section, const std::string& key);

    // Imposta o aggiunge un valore per una chiave in una sezione.
    // Se la sezione o la chiave non esistono, vengono create.
    // Le sezioni e le chiavi sono gestite in modo case-insensitive.
    void setValue(const std::string& section, const std::string& key, const std::string& value);

    bool hasKey(const std::string& section, const std::string& key) const;
    std::vector<std::string> hasKey(const std::string& key) const; // Cerca chiave in tutte le sezioni
    bool deleteKey(const std::string& section, const std::string& key);

    // ancora da implementare
    // void addSection(const std::string& section);
    // bool hasSection(const std::string& section) const;
    // bool deleteSection(const std::string& section);


    std::string print(bool print_comments = true) const;

private:
    // Mappa per memorizzare i dati del file INI
    std::string filename;
    std::map<std::string, std::map<std::string, std::string>> data;

    static std::string toLower(const std::string& str);
};

#endif // INI_PARSER_H
