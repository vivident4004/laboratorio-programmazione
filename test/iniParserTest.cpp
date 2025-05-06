#include "gtest/gtest.h"
#include "../iniParser.h"
#include <fstream>

// Prepara un file INI temporaneo per i test
class IniParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::ofstream testFile("temp_test.ini");
        testFile << "[general]\n"
                 << "name=TestApp\n"
                 << "version=1.0\n"
                 << "\n"
                 << "; commento\n"
                 << "[network]\n"
                 << "host=localhost\n"
                 << "port=8086\n"
                << "\n"
                << "[Database_Primary]\n"
                << "user=db_user\n"
                << "\n"
                << "[DATABASE_Secondary]\n"
                << "user=db_user_secondary\n"
                << "user=admin ; User ID\n"
                << "pass=password\n"
                << "mode=test ; Operation mode\n";
        testFile.close();
    }

    void TearDown() override {
        std::remove("temp_test.ini");
    }
};

TEST_F(IniParserTest, LoadFile) {
    IniParser parser;
    EXPECT_TRUE(parser.load("temp_test.ini"));
    EXPECT_FALSE(parser.load("non_existent_file.ini"));
}

TEST_F(IniParserTest, ReadValues) {
    IniParser parser;
    parser.load("temp_test.ini");

    EXPECT_EQ(parser.getValue("general", "name"), "TestApp");
    EXPECT_EQ(parser.getValue("general", "version"), "1.0");
    EXPECT_EQ(parser.getValue("network", "host"), "localhost");
    EXPECT_EQ(parser.getValue("network", "port"), "8086"); // Corretto

    EXPECT_EQ(parser.getValue("nonexistent", "key"), "");
    EXPECT_EQ(parser.getValue("general", "nonexistent"), "");
}

TEST_F(IniParserTest, SetValues) {
    IniParser parser;

    parser.setValue("section", "key", "value");
    EXPECT_EQ(parser.getValue("section", "key"), "value");

    parser.load("temp_test.ini"); // Ricarica per sovrascrivere
    parser.setValue("general", "name", "NuovoNome");
    EXPECT_EQ(parser.getValue("general", "name"), "NuovoNome");
    EXPECT_EQ(parser.getValue("general", "version"), "1.0");

    parser.setValue("nuova_sezione", "nuova_chiave", "nuovo_valore");
    EXPECT_EQ(parser.getValue("nuova_sezione", "nuova_chiave"), "nuovo_valore");
}

TEST_F(IniParserTest, NonExistentValues) {
    IniParser parser;
    parser.load("temp_test.ini");

    EXPECT_EQ(parser.getValue("nonexistent", "key"), "");
    EXPECT_EQ(parser.getValue("general", "nonexistent"), "");
}

TEST_F(IniParserTest, HasSection) {
    IniParser parser;
    parser.load("temp_test.ini");

    EXPECT_TRUE(parser.hasSection("general"));
    EXPECT_TRUE(parser.hasSection("network"));
    EXPECT_FALSE(parser.hasSection("nonexistent"));

    // Case insensitivity
    EXPECT_TRUE(parser.hasSection("GENERAL"));
    EXPECT_TRUE(parser.hasSection("Network"));
}

TEST_F(IniParserTest, AddSection) {
    IniParser parser;
    parser.load("temp_test.ini");

    EXPECT_FALSE(parser.hasSection("newsection"));
    parser.addSection("NewSection");
    EXPECT_TRUE(parser.hasSection("newsection")); // Verifica con lowercase
    EXPECT_TRUE(parser.hasSection("NewSection")); // Verifica con case originale
}

TEST_F(IniParserTest, ToString) {
    IniParser parser;
    parser.setValue("section1", "key1", "value1");
    parser.setValue("section1", "key2", "value2");

    std::string output = parser.toString();
    EXPECT_TRUE(output.find("[section1]") != std::string::npos);
    EXPECT_TRUE(output.find("key1=value1") != std::string::npos);
    EXPECT_TRUE(output.find("key2=value2") != std::string::npos);
}

