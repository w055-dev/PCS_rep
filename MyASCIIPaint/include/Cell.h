#ifndef CELL_H
#define CELL_H

#include "Color.h"

class Cell {
private:
    char symbol;
    Color foreground;
    Color background;
    
public:
    Cell();
    Cell(char sym, const Color& fg, const Color& bg);
    
    char getSymbol() const;
    void setSymbol(char sym);
    Color getForeground() const;
    void setForeground(const Color& fg);
    Color getBackground() const;
    void setBackground(const Color& bg);
    bool isEmpty() const;
    bool equals(const Cell& other) const;
};

#endif