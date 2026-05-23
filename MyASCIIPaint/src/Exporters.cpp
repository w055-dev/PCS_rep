#include "Exporters.h"
#include "Canvas.h"
#include "Layer.h"
#include "Color.h"
#include "Cell.h"
#include <fstream>
#include <iostream>
#include <vector>

//Использовал библиотеку nlohmann/json для парсинга JSON
using json = nlohmann::json;

std::vector<std::vector<Cell>> getCompositeFrame(Canvas* canvas) {
    int h = canvas->getHeight();
    int w = canvas->getWidth();
    std::vector<std::vector<Cell>> composite(h, std::vector<Cell>(w));
    
    for (int l = 0; l < canvas->getLayerCount(); l++) {
        Layer* layer = canvas->getLayer(l);
        if (!layer || !layer->isVisible()) continue;
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                Cell cell = layer->getCell(x, y);
                if (!cell.isEmpty()) {
                    composite[y][x] = cell;
                }
            }
        }
    }
    return composite;
}

bool TXTExporter::exportTo(Canvas* canvas, const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) return false;
    auto frame = getCompositeFrame(canvas);
    for (int y = 0; y < canvas->getHeight(); y++) {
        for (int x = 0; x < canvas->getWidth(); x++) {
            file << frame[y][x].getSymbol();
        }
        file << '\n';
    }
    return true;
}

bool ANSIExporter::exportTo(Canvas* canvas, const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) return false;
    auto frame = getCompositeFrame(canvas);
    for (int y = 0; y < canvas->getHeight(); y++) {
        Color lastFG = Color::White(), lastBG = Color::Black();
        for (int x = 0; x < canvas->getWidth(); x++) {
            Cell cell = frame[y][x];
            if (cell.getForeground() != lastFG) {
                file << cell.getForeground().toAnsi();
                lastFG = cell.getForeground();
            }
            if (cell.getBackground() != lastBG) {
                file << "\033[" << (cell.getBackground().getAnsiCode() + 10) << "m";
                lastBG = cell.getBackground();
            }
            file << cell.getSymbol();
        }
        file << "\033[0m\n";
    }
    return true;
}

bool CSVExporter::exportTo(Canvas* canvas, const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) return false;
    auto frame = getCompositeFrame(canvas);
    for (int y = 0; y < canvas->getHeight(); y++) {
        for (int x = 0; x < canvas->getWidth(); x++) {
            char ch = frame[y][x].getSymbol();
            if (ch == ',' || ch == '"' || ch == '\n') {
                file << "\"" << (ch == '"' ? "\"\"" : std::string(1, ch)) << "\"";
            } else {
                file << ch;
            }
            if (x < canvas->getWidth() - 1) file << ",";
        }
        file << '\n';
    }
    return true;
}

bool JSONExporter::exportTo(Canvas* canvas, const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) return false;
    
    json j;
    j["format"] = "ASCIIPaint";
    j["version"] = "1.0";
    j["width"] = canvas->getWidth();
    j["height"] = canvas->getHeight();
    
    auto frame = getCompositeFrame(canvas);
    json rows = json::array();
    for (int y = 0; y < canvas->getHeight(); y++) {
        std::string row;
        for (int x = 0; x < canvas->getWidth(); x++) {
            row += frame[y][x].getSymbol();
        }
        rows.push_back(row);
    }
    j["composite"] = rows;
    
    file << j.dump(2);  // Красивый вывод с отступами
    return true;
}

bool ASCPExporter::exportTo(Canvas* canvas, const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "[ASCP] Cannot create file: " << path << std::endl;
        return false;
    }
    
    json j;
    j["format"] = "ASCIIPaint-Project";
    j["version"] = "1.0";
    j["width"] = canvas->getWidth();
    j["height"] = canvas->getHeight();
    j["activeLayer"] = canvas->getActiveLayerIndex();
    
    json layersArr = json::array();
    
    for (int l = 0; l < canvas->getLayerCount(); l++) {
        Layer* layer = canvas->getLayer(l);
        if (!layer) continue;
        
        json layerObj;
        layerObj["name"] = layer->getName();
        layerObj["visible"] = layer->isVisible();
        
        json cellsArr = json::array();
        
        for (int y = 0; y < canvas->getHeight(); y++) {
            json row = json::array();
            for (int x = 0; x < canvas->getWidth(); x++) {
                Cell cell = layer->getCell(x, y);
                json cellObj;
                cellObj["s"] = std::string(1, cell.getSymbol());
                cellObj["fg"] = cell.getForeground().getAnsiCode();
                cellObj["bg"] = cell.getBackground().getAnsiCode();
                row.push_back(cellObj);
            }
            cellsArr.push_back(row);
        }
        
        layerObj["cells"] = cellsArr;
        layersArr.push_back(layerObj);
    }
    
    j["layers"] = layersArr;
    
    file << j.dump(2);
    std::cout << "[ASCP] Saved project to " << path << std::endl;
    return true;
}

