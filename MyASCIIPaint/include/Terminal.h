#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>
#include <utility>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/ioctl.h>
#endif

class Terminal {
public:
    static void enableRawMode();
    static void disableRawMode();
    static int getKey();
    static std::pair<int, int> getSize();
    static void clear();
    static void moveCursor(int x, int y);
    static void hideCursor();
    static void showCursor();
    static void saveCursorPosition();
    static void restoreCursorPosition();
};

#endif