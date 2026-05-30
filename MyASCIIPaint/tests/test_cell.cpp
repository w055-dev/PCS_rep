#include <catch2/catch_test_macros.hpp>
#include "Cell.h"

TEST_CASE("Cell Default State") {
    Cell c;
    REQUIRE(c.getSymbol() == ' ');
    REQUIRE(c.isEmpty() == true);
    REQUIRE(c.getForeground() == Color::White());
    REQUIRE(c.getBackground() == Color::Black());
}

TEST_CASE("Cell Parameterized Construction") {
    Cell c('#', Color::Red(), Color::Blue());
    REQUIRE(c.getSymbol() == '#');
    REQUIRE(c.isEmpty() == false);
    REQUIRE(c.getForeground() == Color::Red());
    REQUIRE(c.getBackground() == Color::Blue());
}

TEST_CASE("Cell Setters Getters") {
    Cell c;
    c.setSymbol('A'); c.setForeground(Color::Green()); c.setBackground(Color::Yellow());
    REQUIRE(c.getSymbol() == 'A');
    REQUIRE(c.getForeground() == Color::Green());
    REQUIRE(c.getBackground() == Color::Yellow());
}

TEST_CASE("Cell isEmpty Logic") {
    Cell empty(' ', Color::White(), Color::Black());
    Cell filled('X', Color::Red(), Color::Black());
    REQUIRE(empty.isEmpty() == true);
    REQUIRE(filled.isEmpty() == false);
}

TEST_CASE("Cell Equals Method") {
    Cell a('#', Color::Red(), Color::Black());
    Cell b('#', Color::Red(), Color::Black());
    Cell c('#', Color::Blue(), Color::Black());
    Cell d(' ', Color::Red(), Color::Black());
    REQUIRE(a.equals(b) == true);
    REQUIRE(a.equals(c) == false);
    REQUIRE(a.equals(d) == false);
}