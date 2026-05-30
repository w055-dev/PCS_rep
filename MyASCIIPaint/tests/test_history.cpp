#include <catch2/catch_test_macros.hpp>
#include "HistoryManager.h"
#include "Canvas.h"
#include "Commands.h"

class TestCommand : public Command {
    bool executed = false, undone = false;
public:
    bool execute(Canvas*) override { executed = true; return true; }
    bool undo(Canvas*) override { undone = true; return true; }
    bool wasExecuted() const { return executed; }
    bool wasUndone() const { return undone; }
};

TEST_CASE("HistoryManager Singleton") {
    auto* h1 = HistoryManager::getInstance();
    auto* h2 = HistoryManager::getInstance();
    REQUIRE(h1 == h2);
}

TEST_CASE("History Push & Undo") {
    auto* hist = HistoryManager::getInstance();
    hist->clear();
    Canvas c(10, 10);
    auto* cmd = new TestCommand();
    hist->pushCommand(cmd);
    REQUIRE(hist->canUndo() == true);
    REQUIRE(hist->canRedo() == false);
    
    hist->undo(&c);
    REQUIRE(cmd->wasUndone() == true);
    REQUIRE(hist->canUndo() == false);
    REQUIRE(hist->canRedo() == true);
}

TEST_CASE("History Redo Flow") {
    auto* hist = HistoryManager::getInstance();
    hist->clear();
    Canvas c(10, 10);
    auto* cmd = new TestCommand();
    hist->pushCommand(cmd);
    hist->undo(&c);
    hist->redo(&c);
    REQUIRE(cmd->wasExecuted() == true);
    REQUIRE(hist->canRedo() == false);
    REQUIRE(hist->canUndo() == true);
}

TEST_CASE("History New Command Clears Redo") {
    auto* hist = HistoryManager::getInstance();
    hist->clear();
    Canvas c(10, 10);
    auto* cmd1 = new TestCommand();
    auto* cmd2 = new TestCommand();
    hist->pushCommand(cmd1);
    hist->undo(&c);
    REQUIRE(hist->canRedo() == true);
    REQUIRE(hist->getUndoStackSize() == 0);
    
    hist->pushCommand(cmd2);
    REQUIRE(hist->canRedo() == false);
    REQUIRE(hist->getUndoStackSize() == 1);
    REQUIRE(hist->getRedoStackSize() == 0);
}

TEST_CASE("History Max Size Limit") {
    auto* hist = HistoryManager::getInstance();
    hist->clear();
    Canvas c(10, 10);
    for(int i=0; i<60; ++i) hist->pushCommand(new TestCommand());
    REQUIRE(hist->getUndoStackSize() == 50);
}

TEST_CASE("History Clear") {
    auto* hist = HistoryManager::getInstance();
    hist->clear();
    Canvas c(10, 10);
    hist->pushCommand(new TestCommand());
    hist->clear();
    REQUIRE(hist->canUndo() == false);
    REQUIRE(hist->canRedo() == false);
    REQUIRE(hist->getUndoStackSize() == 0);
}