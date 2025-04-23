#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <string>
#include <map>

class IniParser {
public:
    // Costruttore predefinito
    IniParser() = default;

    // Carica un file INI
    bool loadFile(const std::string& filename);

    // Ottiene un valore da una sezione e chiave
    std::string getValue(const std::string& section, const std::string& key) const;

    // Imposta un valore
    void setValue(const std::string& section, const std::string& key, const std::string& value);

private:
    // Mappa per memorizzare i dati del file INI
    std::map<std::string, std::map<std::string, std::string>> data;
};

#endif // INI_PARSER_H