bool ASCPImporter::validate(const std::string& path) {
    try {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "[ASCP] Cannot open: " << path << std::endl;
            return false;
        }
        
        json j = json::parse(file);
        
        bool valid = j.contains("format") && 
                     j["format"] == "ASCIIPaint-Project" &&
                     j.contains("width") && 
                     j.contains("height") &&
                     j.contains("layers");
        
        if (!valid) {
            std::cerr << "[ASCP] Missing required fields" << std::endl;
        }
        
        return valid;
        
    } catch (const json::parse_error& e) {
        std::cerr << "[ASCP] Parse error: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "[ASCP] Error: " << e.what() << std::endl;
        return false;
    }
}

Canvas* ASCPImporter::importFrom(const std::string& path, [[maybe_unused]] HistoryManager* history) {
    try {
        std::ifstream file(path);
        if (!file.is_open()) {
            return nullptr;
        }
        
        json j = json::parse(file);
        
        int width = j.at("width").get<int>();
        int height = j.at("height").get<int>();
        int activeLayerIdx = j.value("activeLayer", 0);
        
        Canvas* newCanvas = new Canvas(width, height);
        
        while (newCanvas->getLayerCount() > 1) {
            newCanvas->removeLayer(0);
        }
        
        Layer* layer = newCanvas->getActiveLayer();
        if (!layer) {
            delete newCanvas;
            return nullptr;
        }
        
        layer->setVisible(true);
        layer->setName("Imported Layer");
        
        // Парсим слои
        if (j.contains("layers") && j["layers"].is_array()) {
            for (const auto& layerJson : j["layers"]) {
                // Восстанавливаем видимость и имя
                if (layerJson.contains("visible")) {
                    layer->setVisible(layerJson["visible"].get<bool>());
                }
                if (layerJson.contains("name")) {
                    layer->setName(layerJson["name"].get<std::string>());
                }
                
                // Парсим ячейки
                if (layerJson.contains("cells") && layerJson["cells"].is_array()) {
                    const auto& cellsArr = layerJson["cells"];
                    
                    for (size_t y = 0; y < cellsArr.size() && y < static_cast<size_t>(height); y++) {
                        const auto& row = cellsArr[y];
                        if (!row.is_array()) continue;
                        
                        for (size_t x = 0; x < row.size() && x < static_cast<size_t>(width); x++) {
                            const auto& cellJson = row[x];
                            if (!cellJson.is_object()) continue;
                            
                            // Символ
                            char symbol = ' ';
                            if (cellJson.contains("s") && cellJson["s"].is_string()) {
                                std::string s = cellJson["s"].get<std::string>();
                                if (!s.empty()) symbol = s[0];
                            }
                            
                            // Цвета
                            int fg = cellJson.value("fg", 37);
                            int bg = cellJson.value("bg", 40);
                            
                            // Создаём ячейку (с проверкой границ)
                            if (static_cast<int>(x) < width && static_cast<int>(y) < height) {
                                layer->setCell(static_cast<int>(x), static_cast<int>(y), 
                                             Cell(symbol, Color(fg), Color(bg)));
                            }
                        }
                    }
                }
            }
        }
        
        // Устанавливаем активный слой
        if (activeLayerIdx >= 0 && activeLayerIdx < newCanvas->getLayerCount()) {
            newCanvas->setActiveLayer(activeLayerIdx);
        }
        
        return newCanvas;
        
    } catch (const json::parse_error& e) {
        std::cerr << "[ASCP] Parse error: " << e.what() << std::endl;
        return nullptr;
    } catch (const json::type_error& e) {
        std::cerr << "[ASCP] Type error: " << e.what() << std::endl;
        return nullptr;
    } catch (const std::exception& e) {
        std::cerr << "[ASCP] Error: " << e.what() << std::endl;
        return nullptr;
    }
}