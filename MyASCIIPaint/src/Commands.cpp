#include "Commands.h"
#include "Canvas.h"
#include "Layer.h"
#include <cmath>
#include <algorithm>
#include <stack>

PencilCommand::PencilCommand(int x, int y, const Cell& newCell) 
    : x(x), y(y), newCell(newCell) {}

bool PencilCommand::execute(Canvas* canvas) {
    Layer* layer = canvas->getActiveLayer();
    if (layer) {
        oldCell = layer->getCell(x, y);
        layer->setCell(x, y, newCell);
        return true;
    }
    return false;
}

bool PencilCommand::undo(Canvas* canvas) {
    Layer* layer = canvas->getActiveLayer();
    if (layer) {
        layer->setCell(x, y, oldCell);
        return true;
    }
    return false;
}

LineCommand::LineCommand(int x1, int y1, int x2, int y2, const Cell& symbol)
    : x1(x1), y1(y1), x2(x2), y2(y2), symbol(symbol) {
    points = bresenham();
}

std::vector<Point> LineCommand::bresenham() const {
    std::vector<Point> result;
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int x = x1, y = y1;
    
    while (true) {
        result.push_back(Point(x, y));
        if (x == x2 && y == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x += sx; }
        if (e2 < dx) { err += dx; y += sy; }
    }
    return result;
}

bool LineCommand::execute(Canvas* canvas) {
    Layer* layer = canvas->getActiveLayer();
    if (!layer) return false;
    
    oldCells.clear();
    for (const auto& p : points) {
        oldCells.push_back(layer->getCell(p.x, p.y));
        layer->setCell(p.x, p.y, symbol);
    }
    return true;
}

bool LineCommand::undo(Canvas* canvas) {
    Layer* layer = canvas->getActiveLayer();
    if (!layer) return false;
    
    for (size_t i = 0; i < points.size() && i < oldCells.size(); i++) {
        layer->setCell(points[i].x, points[i].y, oldCells[i]);
    }
    return true;
}

RectangleCommand::RectangleCommand(int x1, int y1, int x2, int y2, const Cell& symbol, bool filled)
    : x1(x1), y1(y1), x2(x2), y2(y2), symbol(symbol), filled(filled) {}

bool RectangleCommand::execute(Canvas* canvas) {
    Layer* layer = canvas->getActiveLayer();
    if (!layer) return false;
    
    int minX = std::min(x1, x2);
    int maxX = std::max(x1, x2);
    int minY = std::min(y1, y2);
    int maxY = std::max(y1, y2);
    
    oldCells.clear();
    oldCells.resize(maxY - minY + 1, std::vector<Cell>(maxX - minX + 1));
    
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            bool onBorder = (x == minX || x == maxX || y == minY || y == maxY);
            if (filled || onBorder) {
                oldCells[y - minY][x - minX] = layer->getCell(x, y);
                layer->setCell(x, y, symbol);
            }
        }
    }
    return true;
}

bool RectangleCommand::undo(Canvas* canvas) {
    Layer* layer = canvas->getActiveLayer();
    if (!layer) return false;
    
    int minX = std::min(x1, x2);
    int minY = std::min(y1, y2);
    
    for (size_t y = 0; y < oldCells.size(); y++) {
        for (size_t x = 0; x < oldCells[y].size(); x++) {
            layer->setCell(minX + static_cast<int>(x), minY + static_cast<int>(y), oldCells[y][x]);
        }
    }
    return true;
}

EllipseCommand::EllipseCommand(int x1, int y1, int x2, int y2, const Cell& symbol)
    : x1(x1), y1(y1), x2(x2), y2(y2), symbol(symbol) {}

bool EllipseCommand::isInsideEllipse(int x, int y, int cx, int cy, int a, int b) const {
    if (a <= 0 || b <= 0) return false;
    
    double dx = static_cast<double>(x - cx) / static_cast<double>(a);
    double dy = static_cast<double>(y - cy) / static_cast<double>(b);
    double value = (dx * dx + dy * dy);
    
    return value <= 1.05;  // Допуск для пиксельной сетки
}

