#include "Terminal.h"
#include <iostream>

void Terminal::enableRawMode() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hIn, &mode);
    SetConsoleMode(hIn, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));
#else
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
#endif
    hideCursor();
}

void Terminal::disableRawMode() {
    showCursor();
#ifndef _WIN32
    struct termios orig;
    tcgetattr(STDIN_FILENO, &orig);
    orig.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig);
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
    int ch = getchar();
    if (ch == 27) { // ESC-последовательности Linux/macOS
        if (getchar() == '[') {
            switch(getchar()) {
                case 'A': return 259; // UP
                case 'B': return 258; // DOWN
                case 'D': return 260; // LEFT
                case 'C': return 261; // RIGHT
            }
        }
        return 27;
    }
    return ch == EOF ? 0 : ch;
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
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return {w.ws_col, w.ws_row};
#endif
}

void Terminal::clear() {
    std::cout << "\033[H\033[J" << std::flush;
}

void Terminal::moveCursor(int x, int y) {
    std::cout << "\033[" << y << ";" << x << "H" << std::flush;
}

void Terminal::hideCursor() {
    std::cout << "\033[?25l" << std::flush;
}

void Terminal::showCursor() {
    std::cout << "\033[?25h" << std::flush;
}

void Terminal::saveCursorPosition() {
    std::cout << "\033[s";
}

void Terminal::restoreCursorPosition() {
    std::cout << "\033[u";
}