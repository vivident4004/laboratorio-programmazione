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

        std::cout << "\nTest delle funzioni hasKey:" << std::endl;
        // Test della prima versione (verifica in una sezione specifica)
        std::cout << "La chiave 'name' esiste nella sezione 'general': "
                  << (parser.hasKey("general", "name") ? "Sì" : "No") << std::endl;
        std::cout << "La chiave 'inesistente' esiste nella sezione 'general': "
                  << (parser.hasKey("general", "inesistente") ? "Sì" : "No") << std::endl;

        // Test della seconda versione (cerca in tutte le sezioni)
        std::cout << "\nRicerca della chiave 'host' in tutte le sezioni:" << std::endl;
        std::vector<std::string> sectionsWithHost = parser.hasKey("host");
        if (sectionsWithHost.empty()) {
            std::cout << "La chiave 'host' non è stata trovata in nessuna sezione." << std::endl;
        } else {
            std::cout << "La chiave 'host' è stata trovata nelle seguenti sezioni:" << std::endl;
            for (const auto& section : sectionsWithHost) {
                std::cout << "- " << section << std::endl;
            }
        }

        // Aggiungiamo una chiave duplicata in un'altra sezione per dimostrare la seconda funzione
        parser.setValue("backup", "host", "backup.server.com");
        std::cout << "\nDopo aver aggiunto 'host' in una seconda sezione:" << std::endl;
        std::vector<std::string> sectionsWithHostAfter = parser.hasKey("host");
        for (const auto& section : sectionsWithHostAfter) {
            std::cout << "- " << section << " (valore: " << parser.getValue(section, "host") << ")" << std::endl;
        }

        // Aggiungi una nuova sezione
        std::cout << "\nAggiunta di una nuova sezione..." << std::endl;
        parser.addSection("configurazione");
        std::cout << "La sezione 'configurazione' esiste: " << (parser.hasSection("configurazione") ? "Sì" : "No") << std::endl;

        // Stampa il file INI completo
        std::cout << "\nContenuto attuale del file INI:" << std::endl;
        std::cout << parser.toString();

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

        // Test della funzione deleteKey
        std::cout << "\nEliminazione di una chiave..." << std::endl;
        std::cout << "Prima dell'eliminazione, nuovachiave = " << parser.getValue("nuovasezione", "nuovachiave") << std::endl;
        if (parser.deleteKey("nuovasezione", "nuovachiave")) {
            std::cout << "Chiave 'nuovachiave' eliminata con successo!" << std::endl;
        } else {
            std::cout << "Errore nell'eliminazione della chiave 'nuovachiave'." << std::endl;
        }
        
        // Test della funzione deleteSection
        std::cout << "\nEliminazione di una sezione..." << std::endl;
        std::cout << "La sezione 'configurazione' esiste: " << (parser.hasSection("configurazione") ? "Sì" : "No") << std::endl;
        if (parser.deleteSection("configurazione")) {
            std::cout << "Sezione 'configurazione' eliminata con successo!" << std::endl;
        } else {
            std::cout << "Errore nell'eliminazione della sezione 'configurazione'." << std::endl;
        }
        
        // Stampa il file INI dopo le modifiche
        std::cout << "\nContenuto del file INI dopo le modifiche:" << std::endl;
        std::cout << parser.toString();

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
