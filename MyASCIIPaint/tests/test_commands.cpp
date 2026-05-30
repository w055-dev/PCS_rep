#include <catch2/catch_test_macros.hpp>
#include "Commands.h"
#include "Canvas.h"

TEST_CASE("PencilCommand execute/undo", "[commands]") {
    Canvas c(10, 10);
    PencilCommand cmd(5, 5, Cell('X', Color::Red(), Color::Black()));
    cmd.execute(&c);
    REQUIRE(c.getActiveLayer()->getCell(5, 5).getSymbol() == 'X');
    cmd.undo(&c);
    REQUIRE(c.getActiveLayer()->getCell(5, 5).isEmpty());
}

TEST_CASE("LineCommand draws line", "[commands]") {
    Canvas c(10, 10);
    LineCommand cmd(1, 1, 5, 5, Cell('\\', Color::Yellow(), Color::Black()));
    cmd.execute(&c);
    REQUIRE(c.getActiveLayer()->getCell(3, 3).getSymbol() == '\\');
    cmd.undo(&c);
    REQUIRE(c.getActiveLayer()->getCell(3, 3).isEmpty());
}

TEST_CASE("RectangleCommand hollow", "[commands]") {
    Canvas c(10, 10);
    RectangleCommand cmd(2, 2, 5, 5, Cell('*', Color::Green(), Color::Black()), false);
    cmd.execute(&c);
    REQUIRE(c.getActiveLayer()->getCell(2, 2).getSymbol() == '*');
    REQUIRE(c.getActiveLayer()->getCell(3, 3).isEmpty());
}

TEST_CASE("RectangleCommand filled", "[commands]") {
    Canvas c(10, 10);
    RectangleCommand cmd(2, 2, 4, 4, Cell('F', Color::Blue(), Color::Black()), true);
    cmd.execute(&c);
    REQUIRE(c.getActiveLayer()->getCell(3, 3).getSymbol() == 'F');
}

TEST_CASE("FloodFillCommand fills area", "[commands]") {
    Canvas c(10, 10);
    for(int x=0; x<10; x++) {
        c.getActiveLayer()->setCell(x, 0, Cell('#', Color::White(), Color::Black()));
        c.getActiveLayer()->setCell(x, 9, Cell('#', Color::White(), Color::Black()));
    }
    FloodFillCommand cmd(5, 5, Cell('*', Color::Red(), Color::Black()));
    cmd.execute(&c);
    REQUIRE(c.getActiveLayer()->getCell(5, 5).getSymbol() == '*');
    REQUIRE(c.getActiveLayer()->getCell(0, 0).getSymbol() == '#');
}

TEST_CASE("EllipseCommand draws ellipse", "[commands]") {
    Canvas c(20, 20);
    EllipseCommand cmd(5, 5, 15, 15, Cell('O', Color::Magenta(), Color::Black()));
    cmd.execute(&c);
    REQUIRE(c.getActiveLayer()->getCell(10, 10).getSymbol() == 'O');
}

TEST_CASE("TextCommand inserts text", "[commands]") {
    Canvas c(20, 10);
    TextCommand cmd(5, 5, "Hi", Cell('X', Color::Green(), Color::Black()));
    cmd.execute(&c);
    REQUIRE(c.getActiveLayer()->getCell(5, 5).getSymbol() == 'H');
    REQUIRE(c.getActiveLayer()->getCell(6, 5).getSymbol() == 'i');
}