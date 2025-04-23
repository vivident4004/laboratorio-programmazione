#include "googletest/googletest/include/gtest/gtest.h"
#include "IniParser.h"
#include <fstream>
#include <string>
#include <iostream>

// Helper per creare file temporaneo
void createTestFile(const std::string& filename) {
    std::ofstream file(filename);
    file << "[general]\n"
         << "name=TestApp\n"
         << "version=1.0\n"
         << "[network]\n"
         << "host=localhost\n"
         << "port=8086\n";
    file.close();

    std::cout << "File di test creato: " << filename << std::endl;
}

// Test di base
TEST(IniParserTest, LoadFile) {
    const std::string testFilename = "test_simple.ini";
    createTestFile(testFilename);

    IniParser parser;
    EXPECT_TRUE(parser.loadFile(testFilename));

    std::remove(testFilename.c_str());
}

TEST(IniParserTest, ReadValues) {
    const std::string testFilename = "test_read.ini";
    createTestFile(testFilename);

    IniParser parser;
    parser.loadFile(testFilename);

    EXPECT_EQ(parser.getValue("general", "name"), "TestApp");
    EXPECT_EQ(parser.getValue("general", "version"), "1.0");
    EXPECT_EQ(parser.getValue("network", "host"), "localhost");
    EXPECT_EQ(parser.getValue("network", "port"), "8086");

    std::remove(testFilename.c_str());
}

TEST(IniParserTest, SetValues) {
    IniParser parser;

    parser.setValue("section", "key", "value");
    EXPECT_EQ(parser.getValue("section", "key"), "value");
}

TEST(IniParserTest, NonExistentValues) {
    IniParser parser;

    EXPECT_EQ(parser.getValue("nonexistent", "key"), "");
    EXPECT_EQ(parser.getValue("section", "nonexistent"), "");
}