bool EllipseCommand::execute(Canvas* canvas) {
    Layer* layer = canvas->getActiveLayer();
    if (!layer) return false;
    
    int minX = std::min(x1, x2);
    int maxX = std::max(x1, x2);
    int minY = std::min(y1, y2);
    int maxY = std::max(y1, y2);
    
    int cx = (minX + maxX) / 2;
    int cy = (minY + maxY) / 2;
    int a = (maxX - minX) / 2;
    int b = (maxY - minY) / 2;
    
    affectedCells.clear();
    
    // Особые случаи для эллипса 
    
    // Точка
    if (a <= 0 && b <= 0) {
        if (cx >= 0 && cx < layer->getWidth() && cy >= 0 && cy < layer->getHeight()) {
            Cell oldCell = layer->getCell(cx, cy);
            affectedCells.push_back(std::make_tuple(cx, cy, oldCell));
            layer->setCell(cx, cy, symbol);
            return true;
        }
        return false;
    }
    
    // Вертикальная линия
    if (a <= 0) {
        for (int y = minY; y <= maxY; y++) {
            if (cx >= 0 && cx < layer->getWidth() && y >= 0 && y < layer->getHeight()) {
                Cell oldCell = layer->getCell(cx, y);
                affectedCells.push_back(std::make_tuple(cx, y, oldCell));
                layer->setCell(cx, y, symbol);
            }
        }
        return true;
    }
    
    // Горизонтальная линия
    if (b <= 0) {
        for (int x = minX; x <= maxX; x++) {
            if (x >= 0 && x < layer->getWidth() && cy >= 0 && cy < layer->getHeight()) {
                Cell oldCell = layer->getCell(x, cy);
                affectedCells.push_back(std::make_tuple(x, cy, oldCell));
                layer->setCell(x, cy, symbol);
            }
        }
        return true;
    }
    
    // Стандартное заполнение эллипса
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            if (x < 0 || x >= layer->getWidth() || y < 0 || y >= layer->getHeight()) {
                continue;
            }
            
            if (isInsideEllipse(x, y, cx, cy, a, b)) {
                Cell oldCell = layer->getCell(x, y);
                affectedCells.push_back(std::make_tuple(x, y, oldCell));
                layer->setCell(x, y, symbol);
            }
        }
    }
    
    return true;
}

bool EllipseCommand::undo(Canvas* canvas) {
    Layer* layer = canvas->getActiveLayer();
    if (!layer) return false;
    
    for (const auto& entry : affectedCells) {
        int x = std::get<0>(entry);
        int y = std::get<1>(entry);
        Cell oldCell = std::get<2>(entry);
        
        if (x >= 0 && x < layer->getWidth() && y >= 0 && y < layer->getHeight()) {
            layer->setCell(x, y, oldCell);
        }
    }
    return true;
}

FloodFillCommand::FloodFillCommand(int startX, int startY, const Cell& newCell)
    : startX(startX), startY(startY), newCell(newCell) {}

void FloodFillCommand::floodFill(Layer* layer, int x, int y, const Cell& targetCell) {
    if (!layer) return;
    
    std::stack<std::pair<int, int>> stack;
    stack.push({x, y});
    
    while (!stack.empty()) {
        auto [cx, cy] = stack.top();
        stack.pop();
        
        if (cx < 0 || cy < 0 || cx >= layer->getWidth() || cy >= layer->getHeight()) continue;
        
        Cell current = layer->getCell(cx, cy);
        if (current.equals(targetCell) && !current.equals(newCell)) {
            affectedCells.push_back(std::make_tuple(cx, cy, current));
            layer->setCell(cx, cy, newCell);
            
            stack.push({cx + 1, cy});
            stack.push({cx - 1, cy});
            stack.push({cx, cy + 1});
            stack.push({cx, cy - 1});
        }
    }
}

bool FloodFillCommand::execute(Canvas* canvas) {
    Layer* layer = canvas->getActiveLayer();
    if (!layer) return false;
    
    affectedCells.clear();
    Cell targetCell = layer->getCell(startX, startY);
    floodFill(layer, startX, startY, targetCell);
    return true;
}

bool FloodFillCommand::undo(Canvas* canvas) {
    Layer* layer = canvas->getActiveLayer();
    if (!layer) return false;
    
    for (const auto& cell : affectedCells) {
        int x = std::get<0>(cell);
        int y = std::get<1>(cell);
        Cell oldCell = std::get<2>(cell);
        layer->setCell(x, y, oldCell);
    }
    return true;
}

TextCommand::TextCommand(int x, int y, const std::string& text, const Cell& symbol)
    : x(x), y(y), text(text), symbol(symbol) {}

bool TextCommand::execute(Canvas* canvas) {
    Layer* layer = canvas->getActiveLayer();
    if (!layer) return false;
    
    oldCells.clear();
    for (size_t i = 0; i < text.length(); i++) {
        int px = x + static_cast<int>(i);
        if (px >= layer->getWidth()) break;
        
        oldCells.push_back(layer->getCell(px, y));
        Cell txtCell = symbol;
        txtCell.setSymbol(text[i]);
        layer->setCell(px, y, txtCell);
    }
    return true;
}

bool TextCommand::undo(Canvas* canvas) {
    Layer* layer = canvas->getActiveLayer();
    if (!layer || oldCells.empty()) return false;
    
    for (size_t i = 0; i < oldCells.size(); i++) {
        layer->setCell(x + static_cast<int>(i), y, oldCells[i]);
    }
    return true;
}