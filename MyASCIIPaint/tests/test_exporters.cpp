#include <catch2/catch_test_macros.hpp>
#include "Exporters.h"
#include "Canvas.h"
#include <filesystem>
#include <fstream>

TEST_CASE("TXTExporter exports to file", "[exporters]") {
    Canvas c(3, 3);
    c.getActiveLayer()->setCell(0, 0, Cell('A', Color::Red(), Color::Black()));
    TXTExporter exporter;
    std::string filename = "test.txt";
    
    bool result = exporter.exportTo(&c, filename);
    REQUIRE(result == true);
    REQUIRE(std::filesystem::exists(filename));
    std::filesystem::remove(filename);
}

TEST_CASE("ANSIExporter exports without crashing", "[exporters]") {
    Canvas c(3, 3);
    c.getActiveLayer()->setCell(0, 0, Cell('R', Color::Red(), Color::Black()));
    ANSIExporter exporter;
    std::string filename = "test.ansi";
    
    bool result = exporter.exportTo(&c, filename);
    REQUIRE(result == true);
    REQUIRE(std::filesystem::exists(filename));
    REQUIRE(std::filesystem::file_size(filename) > 0);
    std::filesystem::remove(filename);
}

TEST_CASE("CSVExporter exports to CSV", "[exporters]") {
    Canvas c(2, 2);
    CSVExporter exporter;
    std::string filename = "test.csv";
    
    bool result = exporter.exportTo(&c, filename);
    REQUIRE(result == true);
    REQUIRE(std::filesystem::exists(filename));
    REQUIRE(std::filesystem::file_size(filename) > 0);
    std::filesystem::remove(filename);
}

TEST_CASE("JSONExporter exports to JSON", "[exporters]") {
    Canvas c(2, 2);
    JSONExporter exporter;
    std::string filename = "test.json";
    
    bool result = exporter.exportTo(&c, filename);
    REQUIRE(result == true);
    REQUIRE(std::filesystem::exists(filename));
    REQUIRE(std::filesystem::file_size(filename) > 0);
    std::filesystem::remove(filename);
}

TEST_CASE("ASCPExporter exports binary format", "[exporters]") {
    Canvas c(5, 5);
    ASCPExporter exporter;
    std::string filename = "test.ascp";
    
    bool result = exporter.exportTo(&c, filename);
    REQUIRE(result == true);
    REQUIRE(std::filesystem::exists(filename));
    REQUIRE(std::filesystem::file_size(filename) > 0);
    std::filesystem::remove(filename);
}

TEST_CASE("ASCPImporter validates files", "[exporters]") {
    Canvas c(2, 2);
    ASCPExporter exporter;
    std::string filename = "test.ascp";
    
    exporter.exportTo(&c, filename);
    REQUIRE(std::filesystem::exists(filename));
    CHECK_NOTHROW(ASCPImporter::validate(filename));
    std::filesystem::remove(filename);
}

TEST_CASE("All exporters have format names", "[exporters]") {
    TXTExporter txt;
    ANSIExporter ansi;
    CSVExporter csv;
    JSONExporter json;
    ASCPExporter ascp;
    
    REQUIRE(txt.getFormatName() == "TXT");
    REQUIRE(ansi.getFormatName() == "ANSI");
    REQUIRE(csv.getFormatName() == "CSV");
    REQUIRE(json.getFormatName() == "JSON");
    REQUIRE(ascp.getFormatName() == "ASCP");
}