// Test per save e ricaricamento
TEST_F(IniParserTest, SaveAndReload) {
    IniParser parser1;
    parser1.load("temp_test.ini");
    parser1.setValue("general", "newkey", "newvalue");
    parser1.addSection("custom");
    parser1.setValue("custom", "setting", "value");

    // Salva su un nuovo file
    EXPECT_TRUE(parser1.save("temp_test_save.ini"));

    // Carica il file salvato in un nuovo parser
    IniParser parser2;
    EXPECT_TRUE(parser2.load("temp_test_save.ini"));

    // I nomi delle sezioni e delle chiavi vengono salvati e letti in lowercase
    EXPECT_EQ(parser2.getValue("general", "newkey"), "newvalue");
    EXPECT_EQ(parser2.getValue("custom", "setting"), "value");
    EXPECT_EQ(parser2.getValue("general", "name"), "TestApp");

    std::remove("temp_test_save.ini");
}

// Test per hasKey (prima versione)
TEST_F(IniParserTest, HasKeyInSection) {
    IniParser parser;
    parser.load("temp_test.ini");

    EXPECT_TRUE(parser.hasKey("general", "name"));
    EXPECT_TRUE(parser.hasKey("NETWORK", "HOST"));
    EXPECT_FALSE(parser.hasKey("general", "nonexistent"));
    EXPECT_FALSE(parser.hasKey("nonexistent", "key"));
}

// Test per hasKey (seconda versione)
TEST_F(IniParserTest, FindKeyInAllSections) {
    IniParser parser;
    parser.load("temp_test.ini");

    // Chiave esistente in una sezione
    const std::vector<std::string> sectionsWithHost = parser.hasKey("host");
    ASSERT_EQ(sectionsWithHost.size(), 1);
    EXPECT_EQ(sectionsWithHost[0], "network");

    // Chiave inesistente
    const std::vector<std::string> sectionsWithNonexistent = parser.hasKey("nonexistent");
    EXPECT_TRUE(sectionsWithNonexistent.empty());

    // Aggiungo la stessa chiave in un'altra sezione
    parser.setValue("backup", "host", "backup.server");
    const std::vector<std::string> sectionsWithHostAfter = parser.hasKey("host");
    ASSERT_EQ(sectionsWithHostAfter.size(), 2);

    // Verifica case insensitivity
    const std::vector<std::string> sectionsWithHostUpper = parser.hasKey("HOST");
    EXPECT_EQ(sectionsWithHostUpper.size(), 2);
}

TEST_F(IniParserTest, DeleteKey) {
    IniParser parser;
    parser.load("temp_test.ini");

    EXPECT_TRUE(parser.hasKey("general", "name"));
    EXPECT_TRUE(parser.deleteKey("general", "name"));
    EXPECT_FALSE(parser.hasKey("general", "name"));
    EXPECT_TRUE(parser.hasKey("general", "version")); // Altra chiave ancora presente

    EXPECT_FALSE(parser.deleteKey("general", "nonexistent"));
    EXPECT_FALSE(parser.deleteKey("nonexistent", "key"));

    parser.setValue("test", "CaseSensitive", "value");
    EXPECT_TRUE(parser.deleteKey("TEST", "casesensitive"));
}

TEST_F(IniParserTest, DeleteSection) {
    IniParser parser;
    parser.load("temp_test.ini");

    EXPECT_TRUE(parser.hasSection("general"));
    EXPECT_TRUE(parser.deleteSection("general"));
    EXPECT_FALSE(parser.hasSection("general"));
    EXPECT_TRUE(parser.hasSection("network")); // Altra sezione ancora presente

    parser.addSection("TestDeleteSection");
    EXPECT_TRUE(parser.hasSection("testdeletesection"));
    EXPECT_TRUE(parser.deleteSection("TESTDELETESECTION"));
    EXPECT_FALSE(parser.hasSection("TestDeleteSection"));
}

