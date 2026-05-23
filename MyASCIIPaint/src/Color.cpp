#include "Color.h"

Color::Color() : ansiCode(0), r(0), g(0), b(0) {}

Color::Color(int ansiCode) : ansiCode(ansiCode), r(0), g(0), b(0) {}

Color::Color(int r, int g, int b) : ansiCode(0), r(r), g(g), b(b) {}

int Color::getAnsiCode() const { return ansiCode; }
void Color::setAnsiCode(int code) { ansiCode = code; }

std::string Color::toAnsi() const {
    return "\033[" + std::to_string(ansiCode) + "m";
}

Color Color::Black()   { return Color(30); }
Color Color::Red()     { return Color(31); }
Color Color::Green()   { return Color(32); }
Color Color::Yellow()  { return Color(33); }
Color Color::Blue()    { return Color(34); }
Color Color::Magenta() { return Color(35); }
Color Color::Cyan()    { return Color(36); }
Color Color::White()   { return Color(37); }

Color Color::BrightBlack()   { return Color(90); }
Color Color::BrightRed()     { return Color(91); }
Color Color::BrightGreen()   { return Color(92); }
Color Color::BrightYellow()  { return Color(93); }
Color Color::BrightBlue()    { return Color(94); }
Color Color::BrightMagenta() { return Color(95); }
Color Color::BrightCyan()    { return Color(96); }
Color Color::BrightWhite()   { return Color(97); }

bool Color::operator==(const Color& other) const {
    return ansiCode == other.ansiCode;
}

bool Color::operator!=(const Color& other) const {
    return ansiCode != other.ansiCode;
}