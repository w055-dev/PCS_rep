#include "Tools.h"
#include "Commands.h"
#include "Canvas.h"

void Tool::setSymbol(char symbol) { currentSymbol.setSymbol(symbol); }
void Tool::setColor(const Color& fg, const Color& bg) {
    currentFG = fg; currentBG = bg;
    currentSymbol.setForeground(fg);
    currentSymbol.setBackground(bg);
}

Command* PencilTool::createCommand(Canvas* canvas, int x1, int y1, int x2, int y2) {
    (void)canvas; (void)x2; (void)y2;
    return new PencilCommand(x1, y1, currentSymbol);
}

Command* EraserTool::createCommand(Canvas* canvas, int x1, int y1, int x2, int y2) {
    (void)canvas; (void)x2; (void)y2;
    return new PencilCommand(x1, y1, Cell(' ', currentBG, currentBG));
}

Command* LineTool::createCommand(Canvas* canvas, int x1, int y1, int x2, int y2) {
    (void)canvas;
    return new LineCommand(x1, y1, x2, y2, currentSymbol);
}

RectangleTool::RectangleTool(bool fill) : filled(fill) {}
void RectangleTool::setFilled(bool fill) { filled = fill; }

Command* RectangleTool::createCommand(Canvas* canvas, int x1, int y1, int x2, int y2) {
    (void)canvas;
    return new RectangleCommand(x1, y1, x2, y2, currentSymbol, filled);
}

Command* EllipseTool::createCommand(Canvas* canvas, int x1, int y1, int x2, int y2) {
    (void)canvas;
    return new EllipseCommand(x1, y1, x2, y2, currentSymbol);
}

Command* FloodFillTool::createCommand(Canvas* canvas, int x1, int y1, int x2, int y2) {
    (void)canvas; (void)x2; (void)y2;
    return new FloodFillCommand(x1, y1, currentSymbol);
}

Command* TextTool::createCommand(Canvas* canvas, int x1, int y1, int x2, int y2) {
    (void)canvas; (void)x2; (void)y2;
    return new TextCommand(x1, y1, "ABC", currentSymbol);
}