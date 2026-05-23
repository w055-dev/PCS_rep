#include "HistoryManager.h"
#include <algorithm>

HistoryManager* HistoryManager::instance = nullptr;

HistoryManager::HistoryManager() : maxSize(50) {}

HistoryManager* HistoryManager::getInstance() {
    if (instance == nullptr) {
        instance = new HistoryManager();
    }
    return instance;
}

HistoryManager::~HistoryManager() {
    clear();
}

void HistoryManager::pushCommand(Command* cmd) {
    if (!cmd) return;
    
    // Очищаем стек redo при новом действии
    for (auto* c : redoStack) delete c;
    redoStack.clear();
    
    undoStack.push_back(cmd);
    
    // Ограничиваем размер истории
    while (static_cast<int>(undoStack.size()) > maxSize) {
        delete undoStack.front();
        undoStack.erase(undoStack.begin());
    }
}

bool HistoryManager::undo(Canvas* canvas) {
    if (undoStack.empty()) return false;
    
    Command* cmd = undoStack.back();
    undoStack.pop_back();
    
    if (cmd->undo(canvas)) {
        redoStack.push_back(cmd);
        return true;
    }
    delete cmd;
    return false;
}

bool HistoryManager::redo(Canvas* canvas) {
    if (redoStack.empty()) return false;
    
    Command* cmd = redoStack.back();
    redoStack.pop_back();
    
    if (cmd->execute(canvas)) {
        undoStack.push_back(cmd);
        return true;
    }
    delete cmd;
    return false;
}

bool HistoryManager::canUndo() const {
    return !undoStack.empty();
}

bool HistoryManager::canRedo() const {
    return !redoStack.empty();
}

void HistoryManager::clear() {
    for (auto* cmd : undoStack) delete cmd;
    for (auto* cmd : redoStack) delete cmd;
    undoStack.clear();
    redoStack.clear();
}