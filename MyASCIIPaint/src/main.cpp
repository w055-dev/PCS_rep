#include "Canvas.h"
#include "HistoryManager.h"
#include "Tools.h"
#include "Terminal.h"
#include "Exporters.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <algorithm>

const Color colorPalette[] = {
    Color::Black(), Color::Red(), Color::Green(), Color::Yellow(), Color::Blue(), 
    Color::Magenta(), Color::Cyan(), Color::White(), Color::BrightGreen(), Color::BrightYellow()
};

const char* colorNames[] = {
    "Black", "Red", "Green", "Yellow", "Blue", 
    "Magenta", "Cyan", "White", "BrightGreen", "BrightYellow"
};

void renderCanvasOnly(Canvas& canvas) {
    std::cout << "\033[2;1H";
    canvas.render();
    std::cout << std::flush;
}

void clearCursorAt(int x, int y, Canvas& canvas) {
    Cell cell = canvas.getCompositeCell(x, y);
    std::cout << "\033[" << (y + 2) << ";" << (x + 1) << "H";
    std::cout << cell.toAnsi();
}

void drawCursorAt(int x, int y, Canvas& canvas) {
    Cell cell = canvas.getCompositeCell(x, y);
    std::cout << "\033[" << (y + 2) << ";" << (x + 1) << "H";
    std::cout << cell.toAnsiCursor();
}

void updateStatusOnly(int canvasHeight, int canvasWidth, const std::string& tool, 
                    int cx, int cy, int undo, int redo, 
                    const std::string& msg, int colorIdx, int layerIdx) {
    int uiY = canvasHeight + 3;
    std::cout << "\033[" << uiY << ";1H\033[44m\033[97m" 
            << " Tool: " << tool 
            << " | Pos: (" << cx << "," << cy << ")"
            << " | Color: " << colorNames[colorIdx]
            << " | Layer: " << layerIdx
            << " | U:" << undo << " R:" << redo;
    std::cout << "\033[K\033[0m";
    
    std::cout << "\033[" << (uiY + 1) << ";1H\033[93m"
            << " 1-0=Color | P/E/L/R/F/O/G=Tools | []=Layers +/-=Add/Del"
            << " | S=TXT A=ANSI C=CSV J=JSON V=Save I=Load | Z=Undo Y=Redo Q=Quit";
    std::cout << "\033[K\033[0m";
    
    std::cout << "\033[" << (uiY + 2) << ";1H\033[92m" 
            << msg;
    std::cout << "\033[K\033[0m" << std::flush;
}

