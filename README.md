# C++ INI Parser

Un semplice parser di file INI scritto in C++ per leggere, scrivere e manipolare file di configurazione. Sviluppato come progetto universitario.

## Caratteristiche principali

*   **Caricamento e salvataggio**: legge da file INI esistenti e salva le modifiche.
*   **Lettura/scrittura valori**: ottiene e imposta valori chiave-valore all'interno di sezioni specificate.
*   **Gestione sezioni e chiavi**: aggiunge ed elimina sezioni e chiavi.
*   **Supporto commenti ai parametri**:
    *   Legge e scrive commenti associati ai parametri (le righe di commento devono precedere il parametro a cui si riferiscono).
    *   Permette di aggiungere, ottenere ed eliminare commenti per i parametri programmaticamente.
*   **Ricerca**:
    *   Trova sezioni i cui nomi contengono una parola specifica.
    *   Trova le stringhe dei commenti dei parametri che contengono una parola specifica.
*   **Case-insensitive**: i nomi delle sezioni e delle chiavi vengono gestiti in modo case-insensitive (memorizzati e confrontati in minuscolo).

## Struttura del progetto

*   `IniParser.h`: file header con la definizione della classe `IniParser`.
*   `IniParser.cpp`: file sorgente con l'implementazione della classe `IniParser`.
*   `main.cpp`: esempio di utilizzo base della libreria.
*   `test/`: cartella contenente i test unitari (con Google Test).
