#ifndef USER_INPUT_HANDLER_H
#define USER_INPUT_HANDLER_H

#include "Tools.h"
#include <string>

class Canvas;
class HistoryManager;

class UserInputHandler {
private:
    Canvas* canvas;
    HistoryManager* history;
    Tool* currentTool;

    PencilTool pencilTool;
    EraserTool eraserTool;
    LineTool lineTool;
    RectangleTool rectangleTool;
    RectangleTool filledRectangleTool;
    EllipseTool ellipseTool;
    FloodFillTool floodFillTool;
    TextTool textTool;
    
    int pendingX1, pendingY1;
    int pendingX2, pendingY2;
    bool isAwaitingSecondPoint;
    bool isAwaitingTextInput;
    std::string pendingText;
    
public:
    UserInputHandler(Canvas* canvas, HistoryManager* history);
    
    void setTool(Tool* tool);
    Tool* getCurrentTool() const { return currentTool; }
    
    bool handleCommand(char key);
    void handlePointSelection(int x, int y);
    void executeCommand(Command* cmd);
    void handleTextInput(const std::string& text, int x, int y);
    
private:
    void handleToolCommand(Tool* tool, int x1, int y1, int x2, int y2);
};

#endif