#ifndef GRENDERER_HPP
#define GRENDERER_HPP

#include <string>
#include <vector>
#include "raylib.h"
#include "TextureManager.hpp"

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
    void DrawCircle(int cx, int cy, float radius, Color c);
    void DrawCircleLines(int cx, int cy, float radius, Color c, int thick = 2);
    void DrawLine(int x1, int y1, int x2, int y2, Color c, int thick = 2);
    void DrawText(const std::string& text, int x, int y, int size, Color c);
    void DrawCenteredText(const std::string& text, int y, int size, Color c);
    void DrawRightText(const std::string& text, int rx, int y, int size, Color c);

    // ---- UI components ----
    void DrawPanel(int x, int y, int w, int h, const std::string& title = "");
    void DrawBar(int val, int max, int x, int y, int w, int h, Color fg, Color bg);
    void DrawBarLabeled(int val, int max, int x, int y, int w, int h, Color fg, Color bg, const std::string& label);

    // ---- Interactive ----
    bool Button(const std::string& text, int x, int y, int w, int h, int focusIndex = -1, int fontSize = 18);
    int  ButtonList(const std::vector<std::string>& items, int x, int y,
                    int w = 260, int h = 42, int spacing = 6, int focusStart = -1);

    // ---- Focus ----
    void SetCurrentFocus(int index);
    int  GetCurrentFocus() const;
    void DrawFocusHighlight(int x, int y, int w, int h);

    // ---- Text input ----
    std::string InputBox(const std::string& label, int x, int y, int w = 300);

    // ---- Texture drawing ----
    void DrawTexture(Texture2D tex, int x, int y, float scale = 1.0f);
    void DrawTextureCentered(Texture2D tex, int y, float scale = 1.0f);
    void DrawTextureFit(Texture2D tex, int x, int y, int w, int h);
    void DrawTextureCropped(Texture2D tex, Rectangle src, int x, int y, int w, int h);

    TextureManager textureManager;

    // ---- Font support ----
    void DrawTextFont(const std::string& text, int x, int y, int size, Color c, Font font);
    void DrawCenteredTextFont(const std::string& text, int y, int size, Color c, Font font);
    Font GetSerifFont() const;
    Font GetSansFont() const;

    // ---- Tooltip ----
    void DrawTooltip(const std::string& text, int mx, int my, int maxW = 280);
    bool IsMouseInRect(int x, int y, int w, int h) const;

    // ---- Transitions ----
    void StartTransition(float duration = 0.3f);
    void DrawTransition();
    bool IsMouseClickedOn(int x, int y, int w, int h) const;

    // ---- Slide-in animation ----
    void StartSlideIn();

    // ---- Helpers ----
    int  CenterX(int w) const { return (W - w) / 2; }
    int  GetW() const { return W; }
    int  GetH() const { return H; }

    float slideOffsetX;
    float slideOffsetY;

private:
    bool inputActive;
    std::string inputBuf;
    double blinkTimer;
    bool showCursor;
    int currentFocus;

    // Transition state
    bool transitionActive;
    float transitionTime;
    float transitionDuration;
    bool transitionFadingIn;

    Font serifFont;
    Font sansFont;
};

#endif