int main() {
    setvbuf(stdout, nullptr, _IONBF, 0);
    setvbuf(stdin, nullptr, _IONBF, 0);
    #ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        GetConsoleMode(hOut, &mode);
        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    #endif

    Terminal::enableRawMode();
    struct Cleanup { ~Cleanup() { Terminal::disableRawMode(); } } cleanup;

    //Фикс бага: Адаптация размера холста под размер терминала
    auto [termWidth, termHeight] = Terminal::getSize();
    int canvasWidth = std::min(80, termWidth);
    int canvasHeight = std::min(25, termHeight - 5);  // -5 для заголовка + 3 строки статуса + отступ
    
    std::cout << "\033[H\033[2J";  // Очистка экрана
    std::cout << "\033[1;1H=== MyASCIIPaint (" << canvasWidth << "x" << canvasHeight << ") ===\033[K";
    
    Canvas canvas(canvasWidth, canvasHeight);
    HistoryManager* history = HistoryManager::getInstance();

    // Инструменты
    PencilTool pencilTool; pencilTool.setSymbol('#'); 
    pencilTool.setColor(Color::BrightGreen(), Color::Black());
    
    EraserTool eraserTool; eraserTool.setColor(Color::Black(), Color::Black());
    LineTool lineTool; lineTool.setSymbol('*'); 
    lineTool.setColor(Color::BrightYellow(), Color::Black());
    RectangleTool rectTool(false); rectTool.setSymbol('#'); 
    rectTool.setColor(Color::BrightCyan(), Color::Black());
    RectangleTool filledRectTool(true); filledRectTool.setSymbol('#'); 
    filledRectTool.setColor(Color::BrightCyan(), Color::Black());
    EllipseTool ellipseTool; ellipseTool.setSymbol('O'); 
    ellipseTool.setColor(Color::BrightMagenta(), Color::Black());
    FloodFillTool floodTool; floodTool.setSymbol('#'); 
    floodTool.setColor(Color::BrightGreen(), Color::Black());

    Tool* currentTool = &pencilTool;
    int cursorX = 1, cursorY = 1;
    int prevCursorX = 1, prevCursorY = 1;
    int currentColorIdx = 8;
    bool running = true;
    bool awaitingSecondPoint = false;
    int firstX = 0, firstY = 0;
    std::string statusMsg = "Ready! Arrows to move, Space to draw";

    // Начальная отрисовка
    renderCanvasOnly(canvas);
    updateStatusOnly(canvas.getHeight(), canvas.getWidth(),
                    currentTool->getName(), 
                    cursorX, cursorY, history->getUndoStackSize(), 
                    history->getRedoStackSize(), statusMsg, currentColorIdx, 
                    canvas.getActiveLayerIndex());
    drawCursorAt(cursorX, cursorY, canvas);
    // Позиционирование курсора терминала
    std::cout << "\033[" << (cursorY + 2) << ";" << (cursorX + 1) << "H" << std::flush;

    // ГЛАВНЫЙ ЦИКЛ
    while (running) {
        bool canvasChanged = false;
        std::string newStatus = "";
        bool statusChanged = false;

        int key = Terminal::getKey();
        if (key == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }

        switch (key) {
            case 'q': case 'Q': running = false; break;
            
            case 259: if (cursorY > 1) cursorY--; break;                      // стрелка вверх
            case 258: if (cursorY < canvas.getHeight()-2) cursorY++; break;    // стрелка вниз
            case 260: if (cursorX > 1) cursorX--; break;                      // стрелка влево
            case 261: if (cursorX < canvas.getWidth()-2) cursorX++; break;     // стрелка вправо

            case '1': currentColorIdx = 0; currentTool->setColor(colorPalette[0], Color::Black()); newStatus = "Color: Black"; statusChanged = true; break;
            case '2': currentColorIdx = 1; currentTool->setColor(colorPalette[1], Color::Black()); newStatus = "Color: Red"; statusChanged = true; break;
            case '3': currentColorIdx = 2; currentTool->setColor(colorPalette[2], Color::Black()); newStatus = "Color: Green"; statusChanged = true; break;
            case '4': currentColorIdx = 3; currentTool->setColor(colorPalette[3], Color::Black()); newStatus = "Color: Yellow"; statusChanged = true; break;
            case '5': currentColorIdx = 4; currentTool->setColor(colorPalette[4], Color::Black()); newStatus = "Color: Blue"; statusChanged = true; break;
            case '6': currentColorIdx = 5; currentTool->setColor(colorPalette[5], Color::Black()); newStatus = "Color: Magenta"; statusChanged = true; break;
            case '7': currentColorIdx = 6; currentTool->setColor(colorPalette[6], Color::Black()); newStatus = "Color: Cyan"; statusChanged = true; break;
            case '8': currentColorIdx = 7; currentTool->setColor(colorPalette[7], Color::Black()); newStatus = "Color: White"; statusChanged = true; break;
            case '9': currentColorIdx = 8; currentTool->setColor(colorPalette[8], Color::Black()); newStatus = "Color: BrightGreen"; statusChanged = true; break;
            case '0': currentColorIdx = 9; currentTool->setColor(colorPalette[9], Color::Black()); newStatus = "Color: BrightYellow"; statusChanged = true; break;

            case 'p': case 'P': currentTool = &pencilTool; newStatus = "Pencil"; statusChanged = true; awaitingSecondPoint = false; break;
            case 'e': case 'E': currentTool = &eraserTool; newStatus = "Eraser"; statusChanged = true; awaitingSecondPoint = false; break;
            case 'l': case 'L': currentTool = &lineTool; newStatus = "Line (2 clicks)"; statusChanged = true; awaitingSecondPoint = false; break;
            case 'r': case 'R': currentTool = &rectTool; newStatus = "Rectangle (2 clicks)"; statusChanged = true; awaitingSecondPoint = false; break;
            case 'f': case 'F': currentTool = &filledRectTool; newStatus = "Filled Rect (2 clicks)"; statusChanged = true; awaitingSecondPoint = false; break;
            case 'o': case 'O': currentTool = &ellipseTool; newStatus = "Ellipse (2 clicks)"; statusChanged = true; awaitingSecondPoint = false; break;
            case 'g': case 'G': currentTool = &floodTool; newStatus = "Flood Fill"; statusChanged = true; awaitingSecondPoint = false; break;

            case '+': case '=': 
                canvas.addLayer(); 
                newStatus = "Layer added: " + std::to_string(canvas.getLayerCount());
                statusChanged = true;
                canvasChanged = true;
                break;
            case '-': case '_': 
                if (canvas.getLayerCount() > 1) {
                    canvas.removeLayer(canvas.getActiveLayerIndex());
                    newStatus = "Layer removed";
                    statusChanged = true;
                    canvasChanged = true;
                } else {
                    newStatus = "Cannot remove last layer";
                    statusChanged = true;
                }
                break;
            case '[': case '{':
                if (canvas.getActiveLayerIndex() > 0) {
                    canvas.setActiveLayer(canvas.getActiveLayerIndex() - 1);
                    newStatus = "Layer: " + std::to_string(canvas.getActiveLayerIndex() + 1);
                    statusChanged = true;
                }
                break;
            case ']': case '}':
                if (canvas.getActiveLayerIndex() < canvas.getLayerCount() - 1) {
                    canvas.setActiveLayer(canvas.getActiveLayerIndex() + 1);
                    newStatus = "Layer: " + std::to_string(canvas.getActiveLayerIndex() + 1);
                    statusChanged = true;
                }
                break;
            
            case ' ': case '\n': {
                if (cursorX <= 0 || cursorX >= canvas.getWidth() - 1 ||
                    cursorY <= 0 || cursorY >= canvas.getHeight() - 1) {
                    newStatus = "Cannot draw on borders!";
                    statusChanged = true;
                    break;
                }
                
                if (awaitingSecondPoint) {
                    Command* cmd = currentTool->createCommand(&canvas, firstX, firstY, cursorX, cursorY);
                    if (cmd) { 
                        history->pushCommand(cmd); 
                        cmd->execute(&canvas); 
                    }
                    awaitingSecondPoint = false;
                    newStatus = "Shape drawn!";
                    canvasChanged = true;
                } else {
                    std::string name = currentTool->getName();
                    
                    if (name == "Line" || name == "Rectangle" || name == "FilledRect" || name == "Ellipse") {
                        firstX = cursorX; 
                        firstY = cursorY;
                        awaitingSecondPoint = true;
                        newStatus = "Click second point...";
                    } else {
                        Command* cmd = currentTool->createCommand(&canvas, cursorX, cursorY, cursorX, cursorY);
                        if (cmd) { 
                            history->pushCommand(cmd); 
                            cmd->execute(&canvas); 
                        }
                        newStatus = "Drawn!";
                        canvasChanged = true;
                    }
                }
                break;
            }

            case 'z': case 'Z': 
                if (history->undo(&canvas)) {
                    newStatus = "Undo!";
                    canvasChanged = true;
                } else {
                    newStatus = "Nothing to undo";
                    statusChanged = true;
                }
                break;
            case 'y': case 'Y': 
                if (history->redo(&canvas)) {
                    newStatus = "Redo!";
                    canvasChanged = true;
                } else {
                    newStatus = "Nothing to redo";
                    statusChanged = true;
                }
                break;
                
            case 's': case 'S': { 
                TXTExporter exp; 
                newStatus = canvas.exportTo(&exp, "output.txt") ? "Saved output.txt" : "Save failed"; 
                statusChanged = true;
                break; 
            }
            case 'a': case 'A': { 
                ANSIExporter exp; 
                newStatus = canvas.exportTo(&exp, "output.ans") ? "Saved output.ans" : "Save failed"; 
                statusChanged = true;
                break; 
            }
            case 'c': case 'C': { 
                CSVExporter exp; 
                newStatus = canvas.exportTo(&exp, "output.csv") ? "Saved output.csv" : "Save failed"; 
                statusChanged = true;
                break; 
            }
            case 'j': case 'J': { 
                JSONExporter exp; 
                newStatus = canvas.exportTo(&exp, "output.json") ? "Saved output.json" : "Save failed"; 
                statusChanged = true;
                break; 
            }
            case 'v': case 'V': {  
                ASCPExporter exp; 
                newStatus = canvas.exportTo(&exp, "project.ascp") ? "Saved project.ascp" : "Save failed"; 
                statusChanged = true;
                break; 
            }
            case 'i': case 'I': {
                if (ASCPImporter::validate("project.ascp")) {
                    Canvas* loaded = ASCPImporter::importFrom("project.ascp", history);
                    if (loaded) {
                        // Удаление слоев
                        int layerCount = canvas.getLayerCount();
                        for (int i = layerCount - 1; i >= 0; i--) {
                            canvas.removeLayer(i);
                        }
                        
                        // Добавление слоев из загруженного холста
                        for (int l = 0; l < loaded->getLayerCount(); l++) {
                            canvas.addLayer();
                            Layer* dst = canvas.getActiveLayer();
                            Layer* src = loaded->getLayer(l);
                            
                            if (src && dst) {
                                dst->setName(src->getName());
                                dst->setVisible(src->isVisible());
                                
                                int maxY = std::min(loaded->getHeight(), canvas.getHeight());
                                int maxX = std::min(loaded->getWidth(), canvas.getWidth());
                                
                                for (int y = 0; y < maxY; y++) {
                                    for (int x = 0; x < maxX; x++) {
                                        dst->setCell(x, y, src->getCell(x, y));
                                    }
                                }
                            }
                        }
                        newStatus = "Loaded project.ascp";
                        canvasChanged = true;
                        delete loaded;
                    } else {
                        newStatus = "Load failed";
                        statusChanged = true;
                    }
                } else {
                    newStatus = "Invalid project.ascp";
                    statusChanged = true;
                }
                break;
            }
        }

        // Очистка старого курсора
        clearCursorAt(prevCursorX, prevCursorY, canvas);
        
        // Если холст изменился - перерисовка
        if (canvasChanged) {
            renderCanvasOnly(canvas);
        }

        // Обновление статуса
        if (statusChanged && !newStatus.empty()) {
            updateStatusOnly(canvas.getHeight(), canvas.getWidth(),
                            currentTool->getName(), 
                            cursorX, cursorY, 
                            history->getUndoStackSize(), 
                            history->getRedoStackSize(), 
                            newStatus, currentColorIdx, 
                            canvas.getActiveLayerIndex());
        }

        // Отрисовка нового курсора
        drawCursorAt(cursorX, cursorY, canvas);
        
        // Перемещение терминального курсора
        std::cout << "\033[" << (cursorY + 2) << ";" << (cursorX + 1) << "H" << std::flush;
        
        prevCursorX = cursorX;
        prevCursorY = cursorY;
    }

    Terminal::clear();
    std::cout << "Goodbye!\n";
    return 0;
}