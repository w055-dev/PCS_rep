#ifndef COLOR_H
#define COLOR_H

#include <string>

class Color {
private:
    int ansiCode;
    int r, g, b;
    
public:
    Color();
    explicit Color(int ansiCode);
    Color(int r, int g, int b);
    
    int getAnsiCode() const;
    void setAnsiCode(int code);
    std::string toAnsi() const;
    
    static Color Black();
    static Color Red();
    static Color Green();
    static Color Yellow();
    static Color Blue();
    static Color Magenta();
    static Color Cyan();
    static Color White();
    static Color BrightBlack();
    static Color BrightRed();
    static Color BrightGreen();
    static Color BrightYellow();
    static Color BrightBlue();
    static Color BrightMagenta();
    static Color BrightCyan();
    static Color BrightWhite();
    
    bool operator==(const Color& other) const;
    bool operator!=(const Color& other) const;
};

#endif