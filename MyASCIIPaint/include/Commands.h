#ifndef COMMANDS_H
#define COMMANDS_H

#include "Command.h"
#include "Cell.h"
#include <vector>
#include <tuple>

class PencilCommand : public Command {
private:
    int x, y;
    Cell newCell;
    Cell oldCell;
public:
    PencilCommand(int x, int y, const Cell& newCell);
    bool execute(Canvas* canvas) override;
    bool undo(Canvas* canvas) override;
    std::string getName() const override { return "Pencil"; }
};

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

class LineCommand : public Command {
private:
    int x1, y1, x2, y2;
    Cell symbol;
    std::vector<Cell> oldCells;
    std::vector<Point> points;
    std::vector<Point> bresenham() const;
public:
    LineCommand(int x1, int y1, int x2, int y2, const Cell& symbol);
    bool execute(Canvas* canvas) override;
    bool undo(Canvas* canvas) override;
    std::string getName() const override { return "Line"; }
};

class RectangleCommand : public Command {
private:
    int x1, y1, x2, y2;
    Cell symbol;
    bool filled;
    std::vector<std::vector<Cell>> oldCells;
public:
    RectangleCommand(int x1, int y1, int x2, int y2, const Cell& symbol, bool filled);
    bool execute(Canvas* canvas) override;
    bool undo(Canvas* canvas) override;
    std::string getName() const override { return "Rectangle"; }
};

class EllipseCommand : public Command {
private:
    int x1, y1, x2, y2;
    Cell symbol;
    std::vector<std::tuple<int, int, Cell>> affectedCells;
    bool isInsideEllipse(int x, int y, int cx, int cy, int a, int b) const;
public:
    EllipseCommand(int x1, int y1, int x2, int y2, const Cell& symbol);
    bool execute(Canvas* canvas) override;
    bool undo(Canvas* canvas) override;
    std::string getName() const override { return "Ellipse"; }
};

class FloodFillCommand : public Command {
private:
    int startX, startY;
    Cell newCell;
    std::vector<std::tuple<int, int, Cell>> affectedCells;
    void floodFill(class Layer* layer, int x, int y, const Cell& targetCell);
public:
    FloodFillCommand(int startX, int startY, const Cell& newCell);
    bool execute(Canvas* canvas) override;
    bool undo(Canvas* canvas) override;
    std::string getName() const override { return "FloodFill"; }
};

class TextCommand : public Command {
private:
    int x, y;
    std::string text;
    Cell symbol;
    std::vector<Cell> oldCells;
public:
    TextCommand(int x, int y, const std::string& text, const Cell& symbol);
    bool execute(Canvas* canvas) override;
    bool undo(Canvas* canvas) override;
    std::string getName() const override { return "Text"; }
};

#endif