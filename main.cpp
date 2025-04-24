//
// Created by vivident4004 on 10/04/2025.
//

#include <iostream>
#include <direct.h> // For chdir
#include "IniParser.h"

int main() {
    chdir("..");
    std::cout << "Parser di file INI - Versione base" << std::endl;
    std::string ini = "test.ini";

    // Carica un file INI di esempio
    if (IniParser parser; parser.load(ini)) {
        std::cout << "File caricato con successo!" << std::endl;

        // Leggi e stampa alcuni valori
        std::cout << "Nome applicazione: " << parser.getValue("general", "name") << std::endl;
        std::cout << "Versione: " << parser.getValue("general", "version") << std::endl;
        std::cout << "Host: " << parser.getValue("network", "host") << std::endl;
        std::cout << "Porta: " << parser.getValue("network", "port") << std::endl;
        std::cout << parser.print();

        // Prova a leggere un valore inesistente
        std::cout << "Valore inesistente (dovrebbe essere vuoto): '" << parser.getValue("nonexistent", "key") << "'" << std::endl;

        // Imposta un nuovo valore (aggiungerà la sezione e la chiave se non esistono)
        std::cout << "\nImpostazione di un nuovo valore..." << std::endl;
        parser.setValue("NuovaSezione", "NuovaChiave", "ValoreProva");
        std::cout << "Valore della nuova chiave: " << parser.getValue("nuovasezione", "nuovachiave") << std::endl;


        // Salva le modifiche sullo stesso file
        std::cout << "\nSalvataggio delle modifiche" << std::endl;
        parser.save(ini);
    } else {
        std::cout << "Errore nel caricamento del file." << std::endl;
    }

    return 0;
}
