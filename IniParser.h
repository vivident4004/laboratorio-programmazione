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
    IniParser() = default;
    explicit IniParser(const std::string& filename);
    bool load(const std::string& filename);
    bool save(const std::string& filename) const;
    std::string getValue(const std::string& section, const std::string& key) const;
    void setValue(const std::string& section, const std::string& key, const std::string& value);
    bool hasKey(const std::string& section, const std::string& key) const;
    std::vector<std::string> hasKey(const std::string& key) const;
    bool deleteKey(const std::string& section, const std::string& key);
    void addSection(const std::string& section);
    bool hasSection(const std::string& section) const;
    std::vector<std::string> findSectionsContainingWord(const std::string& word) const;
    void addCommentToParam(const std::string& section, const std::string& param, const std::string& comment);
    std::string getCommentFromParam(const std::string &section, const std::string &param) const;
    bool deleteCommentFromParam(const std::string &section, const std::string &param);

    bool deleteSection(const std::string& section);
    std::string toString() const;

private:
    std::string filename;
    std::map<std::string, std::map<std::string, std::string>> data;
    std::map<std::string, std::map<std::string, std::string>> paramComments;
    static std::string toLower(const std::string& str);
};

#endif // INI_PARSER_H
