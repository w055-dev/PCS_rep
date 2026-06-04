#include "Canvas.h"
#include "Command.h"
#include "Exporters.h"
#include <iostream>
#include <algorithm>

Canvas::Canvas(int width, int height) 
    : width(width), height(height), activeLayerIndex(0) {
    addLayer();
    initializeWithGrid();
}

Canvas::~Canvas() {
    for (auto* layer : layers) delete layer;
}

int Canvas::getWidth() const { return width; }
int Canvas::getHeight() const { return height; }

void Canvas::initializeWithGrid() {
    Layer* layer = getActiveLayer();
    if (!layer) return;
    
    // ASCII-рамка
    for (int x = 0; x < width; x++) {
        layer->setCell(x, 0, Cell('=', Color::Cyan(), Color::Black()));
        layer->setCell(x, height-1, Cell('=', Color::Cyan(), Color::Black()));
    }
    for (int y = 0; y < height; y++) {
        layer->setCell(0, y, Cell('|', Color::Cyan(), Color::Black()));
        layer->setCell(width-1, y, Cell('|', Color::Cyan(), Color::Black()));
    }
    layer->setCell(0, 0, Cell('+', Color::Cyan(), Color::Black()));
    layer->setCell(width-1, 0, Cell('+', Color::Cyan(), Color::Black()));
    layer->setCell(0, height-1, Cell('+', Color::Cyan(), Color::Black()));
    layer->setCell(width-1, height-1, Cell('+', Color::Cyan(), Color::Black()));
}

void Canvas::render() {
    Layer* activeLayer = getActiveLayer();
    if (!activeLayer) return;
    // Композитный буфер
    std::vector<std::vector<Cell>> composite(height, std::vector<Cell>(width));
    
    for (auto* layer : layers) {
        if (!layer->isVisible()) continue;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Cell cell = layer->getCell(x, y);
                if (!cell.isEmpty()) composite[y][x] = cell;
            }
        }
    }
    
    // Вывод
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Cell cell = composite[y][x];
            
            // Всегда выводим цвет текста
            std::cout << cell.getForeground().toAnsi();
            // Всегда выводим цвет фона
            std::cout << "\033[" << (cell.getBackground().getAnsiCode() + 10) << "m";
            std::cout << cell.getSymbol();
        }
        std::cout << "\033[0m\n";
    }
    std::cout << std::flush;
}

void Canvas::renderWithCursor(int cursorX, int cursorY) {
    render();
    // Рисуем инвертированный курсор поверх холста
    std::cout << "\033[" << (cursorY + 1) << ";" << (cursorX + 1) << "H";
    Layer* layer = getActiveLayer();
    if (layer) {
        Cell cell = layer->getCell(cursorX, cursorY);
        std::cout << "\033[7m" << cell.getSymbol() << "\033[0m";
    }
    std::cout << std::flush;
}

void Canvas::addLayer() {
    layers.push_back(new Layer(width, height));
    activeLayerIndex = layers.size() - 1;
}

void Canvas::removeLayer(int index) {
    if (index >= 0 && index < static_cast<int>(layers.size()) && layers.size() > 1) {
        delete layers[index];
        layers.erase(layers.begin() + index);
        if (activeLayerIndex >= static_cast<int>(layers.size())) {
            activeLayerIndex = layers.size() - 1;
        }
    }
}

Layer* Canvas::getActiveLayer() {
    if (activeLayerIndex >= 0 && activeLayerIndex < static_cast<int>(layers.size())) {
        return layers[activeLayerIndex];
    }
    return nullptr;
}

void Canvas::setActiveLayer(int index) {
    if (index >= 0 && index < static_cast<int>(layers.size())) {
        activeLayerIndex = index;
    }
}

int Canvas::getActiveLayerIndex() const { return activeLayerIndex; }
int Canvas::getLayerCount() const { return layers.size(); }

void Canvas::applyCommand(Command* cmd) {
    if (cmd) cmd->execute(this);
}

bool Canvas::exportTo(Exporter* exporter, const std::string& path) {
    return exporter && exporter->exportTo(this, path);
}