TEST_F(IniParserTest, FindSectionContainingWord) {
    IniParser parser;
    parser.load("temp_test.ini");

    // 1. Parola presente in una sezione (match parziale, case-insensitive)
    std::vector<std::string> result1 = parser.findSectionsContainingWord("gen");
    ASSERT_EQ(result1.size(), 1);
    EXPECT_EQ(result1[0], "general");

    // 2. Parola presente in più sezioni (match esatto e parziale, case-insensitive)
    std::vector<std::string> result2 = parser.findSectionsContainingWord("database");
    ASSERT_EQ(result2.size(), 2);
    std::sort(result2.begin(), result2.end()); // Ordina per confronto predicibile
    EXPECT_EQ(result2[0], "database_primary");
    EXPECT_EQ(result2[1], "database_secondary");

    // 3. Parola non presente
    std::vector<std::string> result3 = parser.findSectionsContainingWord("nonexistent");
    EXPECT_TRUE(result3.empty());

    // 4. Parola vuota (dovrebbe restituire un vettore vuoto)
    std::vector<std::string> result4 = parser.findSectionsContainingWord("");
    EXPECT_TRUE(result4.empty());

    // 5. Case-insensitivity
    std::vector<std::string> result5 = parser.findSectionsContainingWord("NETWORK");
    ASSERT_EQ(result5.size(), 1);
    EXPECT_EQ(result5[0], "network");
}

TEST_F(IniParserTest, AddGetDeleteComment) {
    IniParser parser;
    parser.load("temp_test.ini");

    // La chiave esiste, il commento no
    EXPECT_EQ(parser.getCommentFromParam("general", "name"), "");

    // Aggiungi un commento
    parser.addCommentToParam("general", "name", "This is a name");
    EXPECT_EQ(parser.getCommentFromParam("general", "name"), "This is a name");

    // Prova ad aggiungere un commento a una chiave/sezione non esistente (non dovrebbe fare nulla o stampare errore)
    parser.addCommentToParam("general", "nonexistentkey", "test");
    EXPECT_EQ(parser.getCommentFromParam("general", "nonexistentkey"), "");
    parser.addCommentToParam("nonexistentsection", "name", "test");
    EXPECT_EQ(parser.getCommentFromParam("nonexistentsection", "name"), "");

    // Sovrascrivi commento
    parser.addCommentToParam("general", "name", "Updated name comment");
    EXPECT_EQ(parser.getCommentFromParam("general", "name"), "Updated name comment");

    // Elimina commento
    EXPECT_TRUE(parser.deleteCommentFromParam("general", "name"));
    EXPECT_EQ(parser.getCommentFromParam("general", "name"), "");

    // Elimina commento non esistente
    EXPECT_FALSE(parser.deleteCommentFromParam("general", "name"));
    EXPECT_FALSE(parser.deleteCommentFromParam("general", "nonexistentkey"));
    EXPECT_FALSE(parser.deleteCommentFromParam("nonexistentsection", "key"));
}

// Verifica toString() per un parser vuoto
TEST_F(IniParserTest, ToStringEmptyParser) {
    IniParser parser;
    EXPECT_EQ(parser.toString(), "");
}

// Verifica toString() con dati e commenti
TEST_F(IniParserTest, ToStringProgrammaticDataWithComments) {
    IniParser parser;
    parser.setValue("Settings", "Theme", "Dark");
    // Assumendo che addCommentToParam funzioni come previsto
    parser.addCommentToParam("Settings", "Theme", "User preferred color theme");
    parser.setValue("Settings", "FontSize", "12");
    parser.addCommentToParam("settings", "fontsize", "Font size in points"); // Case-insensitive

    std::string expectedOutput =
        "[settings]\n"
        "; Font size in points\n" // I commenti vengono prima della chiave
        "fontsize=12\n"
        "; User preferred color theme\n"
        "theme=Dark\n"
        "\n";

    EXPECT_EQ(parser.toString(), expectedOutput);
}

