#ifndef GRENDERER_HPP
#define GRENDERER_HPP

#include <string>
#include <vector>
#include "raylib.h"

class GRenderer
{
public:
    static constexpr int W = 1024;
    static constexpr int H = 768;

    GRenderer(const char* title);
    ~GRenderer();

    bool ShouldClose();
    void BeginFrame();
    void EndFrame();
    double GetTime();

    // ---- Drawing primitives ----
    void Clear(Color bg);
    void DrawRect(int x, int y, int w, int h, Color c);
    void DrawRectLines(int x, int y, int w, int h, Color c, int thick = 2);
    void DrawText(const std::string& text, int x, int y, int size, Color c);
    void DrawCenteredText(const std::string& text, int y, int size, Color c);
    void DrawRightText(const std::string& text, int rx, int y, int size, Color c);

    // ---- UI components ----
    void DrawPanel(int x, int y, int w, int h, const std::string& title = "");
    void DrawBar(int val, int max, int x, int y, int w, int h, Color fg, Color bg);
    void DrawBarLabeled(int val, int max, int x, int y, int w, int h, Color fg, Color bg, const std::string& label);

    // ---- Interactive ----
    bool Button(const std::string& text, int x, int y, int w, int h);
    int  ButtonList(const std::vector<std::string>& items, int x, int y,
                    int w = 260, int h = 42, int spacing = 6);

    // ---- Text input ----
    std::string InputBox(const std::string& label, int x, int y, int w = 300);

    // ---- Mouse helpers ----
    bool IsMouseInRect(int x, int y, int w, int h) const;
    bool IsMouseClickedOn(int x, int y, int w, int h) const;

    // ---- Helpers ----
    int  CenterX(int w) const { return (W - w) / 2; }
    int  GetW() const { return W; }
    int  GetH() const { return H; }

private:
    bool inputActive;
    std::string inputBuf;
    double blinkTimer;
    bool showCursor;
};

#endif
