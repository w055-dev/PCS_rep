#include "Cell.h"

Cell::Cell() : symbol(' '), foreground(Color::White()), background(Color::Black()) {}

Cell::Cell(char sym, const Color& fg, const Color& bg) 
    : symbol(sym), foreground(fg), background(bg) {}

char Cell::getSymbol() const { return symbol; }
void Cell::setSymbol(char sym) { symbol = sym; }

Color Cell::getForeground() const { return foreground; }
void Cell::setForeground(const Color& fg) { foreground = fg; }

Color Cell::getBackground() const { return background; }
void Cell::setBackground(const Color& bg) { background = bg; }

bool Cell::isEmpty() const { return symbol == ' '; }

bool Cell::equals(const Cell& other) const {
    return symbol == other.symbol && 
           foreground == other.foreground && 
           background == other.background;
}

std::string Cell::toAnsi() const {
    return background.toAnsiBg() + foreground.toAnsi() + symbol + "\033[0m";
}

std::string Cell::toAnsiCursor() const {
    return foreground.toAnsiBg() + background.toAnsi() + symbol + "\033[0m";
}