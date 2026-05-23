#ifndef LAYER_H
#define LAYER_H

#include <vector>
#include <string>
#include "Cell.h"

class Layer {
private:
    std::string name;
    bool visible;
    std::vector<std::vector<Cell>> grid;
    int width;
    int height;
    
public:
    Layer(int width, int height);
    
    std::string getName() const;
    void setName(const std::string& newName);
    bool isVisible() const;
    void setVisible(bool visible);
    void setCell(int x, int y, const Cell& cell);
    Cell getCell(int x, int y) const;
    std::vector<std::vector<Cell>> getSnapshot() const;
    void restore(const std::vector<std::vector<Cell>>& snapshot);
    void clear();
    void resize(int width, int height);
    int getWidth() const;
    int getHeight() const;
};

#endif