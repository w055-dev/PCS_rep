#include <catch2/catch_test_macros.hpp>
#include "Canvas.h"
#include "Exporters.h"
#include <filesystem>

TEST_CASE("Scenario: Export canvas to multiple formats", "[scenario]") {
    Canvas canvas(5, 5);
    
    // 1. Рисуем простую фигуру - домик
    // Стены
    for(int x = 1; x <= 3; ++x) {
        canvas.getActiveLayer()->setCell(x, 3, Cell('#', Color::White(), Color::Black()));
        canvas.getActiveLayer()->setCell(x, 4, Cell('#', Color::White(), Color::Black()));
    }
    // Крыша
    canvas.getActiveLayer()->setCell(2, 2, Cell('^', Color::Red(), Color::Black()));
    canvas.getActiveLayer()->setCell(1, 3, Cell('/', Color::Red(), Color::Black()));
    canvas.getActiveLayer()->setCell(3, 3, Cell('\\', Color::Red(), Color::Black()));
    // Дверь
    canvas.getActiveLayer()->setCell(2, 4, Cell('=', Color::Yellow(), Color::Black()));
    
    // 2. Экспорт в TXT
    TXTExporter txtExporter;
    std::string txtFile = "house.txt";
    REQUIRE(txtExporter.exportTo(&canvas, txtFile));
    REQUIRE(std::filesystem::exists(txtFile));
    REQUIRE(std::filesystem::file_size(txtFile) > 0);
    
    // 3. Экспорт в ANSI (с цветами)
    ANSIExporter ansiExporter;
    std::string ansiFile = "house.ansi";
    REQUIRE(ansiExporter.exportTo(&canvas, ansiFile));
    REQUIRE(std::filesystem::exists(ansiFile));
    
    // 4. Экспорт в CSV
    CSVExporter csvExporter;
    std::string csvFile = "house.csv";
    REQUIRE(csvExporter.exportTo(&canvas, csvFile));
    REQUIRE(std::filesystem::exists(csvFile));
    
    // 5. Экспорт в JSON
    JSONExporter jsonExporter;
    std::string jsonFile = "house.json";
    REQUIRE(jsonExporter.exportTo(&canvas, jsonFile));
    REQUIRE(std::filesystem::exists(jsonFile));
    
    // 6. Экспорт в ASCP (бинарный формат)
    ASCPExporter ascpExporter;
    std::string ascpFile = "house.ascp";
    REQUIRE(ascpExporter.exportTo(&canvas, ascpFile));
    REQUIRE(std::filesystem::exists(ascpFile));
    
    // 7. Очистка временных файлов
    std::filesystem::remove(txtFile);
    std::filesystem::remove(ansiFile);
    std::filesystem::remove(csvFile);
    std::filesystem::remove(jsonFile);
    std::filesystem::remove(ascpFile);
}