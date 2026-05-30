#include <catch2/catch_test_macros.hpp>
#include "Canvas.h"

TEST_CASE("Scenario: Multi-layer composition", "[scenario]") {
    Canvas canvas(10, 10);
    
    // Слой 0: небо
    canvas.getActiveLayer()->setName("Sky");
    for(int x = 0; x < 10; ++x)
        canvas.getActiveLayer()->setCell(x, 0, Cell('.', Color::Cyan(), Color::Black()));
    
    // Слой 1: земля
    canvas.addLayer();
    canvas.setActiveLayer(1);
    canvas.getActiveLayer()->setName("Ground");
    for(int x = 0; x < 10; ++x)
        canvas.getActiveLayer()->setCell(x, 8, Cell('_', Color::Green(), Color::Black()));
    
    // Слой 2: солнце
    canvas.addLayer();
    canvas.setActiveLayer(2);
    canvas.getActiveLayer()->setName("Sun");
    canvas.getActiveLayer()->setCell(8, 1, Cell('O', Color::Yellow(), Color::Black()));
    
    // Проверка независимости слоёв
    REQUIRE(canvas.getLayer(0)->getCell(5, 0).getSymbol() == '.');
    REQUIRE(canvas.getLayer(1)->getCell(5, 8).getSymbol() == '_');
    REQUIRE(canvas.getLayer(2)->getCell(8, 1).getSymbol() == 'O');
    
    // Скрытие слоя
    canvas.getLayer(1)->setVisible(false);
    REQUIRE(canvas.getLayer(1)->isVisible() == false);
    
    // Удаление слоя
    canvas.removeLayer(2);
    REQUIRE(canvas.getLayerCount() == 2);
    
    // Переименование
    canvas.setActiveLayer(0);
    canvas.getActiveLayer()->setName("Background");
    REQUIRE(canvas.getActiveLayer()->getName() == "Background");
}