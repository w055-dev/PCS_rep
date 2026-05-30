#include <catch2/catch_test_macros.hpp>
#include "Tools.h"
#include "Canvas.h"

TEST_CASE("PencilTool creates command", "[tools]") {
    Canvas c(10, 10);
    PencilTool tool;
    tool.setSymbol('@');
    tool.setColor(Color::Magenta(), Color::Black());
    Command* cmd = tool.createCommand(&c, 5, 5, 5, 5);
    cmd->execute(&c);
    REQUIRE(c.getActiveLayer()->getCell(5, 5).getSymbol() == '@');
    delete cmd;
}

TEST_CASE("EraserTool clears cells", "[tools]") {
    Canvas c(10, 10);
    c.getActiveLayer()->setCell(7, 7, Cell('X', Color::White(), Color::Black()));
    EraserTool tool;
    Command* cmd = tool.createCommand(&c, 7, 7, 7, 7);
    cmd->execute(&c);
    REQUIRE(c.getActiveLayer()->getCell(7, 7).isEmpty());
    delete cmd;
}

TEST_CASE("LineTool draws line", "[tools]") {
    Canvas c(10, 10);
    LineTool tool;
    tool.setSymbol('=');
    tool.setColor(Color::Cyan(), Color::Black());
    Command* cmd = tool.createCommand(&c, 1, 1, 5, 5);
    cmd->execute(&c);
    REQUIRE(c.getActiveLayer()->getCell(3, 3).getSymbol() == '=');
    delete cmd;
}

TEST_CASE("RectangleTool hollow", "[tools]") {
    Canvas c(10, 10);
    RectangleTool tool(false);
    tool.setSymbol('*');
    tool.setColor(Color::Green(), Color::Black());
    Command* cmd = tool.createCommand(&c, 2, 2, 5, 5);
    cmd->execute(&c);
    REQUIRE(c.getActiveLayer()->getCell(2, 2).getSymbol() == '*');
    delete cmd;
}

TEST_CASE("RectangleTool filled", "[tools]") {
    Canvas c(10, 10);
    RectangleTool tool(true);
    tool.setSymbol('F');
    tool.setColor(Color::Blue(), Color::Black());
    Command* cmd = tool.createCommand(&c, 2, 2, 4, 4);
    cmd->execute(&c);
    REQUIRE(c.getActiveLayer()->getCell(3, 3).getSymbol() == 'F');
    delete cmd;
}

TEST_CASE("FloodFillTool fills area", "[tools]") {
    Canvas c(10, 10);
    for(int x=0; x<10; x++) {
        c.getActiveLayer()->setCell(x, 0, Cell('#', Color::White(), Color::Black()));
    }
    FloodFillTool tool;
    tool.setSymbol('*');
    tool.setColor(Color::Red(), Color::Black());
    Command* cmd = tool.createCommand(&c, 5, 5, 5, 5);
    cmd->execute(&c);
    REQUIRE(c.getActiveLayer()->getCell(5, 5).getSymbol() == '*');
    delete cmd;
}

TEST_CASE("Tool color and symbol setting", "[tools]") {
    PencilTool tool;
    tool.setSymbol('#');
    tool.setColor(Color::Red(), Color::Blue());
    REQUIRE(tool.getCurrentSymbol().getSymbol() == '#');
    REQUIRE(tool.getCurrentSymbol().getForeground() == Color::Red());
}