TEST_F(IniParserTest, AddCommentToNonExistentKeyOrSection) {
    IniParser parser;
    // Tentativo di aggiungere un commento a una chiave/sezione non esistente non dovrebbe avere effetto (o segnalare errore)
    parser.addCommentToParam("nonExistentSection", "nonExistentKey", "A comment");
    ASSERT_EQ(parser.getCommentFromParam("nonExistentSection", "nonExistentKey"), "");

    parser.addSection("existingSection");
    parser.addCommentToParam("existingSection", "nonExistentKeyInExistingSection", "Another comment");
    ASSERT_EQ(parser.getCommentFromParam("existingSection", "nonExistentKeyInExistingSection"), "");
}

TEST_F(IniParserTest, DeleteKeyAlsoDeletesComment) {
    IniParser parser;
    parser.setValue("sectionForDelete", "keyToDelete", "value");
    parser.addCommentToParam("sectionForDelete", "keyToDelete", "Comment for keyToDelete");
    ASSERT_TRUE(parser.deleteKey("sectionForDelete", "keyToDelete"));
    ASSERT_FALSE(parser.hasKey("sectionForDelete", "keyToDelete"));
    ASSERT_EQ(parser.getCommentFromParam("sectionForDelete", "keyToDelete"), ""); // Il commento dovrebbe essere sparito
}

TEST_F(IniParserTest, DeleteSectionAlsoDeletesComments) {
    IniParser parser;
    parser.addSection("sectionWithComments");
    parser.setValue("sectionWithComments", "key1", "value1");
    parser.addCommentToParam("sectionWithComments", "key1", "Comment for key1 in sectionWithComments");
    parser.setValue("sectionWithComments", "key2", "value2");
    parser.addCommentToParam("sectionWithComments", "key2", "Comment for key2 in sectionWithComments");

    ASSERT_TRUE(parser.deleteSection("sectionWithComments"));
    ASSERT_FALSE(parser.hasSection("sectionWithComments"));
    ASSERT_EQ(parser.getCommentFromParam("sectionWithComments", "key1"), ""); // Commento sparito
    ASSERT_EQ(parser.getCommentFromParam("sectionWithComments", "key2"), ""); // Commento sparito
}

