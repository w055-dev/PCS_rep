#include <catch2/catch_test_macros.hpp>
#include "Layer.h"

TEST_CASE("Layer Construction & Dimensions") {
    Layer l(10, 5);
    REQUIRE(l.getWidth() == 10);
    REQUIRE(l.getHeight() == 5);
    REQUIRE(l.isVisible() == true);
    REQUIRE(l.getName() == "Layer");
}

TEST_CASE("Layer Set/Get Cell Valid Bounds") {
    Layer l(5, 5);
    Cell c('#', Color::Red(), Color::Black());
    l.setCell(2, 3, c);
    REQUIRE(l.getCell(2, 3).equals(c) == true);
}

TEST_CASE("Layer Set/Get Cell Out of Bounds") {
    Layer l(3, 3);
    Cell c('#', Color::Red(), Color::Black());
    l.setCell(5, 5, c);
    REQUIRE(l.getCell(5, 5).isEmpty() == true);
    l.setCell(-1, 0, c);
    REQUIRE(l.getCell(-1, 0).isEmpty() == true);
}

TEST_CASE("Layer Clear Method") {
    Layer l(3, 3);
    l.setCell(1, 1, Cell('#', Color::Red(), Color::Black()));
    l.clear();
    for(int y=0; y<3; ++y)
        for(int x=0; x<3; ++x)
            REQUIRE(l.getCell(x, y).isEmpty() == true);
}

TEST_CASE("Layer Snapshot & Restore") {
    Layer l(2, 2);
    l.setCell(0, 0, Cell('A', Color::Red(), Color::Black()));
    auto snap = l.getSnapshot();
    l.clear();
    REQUIRE(l.getCell(0, 0).isEmpty() == true);
    l.restore(snap);
    REQUIRE(l.getCell(0, 0).getSymbol() == 'A');
}

TEST_CASE("Layer Visibility & Name") {
    Layer l(5, 5);
    l.setVisible(false);
    l.setName("Background");
    REQUIRE(l.isVisible() == false);
    REQUIRE(l.getName() == "Background");
}

TEST_CASE("Layer Resize") {
    Layer l(3, 3);
    l.setCell(2, 2, Cell('X', Color::White(), Color::Black()));
    l.resize(5, 5);
    REQUIRE(l.getWidth() == 5);
    REQUIRE(l.getHeight() == 5);
    REQUIRE(l.getCell(2, 2).getSymbol() == 'X');
}