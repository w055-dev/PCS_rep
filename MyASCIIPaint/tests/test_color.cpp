#include <catch2/catch_test_macros.hpp>
#include "Color.h"

TEST_CASE("Color default constructor", "[color]") {
    Color c;
    REQUIRE(c.getAnsiCode() == 0);
}

TEST_CASE("Color ANSI code constructor", "[color]") {
    Color c(196);
    REQUIRE(c.getAnsiCode() == 196);
}

TEST_CASE("Color predefined constants work", "[color]") {
    REQUIRE(Color::Black().getAnsiCode() == 30);
    REQUIRE(Color::Red().getAnsiCode() == 31);
    REQUIRE(Color::Green().getAnsiCode() == 32);
    REQUIRE(Color::Blue().getAnsiCode() == 34);
    REQUIRE(Color::White().getAnsiCode() == 37);
    REQUIRE(Color::Yellow().getAnsiCode() == 33);
    REQUIRE(Color::Magenta().getAnsiCode() == 35);
    REQUIRE(Color::Cyan().getAnsiCode() == 36);
}

TEST_CASE("Color bright variants work", "[color]") {
    REQUIRE(Color::BrightBlack().getAnsiCode() == 90);
    REQUIRE(Color::BrightRed().getAnsiCode() == 91);
    REQUIRE(Color::BrightGreen().getAnsiCode() == 92);
    REQUIRE(Color::BrightBlue().getAnsiCode() == 94);
    REQUIRE(Color::BrightYellow().getAnsiCode() == 93);
    REQUIRE(Color::BrightMagenta().getAnsiCode() == 95);
    REQUIRE(Color::BrightCyan().getAnsiCode() == 96);
    REQUIRE(Color::BrightWhite().getAnsiCode() == 97);
}

TEST_CASE("Color equality operators work", "[color]") {
    Color red1 = Color::Red();
    Color red2 = Color::Red();
    Color blue = Color::Blue();
    
    REQUIRE(red1 == red2);
    REQUIRE(red1 != blue);
}

TEST_CASE("Color toAnsi generates escape codes", "[color]") {
    Color red = Color::Red();
    std::string ansi = red.toAnsi();
    
    REQUIRE_FALSE(ansi.empty());
    REQUIRE(ansi == "\033[31m");
}

TEST_CASE("Color setAnsiCode modifies color", "[color]") {
    Color c;
    REQUIRE(c.getAnsiCode() == 0);
    
    c.setAnsiCode(31);
    REQUIRE(c.getAnsiCode() == 31);
    
    c.setAnsiCode(32);
    REQUIRE(c.getAnsiCode() == 32);
}

TEST_CASE("Color toAnsi works for different colors", "[color]") {
    REQUIRE(Color::Black().toAnsi() == "\033[30m");
    REQUIRE(Color::Red().toAnsi() == "\033[31m");
    REQUIRE(Color::Green().toAnsi() == "\033[32m");
    REQUIRE(Color::BrightRed().toAnsi() == "\033[91m");
    REQUIRE(Color::BrightBlue().toAnsi() == "\033[94m");
}