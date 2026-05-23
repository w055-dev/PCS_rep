#ifndef HISTORY_MANAGER_H
#define HISTORY_MANAGER_H

#include <vector>
#include "Command.h"

class Canvas;

class HistoryManager {
private:
    static HistoryManager* instance;
    std::vector<Command*> undoStack;
    std::vector<Command*> redoStack;
    int maxSize;
    
    HistoryManager();
    
public:
    static HistoryManager* getInstance();
    ~HistoryManager();
    
    void pushCommand(Command* cmd);
    bool undo(Canvas* canvas);
    bool redo(Canvas* canvas);
    bool canUndo() const;
    bool canRedo() const;
    void clear();
    int getUndoStackSize() const { return undoStack.size(); }
    int getRedoStackSize() const { return redoStack.size(); }
    
    HistoryManager(const HistoryManager&) = delete;
    HistoryManager& operator=(const HistoryManager&) = delete;
};

#endif