#include "GRenderer.hpp"
#include "Colors.hpp"
#include <algorithm>

GRenderer::GRenderer(const char* title)
    : inputActive(false), blinkTimer(0.0), showCursor(true)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(W, H, title);
    SetTargetFPS(60);
}

GRenderer::~GRenderer()
{
    CloseWindow();
}

bool GRenderer::ShouldClose()
{
    return WindowShouldClose();
}

void GRenderer::BeginFrame()
{
    BeginDrawing();
}

void GRenderer::EndFrame()
{
    EndDrawing();
}

double GRenderer::GetTime()
{
    return ::GetTime();
}

void GRenderer::Clear(Color bg)
{
    ClearBackground(bg);
}

void GRenderer::DrawRect(int x, int y, int w, int h, Color c)
{
    DrawRectangle(x, y, w, h, c);
}

void GRenderer::DrawRectLines(int x, int y, int w, int h, Color c, int thick)
{
    DrawRectangleLines(x, y, w, h, c);
}

void GRenderer::DrawText(const std::string& text, int x, int y, int size, Color c)
{
    ::DrawText(text.c_str(), x, y, size, c);
}

void GRenderer::DrawCenteredText(const std::string& text, int y, int size, Color c)
{
    int tw = MeasureText(text.c_str(), size);
    ::DrawText(text.c_str(), (W - tw) / 2, y, size, c);
}

void GRenderer::DrawRightText(const std::string& text, int rx, int y, int size, Color c)
{
    int tw = MeasureText(text.c_str(), size);
    ::DrawText(text.c_str(), rx - tw, y, size, c);
}

void GRenderer::DrawPanel(int x, int y, int w, int h, const std::string& title)
{
    DrawRect(x, y, w, h, CQColors::BgPanel);
    DrawRectLines(x, y, w, h, CQColors::BorderLight, 1);
    if (!title.empty())
    {
        DrawRect(x, y, w, 30, CQColors::BtnBg);
        DrawText(title, x + 8, y + 5, 18, CQColors::TextGold);
    }
}

void GRenderer::DrawBar(int val, int max, int x, int y, int w, int h, Color fg, Color bg)
{
    DrawRect(x, y, w, h, bg);
    if (max > 0)
    {
        float pct = std::min(1.0f, static_cast<float>(val) / static_cast<float>(max));
        DrawRect(x + 2, y + 2, static_cast<int>((w - 4) * pct), h - 4, fg);
    }
}

void GRenderer::DrawBarLabeled(int val, int max, int x, int y, int w, int h,
                                Color fg, Color bg, const std::string& label)
{
    DrawBar(val, max, x, y, w, h, fg, bg);
    std::string txt = label + " " + std::to_string(val) + "/" + std::to_string(max);
    int tw = MeasureText(txt.c_str(), 16);
    DrawText(txt, x + (w - tw) / 2, y + (h - 16) / 2, 16, CQColors::TextLight);
}

bool GRenderer::IsMouseInRect(int x, int y, int w, int h) const
{
    Vector2 mp = GetMousePosition();
    return CheckCollisionPointRec(mp, { static_cast<float>(x), static_cast<float>(y),
                                        static_cast<float>(w), static_cast<float>(h) });
}

bool GRenderer::IsMouseClickedOn(int x, int y, int w, int h) const
{
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && IsMouseInRect(x, y, w, h);
}

bool GRenderer::Button(const std::string& text, int x, int y, int w, int h)
{
    bool hover = IsMouseInRect(x, y, w, h);
    Color bg = hover ? CQColors::BtnHover : CQColors::BtnBg;
    bool clicked = IsMouseClickedOn(x, y, w, h);

    DrawRect(x, y, w, h, bg);
    DrawRectLines(x, y, w, h, CQColors::BtnBorder, 1);

    std::string displayText = text;
    auto pos = displayText.find("##");
    if (pos != std::string::npos)
        displayText = displayText.substr(0, pos);

    int tw = MeasureText(displayText.c_str(), 18);
    ::DrawText(displayText.c_str(), x + (w - tw) / 2, y + (h - 18) / 2, 18, CQColors::BtnText);

    return clicked;
}

int GRenderer::ButtonList(const std::vector<std::string>& items, int x, int y,
                           int w, int h, int spacing)
{
    for (size_t i = 0; i < items.size(); ++i)
    {
        int by = y + static_cast<int>(i) * (h + spacing);
        if (Button(items[i], x, by, w, h))
            return static_cast<int>(i);
    }
    return -1;
}

std::string GRenderer::InputBox(const std::string& label, int x, int y, int w)
{
    if (!inputActive)
    {
        inputBuf.clear();
        inputActive = true;
        blinkTimer = 0.0;
        showCursor = true;
    }

    if (!label.empty())
        DrawText(label, x, y - 22, 18, CQColors::TextLight);
    DrawRect(x, y, w, 35, CQColors::BgInput);
    DrawRectLines(x, y, w, 35, CQColors::BorderLight, 1);

    // Read character input
    int c = GetCharPressed();
    while (c > 0)
    {
        if (c >= 32 && c <= 126)
            inputBuf += static_cast<char>(c);
        c = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !inputBuf.empty())
        inputBuf.pop_back();

    // Blink cursor
    blinkTimer += ::GetFrameTime();
    if (blinkTimer >= 0.5)
    {
        blinkTimer -= 0.5;
        showCursor = !showCursor;
    }

    std::string display = inputBuf;
    if (showCursor)
        display += "_";

    DrawText(display, x + 5, y + 8, 18, CQColors::TextLight);

    if (IsKeyPressed(KEY_ENTER))
    {
        inputActive = false;
        return inputBuf;
    }
    return "";
}
