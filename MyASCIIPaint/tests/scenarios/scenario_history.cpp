#include <catch2/catch_test_macros.hpp>
#include "Canvas.h"
#include "HistoryManager.h"
#include "Commands.h"

TEST_CASE("Scenario: Undo/Redo chain", "[scenario]") {
    Canvas c(5, 5);
    auto* h = HistoryManager::getInstance();
    h->clear();
    
    // 3 действия
    auto* cmd1 = new PencilCommand(1, 1, Cell('A', Color::White(), Color::Black()));
    auto* cmd2 = new PencilCommand(2, 2, Cell('B', Color::Red(), Color::Black()));
    auto* cmd3 = new PencilCommand(3, 3, Cell('C', Color::Green(), Color::Black()));
    
    h->pushCommand(cmd1); cmd1->execute(&c);
    h->pushCommand(cmd2); cmd2->execute(&c);
    h->pushCommand(cmd3); cmd3->execute(&c);
    
    // Откат 2 шагов
    h->undo(&c);
    h->undo(&c);
    
    REQUIRE(c.getActiveLayer()->getCell(3, 3).isEmpty());
    REQUIRE(c.getActiveLayer()->getCell(2, 2).isEmpty());
    REQUIRE(c.getActiveLayer()->getCell(1, 1).getSymbol() == 'A');
    
    // Повтор 1 шага
    h->redo(&c);
    REQUIRE(c.getActiveLayer()->getCell(2, 2).getSymbol() == 'B');
    
    delete cmd1;
    delete cmd2;
    delete cmd3;
}