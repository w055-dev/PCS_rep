#ifndef TOOLS_H
#define TOOLS_H

#include "Command.h"
#include "Cell.h"
#include "Color.h"

class Canvas;

class Tool {
protected:
    Cell currentSymbol;
    Color currentFG;
    Color currentBG;
    
public:
    virtual ~Tool() = default;
    void setSymbol(char symbol);
    void setColor(const Color& fg, const Color& bg);
    Cell getCurrentSymbol() const { return currentSymbol; }
    virtual std::string getName() const = 0;
    virtual Command* createCommand(Canvas* canvas, int x1, int y1, int x2, int y2) = 0;
};

class PencilTool : public Tool {
public:
    Command* createCommand(Canvas* canvas, int x1, int y1, int x2, int y2) override;
    std::string getName() const override { return "Pencil"; }
};

class EraserTool : public Tool {
public:
    Command* createCommand(Canvas* canvas, int x1, int y1, int x2, int y2) override;
    std::string getName() const override { return "Eraser"; }
};

class LineTool : public Tool {
public:
    Command* createCommand(Canvas* canvas, int x1, int y1, int x2, int y2) override;
    std::string getName() const override { return "Line"; }
};

class RectangleTool : public Tool {
private:
    bool filled;
public:
    RectangleTool(bool filled = false);
    void setFilled(bool fill);
    Command* createCommand(Canvas* canvas, int x1, int y1, int x2, int y2) override;
    std::string getName() const override { return filled ? "FilledRect" : "Rectangle"; }
};

class EllipseTool : public Tool {
public:
    Command* createCommand(Canvas* canvas, int x1, int y1, int x2, int y2) override;
    std::string getName() const override { return "Ellipse"; }
};

class FloodFillTool : public Tool {
public:
    Command* createCommand(Canvas* canvas, int x1, int y1, int x2, int y2) override;
    std::string getName() const override { return "FloodFill"; }
};

class TextTool : public Tool {
public:
    Command* createCommand(Canvas* canvas, int x1, int y1, int x2, int y2) override;
    std::string getName() const override { return "Text"; }
};

#endif