TEST_F(IniParserTest, MultiLineCommentsLoadAndSave) {
    const std::string multiLineFilename = "temp_multiline_test.ini";
    // File con commenti multiriga
    {
        std::ofstream testFile(multiLineFilename);
        testFile << "[section1]\n"
                 << "; This is the first line of the comment.\n" // Commento per key1
                 << "; This is the second line.\n"               // Commento per key1
                 << "# And a third line with a different char.\n"// Commento per key1
                 << "key1=value1\n"
                 << "\n" // Una linea vuota opzionale
                 << "; Single line comment for key2\n"           // Commento per key2
                 << "key2=value2 ; inline comment, part of value\n"
                 << "[section2]\n"
                 << "key3=value3\n";
        testFile.close();
    }

    IniParser parser1;
    ASSERT_TRUE(parser1.load(multiLineFilename));

    // Verifica la lettura del commento multi-riga
    std::string expectedCommentForKey1 =
        "This is the first line of the comment.\n"
        "This is the second line.\n"
        "And a third line with a different char.";
    EXPECT_EQ(parser1.getCommentFromParam("section1", "key1"),
                expectedCommentForKey1);

    // Verifica la lettura del commento a riga singola per key2
    EXPECT_EQ(parser1.getCommentFromParam("section1", "key2"),
                "Single line comment for key2");

    // Salva e ricarica
    const std::string savedMultiLineFilename = "temp_multiline_saved.ini";
    ASSERT_TRUE(parser1.save(savedMultiLineFilename));

    IniParser parser2;
    ASSERT_TRUE(parser2.load(savedMultiLineFilename));
    // Verifica che i commenti siano preservati dopo il salvataggio e ricaricamento
    EXPECT_EQ(parser2.getCommentFromParam("section1", "key1"),
                expectedCommentForKey1);
    EXPECT_EQ(parser2.getCommentFromParam("section1", "key2"), // Verifica anche qui
                "Single line comment for key2");
    EXPECT_EQ(parser2.getValue("section1", "key1"), "value1");
    EXPECT_EQ(parser2.getValue("section1", "key2"),
                "value2 ; inline comment, part of value");

    // Verifica toString() con commenti multi-riga
    //    L'ordine delle chiavi in una sezione è alfabetico a causa di std::map
    //    L'uscita attesa per toString deve riflettere la nuova struttura del file
    std::string expectedToStringOutput =
        "[section1]\n"
        "; This is the first line of the comment.\n"
        "; This is the second line.\n"
        "; And a third line with a different char.\n"
        "key1=value1\n"
        "; Single line comment for key2\n"
        "key2=value2 ; inline comment, part of value\n"
        "\n"
        "[section2]\n"
        "key3=value3\n"
        "\n";

    EXPECT_EQ(parser2.toString(), expectedToStringOutput);

    std::remove(multiLineFilename.c_str());
    std::remove(savedMultiLineFilename.c_str());
}

TEST_F(IniParserTest, FindAllCommentStringsContainingWord) {
    IniParser parser;
    parser.addSection("general");
    parser.setValue("general", "name", "TestApp");
    parser.addCommentToParam("general", "name", "Application Name - IMPORTANT"); // Commento 1
    parser.setValue("general", "version", "1.0");
    parser.addCommentToParam("general", "version", "Version (critical for updates)"); // Commento 2

    parser.addSection("network");
    parser.setValue("network", "host", "localhost");
    parser.addCommentToParam("network", "host", "Server address (important for connection)"); // Commento 3
    parser.setValue("network", "port", "8080");

    // parola "important" (case-insensitive)
    std::vector<std::string> resultsImportant =
        parser.findCommentStringsContainingWord("important");
    ASSERT_EQ(resultsImportant.size(), 2); // Verifica che ci siano due risultati

    // Verifica la presenza di ciascun commento atteso, indipendentemente dall'ordine
    bool foundComment1 = false;
    bool foundComment3 = false;
    for (const std::string& comment : resultsImportant) {
        if (comment == "Application Name - IMPORTANT") {
            foundComment1 = true;
        } else if (comment == "Server address (important for connection)") {
            foundComment3 = true;
        }
    }
    EXPECT_TRUE(foundComment1);
    EXPECT_TRUE(foundComment3);

    // Parola "critical"
    std::vector<std::string> resultsCritical =
        parser.findCommentStringsContainingWord("critical");
    ASSERT_EQ(resultsCritical.size(), 1);
    EXPECT_EQ(resultsCritical[0], "Version (critical for updates)");

    // Caso 3: Parola non trovata
    std::vector<std::string> resultsNonExistent =
        parser.findCommentStringsContainingWord("nonexistentword");
    EXPECT_TRUE(resultsNonExistent.empty());

    // Caso 4: Parola vuota
    std::vector<std::string> resultsEmptyWord =
        parser.findCommentStringsContainingWord("");
    EXPECT_TRUE(resultsEmptyWord.empty());

    // Caso 5: Parser senza commenti
    IniParser emptyParser;
    emptyParser.addSection("test");
    emptyParser.setValue("test", "key", "value");
    std::vector<std::string> resultsNoComments =
        emptyParser.findCommentStringsContainingWord("any");
    EXPECT_TRUE(resultsNoComments.empty());
}