#include <catch2/catch_test_macros.hpp>
#include "Canvas.h"
#include "Color.h"

TEST_CASE("Canvas initialization", "[canvas]") {
    Canvas c(20, 15);
    REQUIRE(c.getWidth() == 20);
    REQUIRE(c.getHeight() == 15);
    REQUIRE(c.getLayerCount() == 1);
    REQUIRE(c.getActiveLayerIndex() == 0);
}

TEST_CASE("Canvas layer management", "[canvas]") {
    Canvas c(10, 10);
    c.addLayer();
    REQUIRE(c.getLayerCount() == 2);
    c.setActiveLayer(1);
    REQUIRE(c.getActiveLayerIndex() == 1);
    c.removeLayer(0);
    REQUIRE(c.getLayerCount() == 1);
}

TEST_CASE("Canvas cell access", "[canvas]") {
    Canvas c(10, 10);
    c.getActiveLayer()->setCell(5, 5, Cell('X', Color::Red(), Color::Black()));
    REQUIRE(c.getActiveLayer()->getCell(5, 5).getSymbol() == 'X');
}

TEST_CASE("Canvas multiple layers cell access", "[canvas]") {
    Canvas c(10, 10);
    c.addLayer();
    c.setActiveLayer(0);
    c.getActiveLayer()->setCell(1, 1, Cell('A', Color::Red(), Color::Black()));
    c.setActiveLayer(1);
    c.getActiveLayer()->setCell(1, 1, Cell('B', Color::Blue(), Color::Black()));
    REQUIRE(c.getLayer(0)->getCell(1, 1).getSymbol() == 'A');
    REQUIRE(c.getLayer(1)->getCell(1, 1).getSymbol() == 'B');
}

TEST_CASE("Canvas layer removal updates active index", "[canvas]") {
    Canvas c(10, 10);
    c.addLayer();
    c.addLayer();
    c.setActiveLayer(2);
    c.removeLayer(2);
    REQUIRE(c.getActiveLayerIndex() == 1);
}

TEST_CASE("Canvas preserves data after layer operations", "[canvas]") {
    Canvas c(5, 5);
    c.getActiveLayer()->setCell(2, 2, Cell('X', Color::White(), Color::Black()));
    c.addLayer();
    c.setActiveLayer(0);
    REQUIRE(c.getActiveLayer()->getCell(2, 2).getSymbol() == 'X');
}

TEST_CASE("Canvas render functions don't crash", "[canvas]") {
    Canvas c(5, 5);
    REQUIRE_NOTHROW(c.render());
    REQUIRE_NOTHROW(c.renderWithCursor(2, 2));
}