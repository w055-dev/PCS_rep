#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Canvas;

class Command {
public:
    virtual ~Command() = default;
    virtual bool execute(Canvas* canvas) = 0;
    virtual bool undo(Canvas* canvas) = 0;
    virtual std::string getName() const { return "Command"; }
};

#endif