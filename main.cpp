//
// Created by vivident4004 on 10/04/2025.
//

#include <iostream>
#include <fstream>

#include "IniParser.h"

int main() {
    const std::string filename = "demo_test.ini";
    IniParser parser;

    std::cout << "1. Costruzione della struttura INI in memoria..." << std::endl;
    parser.addSection("General");
    parser.setValue("General", "AppName", "TestApp");
    parser.setValue("General", "Version", "1.2");
    parser.addCommentToParam("General", "Version",
                             "Commento iniziale per Version");

    parser.addSection("UserSettings");
    parser.setValue("UserSettings", "Tema", "Scuro");
    parser.setValue("UserSettings", "GrandezzaCarattere", "12");

    parser.addSection("DatabaseConfig");
    parser.setValue("DatabaseConfig", "Host", "db.example.com");
    parser.addSection("NetworkConfig");
    parser.setValue("NetworkConfig", "Timeout", "30");
    parser.addSection("AnotherConfig");
    parser.setValue("AnotherConfig", "Mode", "Advanced");

    std::cout << "   Struttura INI costruita in memoria.\n" << std::endl;

    std::cout << "Contenuto iniziale (in memoria):\n"
              << parser.toString() << std::endl;

    std::string sectionSearchTerm = "Config";
    std::cout << "Ricerca sezioni contenenti la parola: \"" << sectionSearchTerm
              << "\"" << std::endl;

    std::vector<std::string> matchingSections =
        parser.findSectionsContainingWord(sectionSearchTerm);

    if (matchingSections.empty()) {
        std::cout << "Nessuna sezione trovata contenente la parola \""
                  << sectionSearchTerm << "\"." << std::endl;
    } else {
        std::cout << "Trovate " << matchingSections.size()
                  << " sezioni contenenti la parola \"" << sectionSearchTerm
                  << "\":" << std::endl;
        for (const std::string& sectionName : matchingSections) {
            std::cout << "  - Sezione: [" << sectionName << "]" << std::endl;
        }
    }
    std::cout << std::endl;

    std::cout << "2. Lettura valore [General]AppName: "
              << parser.getValue("General", "AppName") << "\n"
              << std::endl;

    std::cout << "3. Modifica [General]Version a '1.1'..." << std::endl;
    parser.setValue("General", "Version", "1.1");
    std::cout << "   Nuovo valore [General]Version: "
              << parser.getValue("General", "Version") << "\n"
              << std::endl;

    std::cout << "4. Aggiunta sezione [NewSection] e chiave TestKey=TestValue..."
              << std::endl;
    parser.addSection("NewSection");
    parser.setValue("NewSection", "TestKey", "TestValue");
    std::cout << "   Valore [NewSection]TestKey: "
              << parser.getValue("NewSection", "TestKey") << "\n"
              << std::endl;
    parser.addCommentToParam("newsection", "testkey",
        "Commento di prova - Federico Faggin");

    std::cout << "5. Modifica commento a [UserSettings]Tema..." << std::endl;
    parser.addCommentToParam("UserSettings", "Tema",
                             "Tema preferito dall'utente Faggin - aggiornato");
    std::cout << "   Commento per [UserSettings]Tema: '"
              << parser.getCommentFromParam("UserSettings", "Tema") << "'\n"
              << std::endl;

    std::string mainSearchTerm = "FAGGIN";
    std::cout << "6. Ricerca stringhe di commento contenenti la parola: \""
              << mainSearchTerm << "\"" << std::endl;

    std::vector<std::string> foundCommentStrings =
        parser.findCommentStringsContainingWord(mainSearchTerm);

    if (foundCommentStrings.empty()) {
        std::cout << "   Nessuna stringa di commento trovata contenente la parola \""
                  << mainSearchTerm << "\"." << std::endl;
    } else {
        std::cout << "   Trovate " << foundCommentStrings.size()
                  << " stringhe di commento contenenti la parola \""
                  << mainSearchTerm << "\":" << std::endl;
        for (const std::string& commentStr : foundCommentStrings) {
            std::cout << "   - Commento: \"" << commentStr << "\"" << std::endl;
        }
    }
    std::cout << std::endl;

    std::cout << "7. Eliminazione chiave [General]AppName..." << std::endl;
    parser.deleteKey("General", "AppName");
    std::cout << "   Chiave [General]AppName esiste? "
              << (parser.hasKey("General", "AppName") ? "Sì" : "No") << "\n"
              << std::endl;

    std::cout << "8. Eliminazione commento da [UserSettings]Tema..."
              << std::endl;
    parser.deleteCommentFromParam("UserSettings", "Tema");
    std::cout << "   Commento per [UserSettings]Tema dopo eliminazione: '"
              << parser.getCommentFromParam("UserSettings", "Tema") << "'\n"
              << std::endl;

    std::cout << "9. Eliminazione sezione [NewSection]..." << std::endl;
    parser.deleteSection("NewSection");
    std::cout << "   Sezione [NewSection] esiste? "
              << (parser.hasSection("NewSection") ? "Sì" : "No") << "\n"
              << std::endl;

    std::cout << "10. Contenuto INI corrente (toString):\n"
              << parser.toString() << std::endl;

    std::cout << "11. Salvataggio modifiche su '" << filename << "'..."
              << std::endl;
    if (parser.save(filename)) {
        std::cout << "    File salvato con successo.\n" << std::endl;
        std::ifstream infile(filename);
        std::cout << "Contenuto del file '" << filename
                  << "' dopo il salvataggio:\n";
        if (infile.is_open()) {
            std::string line;
            while (getline(infile, line)) {
                std::cout << line << std::endl;
            }
            infile.close();
        } else {
            std::cerr << "    Impossibile riaprire il file per verifica."
                      << std::endl;
        }
        std::cout << std::endl;
    } else {
        std::cerr << "    Errore durante il salvataggio del file.\n"
                  << std::endl;
    }

    std::cout << "12. Eliminazione del file '" << filename << "'..."
              << std::endl;
    if (std::remove(filename.c_str()) == 0) {
        std::cout << "    File eliminato con successo." << std::endl;
    } else {
        std::cerr << "    Errore nell'eliminazione del file." << std::endl;
    }

    std::cout << "\nDemo completata." << std::endl;
    return 0;
}