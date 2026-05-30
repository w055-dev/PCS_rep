#include <catch2/catch_test_macros.hpp>
#include "Canvas.h"

TEST_CASE("Scenario: Basic drawing operations", "[scenario]") {
    Canvas c(10, 5);
    Layer* l = c.getActiveLayer();
    
    // Рисование в центре
    l->setCell(5, 2, Cell('#', Color::White(), Color::Black()));
    REQUIRE(l->getCell(5, 2).getSymbol() == '#');
    
    // Рисование на границах
    l->setCell(0, 0, Cell('+', Color::Cyan(), Color::Black()));
    l->setCell(9, 4, Cell('+', Color::Cyan(), Color::Black()));
    REQUIRE(l->getCell(0, 0).getSymbol() == '+');
    REQUIRE(l->getCell(9, 4).getSymbol() == '+');
    
    // Выход за границы
    l->setCell(15, 15, Cell('X', Color::Red(), Color::Black()));
    REQUIRE(l->getCell(15, 15).isEmpty());
    
    // Очистка
    l->clear();
    for(int y = 0; y < 5; ++y)
        for(int x = 0; x < 10; ++x)
            REQUIRE(l->getCell(x, y).isEmpty());
}