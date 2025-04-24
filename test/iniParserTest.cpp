#include "gtest/gtest.h" // Funzionerà grazie al linking con gtest_main (via FetchContent)
#include "../iniParser.h"   // Funzionerà grazie a target_include_directories(${CMAKE_SOURCE_DIR})

#include <fstream>
#include <cstdio> // Per std::remove

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
                 << "port=8086\n";
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

    parser.load("temp_test.ini");
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

TEST_F(IniParserTest, IgnoreLines) {
    IniParser parser;
    parser.load("temp_test.ini");

    EXPECT_EQ(parser.getValue("general", "name"), "TestApp");
    EXPECT_EQ(parser.getValue("network", "port"), "8086");
}

TEST_F(IniParserTest, HasSection) {
    IniParser parser;
    parser.load("temp_test.ini");

    EXPECT_TRUE(parser.hasSection("general"));
    EXPECT_TRUE(parser.hasSection("network"));
    EXPECT_FALSE(parser.hasSection("nonexistent"));

    // ase insensitivity
    EXPECT_TRUE(parser.hasSection("GENERAL"));
    EXPECT_TRUE(parser.hasSection("Network"));
}

TEST_F(IniParserTest, AddSection) {
    IniParser parser;
    parser.load("temp_test.ini");

    EXPECT_FALSE(parser.hasSection("newsection"));
    parser.addSection("newsection");
    EXPECT_TRUE(parser.hasSection("newsection"));

    parser.addSection("general");
    EXPECT_TRUE(parser.hasSection("general"));
}

TEST_F(IniParserTest, PrintContent) {
    IniParser parser;
    parser.setValue("section1", "key1", "value1");
    parser.setValue("section1", "key2", "value2");

    std::string output = parser.print();
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

    // Verifica che i contenuti corrispondano
    EXPECT_EQ(parser2.getValue("general", "newkey"), "newvalue");
    EXPECT_EQ(parser2.getValue("custom", "setting"), "value");
    EXPECT_EQ(parser2.getValue("general", "name"), "TestApp");

    // Pulizia
    std::remove("temp_test_save.ini");
}

// Test per hasKey (prima versione)
TEST_F(IniParserTest, HasKeyInSection) {
    IniParser parser;
    parser.load("temp_test.ini");

    EXPECT_TRUE(parser.hasKey("general", "name"));
    EXPECT_TRUE(parser.hasKey("network", "host"));
    EXPECT_FALSE(parser.hasKey("general", "nonexistent"));
    EXPECT_FALSE(parser.hasKey("nonexistent", "key"));

    // Test case insensitivity
    EXPECT_TRUE(parser.hasKey("GENERAL", "NAME"));
}

// Test per hasKey (seconda versione)
TEST_F(IniParserTest, FindKeyInAllSections) {
    IniParser parser;
    parser.load("temp_test.ini");

    // Chiave esistente in una sezione
    std::vector<std::string> sectionsWithHost = parser.hasKey("host");
    EXPECT_EQ(sectionsWithHost.size(), 1);
    EXPECT_EQ(sectionsWithHost[0], "network");

    // Chiave inesistente
    std::vector<std::string> sectionsWithNonexistent = parser.hasKey("nonexistent");
    EXPECT_TRUE(sectionsWithNonexistent.empty());

    // Aggiungiamo la stessa chiave in un'altra sezione
    parser.setValue("backup", "host", "backup.server");
    std::vector<std::string> sectionsWithHostAfter = parser.hasKey("host");
    EXPECT_EQ(sectionsWithHostAfter.size(), 2);

    // Verifica case insensitivity
    std::vector<std::string> sectionsWithHostUpper = parser.hasKey("HOST");
    EXPECT_EQ(sectionsWithHostUpper.size(), 2);
}
