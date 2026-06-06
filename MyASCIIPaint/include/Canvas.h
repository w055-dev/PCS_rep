#ifndef CANVAS_H
#define CANVAS_H

#include <vector>
#include <string>
#include "Layer.h"

class Exporter;
class ASCPExporter;
class ASCPImporter;

class Canvas {
private:
    int width;
    int height;
    std::vector<Layer*> layers;
    int activeLayerIndex;
    
public:
    Canvas(int width = 80, int height = 25);
    ~Canvas();
    
    int getWidth() const;
    int getHeight() const;
    
    void render();
    void renderWithCursor(int cursorX, int cursorY);
    void initializeWithGrid();
    
    void addLayer();
    void removeLayer(int index);
    Layer* getActiveLayer();
    void setActiveLayer(int index);
    int getActiveLayerIndex() const;
    int getLayerCount() const;
    
    // Для доступа к произвольному слою
    Layer* getLayer(int index) {
        if (index >= 0 && index < static_cast<int>(layers.size())) {
            return layers[index];
        }
        return nullptr;
    }

    Cell getCompositeCell(int x, int y) const;
    
    void applyCommand(class Command* cmd);
    bool exportTo(Exporter* exporter, const std::string& path);
    
    // Дружественные классы для доступа к layers
    friend class ASCPExporter;
    friend class ASCPImporter;
};

#endif