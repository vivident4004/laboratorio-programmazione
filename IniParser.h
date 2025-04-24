//
// Created by vivident4004 on 10/04/2025.
//
#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <string>
#include <map>
#include <vector>

class IniParser {
public:
    IniParser();
    explicit IniParser(const std::string& filename);
    bool load(const std::string& filename);
    bool save(const std::string& filename) const;
    std::string getValue(const std::string& section, const std::string& key);
    void setValue(const std::string& section, const std::string& key, const std::string& value);
    static bool hasKey(const std::string& section, const std::string& key);
    static std::vector<std::string> hasKey(const std::string& key);
    bool deleteKey(const std::string& section, const std::string& key);
    void addSection(const std::string& section);
    bool hasSection(const std::string& section) const;
    bool deleteSection(const std::string& section);
    std::string print(bool print_comments = true) const;

private:
    std::string filename;
    std::map<std::string, std::map<std::string, std::string>> data;
    static std::string toLower(const std::string& str);
};

#endif // INI_PARSER_H
