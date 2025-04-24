//
// Created by vivident4004 on 10/04/2025.
//

#include <iostream>
#include <direct.h>
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

        // Verifica se una sezione esiste
        std::cout << "\nControllo delle sezioni:" << std::endl;
        std::cout << "La sezione 'general' esiste: " << (parser.hasSection("general") ? "Sì" : "No") << std::endl;
        std::cout << "La sezione 'inesistente' esiste: " << (parser.hasSection("inesistente") ? "Sì" : "No") << std::endl;

        // Aggiungi una nuova sezione
        std::cout << "\nAggiunta di una nuova sezione..." << std::endl;
        parser.addSection("configurazione");
        std::cout << "La sezione 'configurazione' esiste: " << (parser.hasSection("configurazione") ? "Sì" : "No") << std::endl;

        // Stampa il file INI completo
        std::cout << "\nContenuto attuale del file INI:" << std::endl;
        std::cout << parser.print();

        // Prova a leggere un valore inesistente
        std::cout << "\nValore inesistente (dovrebbe essere vuoto): '" << parser.getValue("nonexistent", "key") << "'" << std::endl;

        // Imposta un nuovo valore (aggiungerà la sezione e la chiave se non esistono)
        std::cout << "\nImpostazione di un nuovo valore..." << std::endl;
        parser.setValue("NuovaSezione", "NuovaChiave", "ValoreProva");
        std::cout << "Valore della nuova chiave: " << parser.getValue("nuovasezione", "nuovachiave") << std::endl;

        // Modifica un valore esistente
        std::cout << "\nModifica di un valore esistente..." << std::endl;
        parser.setValue("general", "name", "NomeModificato");
        std::cout << "Nuovo valore di 'name': " << parser.getValue("general", "name") << std::endl;

        // Stampa il file INI dopo le modifiche
        std::cout << "\nContenuto del file INI dopo le modifiche:" << std::endl;
        std::cout << parser.print();

        // Salva le modifiche sullo stesso file
        std::cout << "\nSalvataggio delle modifiche" << std::endl;
        if (parser.save(ini)) {
            std::cout << "File salvato con successo!" << std::endl;
        } else {
            std::cout << "Errore durante il salvataggio del file." << std::endl;
        }
    } else {
        std::cout << "Errore nel caricamento del file." << std::endl;
    }

    return 0;
}
