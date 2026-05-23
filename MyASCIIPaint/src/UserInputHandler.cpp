#include "UserInputHandler.h"
#include "Canvas.h"
#include "HistoryManager.h"
#include "Commands.h"
#include <iostream>

UserInputHandler::UserInputHandler(Canvas* canvas, HistoryManager* history)
    : canvas(canvas), history(history), currentTool(nullptr), 
      rectangleTool(false), filledRectangleTool(true),
      pendingX1(0), pendingY1(0), pendingX2(0), pendingY2(0),
      isAwaitingSecondPoint(false), isAwaitingTextInput(false) {}

void UserInputHandler::setTool(Tool* tool) {
    currentTool = tool;
    isAwaitingSecondPoint = false;
    isAwaitingTextInput = false;
}

bool UserInputHandler::handleCommand([[maybe_unused]] char key) {
    // Заглушка для будущих расширений
    return true;
}

void UserInputHandler::handlePointSelection(int x, int y) {
    if (!currentTool) return;
    
    if (!isAwaitingSecondPoint) {
        pendingX1 = x;
        pendingY1 = y;
        
        std::string name = currentTool->getName();
        if (name == "Pencil" || name == "Eraser" || name == "FloodFill" || name == "Text") {
            pendingX2 = x;
            pendingY2 = y;
            handleToolCommand(currentTool, pendingX1, pendingY1, pendingX2, pendingY2);
            return;
        }
        
        isAwaitingSecondPoint = true;
    } else {
        pendingX2 = x;
        pendingY2 = y;
        isAwaitingSecondPoint = false;
        handleToolCommand(currentTool, pendingX1, pendingY1, pendingX2, pendingY2);
    }
}

void UserInputHandler::handleToolCommand(Tool* tool, int x1, int y1, int x2, int y2) {
    Command* cmd = tool->createCommand(canvas, x1, y1, x2, y2);
    if (cmd) {
        if (tool->getName() == "Text" && isAwaitingTextInput) {
            delete cmd;
            cmd = new TextCommand(x1, y1, pendingText, tool->getCurrentSymbol());
            isAwaitingTextInput = false;
        }
        executeCommand(cmd);
    }
}

void UserInputHandler::executeCommand(Command* cmd) {
    if (history && cmd) {
        if (cmd->execute(canvas)) {
            history->pushCommand(cmd);
        } else {
            delete cmd;
        }
    }
}

void UserInputHandler::handleTextInput(const std::string& text, int x, int y) {
    pendingText = text;
    pendingX1 = x;
    pendingY1 = y;
    isAwaitingTextInput = true;
    isAwaitingSecondPoint = false;
    handleToolCommand(currentTool, x, y, x, y);
}