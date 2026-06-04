#include "Terminal.h"
#include <iostream>
#include <cstring>

#ifndef _WIN32
struct termios Terminal::orig_termios;
#endif

void Terminal::enableRawMode() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hIn, &mode);
    SetConsoleMode(hIn, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT));
#else
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
#endif
    
    // Включаем альтернативный буфер экрана
    setAlternateScreen(true);
    hideCursor();
    clear();
}

void Terminal::disableRawMode() {
    showCursor();
    setAlternateScreen(false);
    clear();
    
#ifndef _WIN32
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
#endif
}

int Terminal::getKey() {
#ifdef _WIN32
    if (_kbhit()) {
        int ch = _getch();
        if (ch == 0 || ch == 224) { // Спецклавиши Windows
            ch = _getch();
            switch(ch) {
                case 72: return 259; // UP
                case 80: return 258; // DOWN
                case 75: return 260; // LEFT
                case 77: return 261; // RIGHT
                default: return ch;
            }
        }
        return ch;
    }
    return 0;
#else
    unsigned char ch;
    ssize_t n = read(STDIN_FILENO, &ch, 1);
    
    if (n <= 0) return 0;
    if (ch == 27) { // ESC-последовательности
        unsigned char seq[3];
        if (read(STDIN_FILENO, &seq[0], 1) > 0 && seq[0] == '[') {
            if (read(STDIN_FILENO, &seq[1], 1) > 0) {
                switch(seq[1]) {
                    case 'A': return 259; // UP
                    case 'B': return 258; // DOWN
                    case 'D': return 260; // LEFT
                    case 'C': return 261; // RIGHT
                    default: return 27;
                }
            }
        }
        return 27;
    }
    return ch;
#endif
}

std::pair<int, int> Terminal::getSize() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return {csbi.srWindow.Right - csbi.srWindow.Left + 1, 
            csbi.srWindow.Bottom - csbi.srWindow.Top + 1};
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        return {80, 24}; // Значения по умолчанию
    }
    return {w.ws_col, w.ws_row};
#endif
}

void Terminal::clear() {
    // ESC[2J = очистить весь экран
    // ESC[H = переместить курсор в 0,0
    std::cout << "\033[2J\033[H" << std::flush;
}

void Terminal::clearLine() {
    // ESC[K = очистить строку от курсора до конца
    std::cout << "\033[K" << std::flush;
}

void Terminal::moveCursor(int x, int y) {
    // ANSI: ESC[y;xH (1-индексированные координаты)
    std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H" << std::flush;
}

void Terminal::hideCursor() {
    std::cout << "\033[?25l" << std::flush;
}

void Terminal::showCursor() {
    std::cout << "\033[?25h" << std::flush;
}

void Terminal::saveCursorPosition() {
    std::cout << "\033[s" << std::flush;
}

void Terminal::restoreCursorPosition() {
    std::cout << "\033[u" << std::flush;
}

void Terminal::setAlternateScreen(bool enable) {
    if (enable) {
        // Переключиться на альтернативный буфер экрана
        std::cout << "\033[?1049h" << std::flush;
    } else {
        // Вернуться к основному буферу
        std::cout << "\033[?1049l" << std::flush;
    }
}