#include "Layer.h"

Layer::Layer(int width, int height) 
    : name("Layer"), visible(true), width(width), height(height) {
    grid.resize(height, std::vector<Cell>(width));
}

std::string Layer::getName() const { return name; }
void Layer::setName(const std::string& newName) { name = newName; }

bool Layer::isVisible() const { return visible; }
void Layer::setVisible(bool v) { visible = v; }

void Layer::setCell(int x, int y, const Cell& cell) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x] = cell;
    }
}

Cell Layer::getCell(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return grid[y][x];
    }
    return Cell();
}

std::vector<std::vector<Cell>> Layer::getSnapshot() const {
    return grid;
}

void Layer::restore(const std::vector<std::vector<Cell>>& snapshot) {
    if (!snapshot.empty() && !snapshot[0].empty()) {
        grid = snapshot;
        height = static_cast<int>(grid.size());
        width = static_cast<int>(grid[0].size());
    }
}

void Layer::clear() {
    for (auto& row : grid) {
        for (auto& cell : row) {
            cell = Cell();
        }
    }
}

void Layer::resize(int newWidth, int newHeight) {
    grid.resize(newHeight);
    for (auto& row : grid) {
        row.resize(newWidth);
    }
    width = newWidth;
    height = newHeight;
}

int Layer::getWidth() const { return width; }
int Layer::getHeight() const { return height; }