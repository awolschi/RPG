#include "GRenderer.hpp"
#include "Colors.hpp"
#include <algorithm>
#include <cmath>
#include <string>

static std::string AssetPath(const std::string& relative)
{
    std::string appDir = GetApplicationDirectory();
    return appDir + "../" + relative;
}

GRenderer::GRenderer(const char* title)
    : inputActive(false), blinkTimer(0.0), showCursor(true), currentFocus(-1),
      slideOffsetX(0.0f), slideOffsetY(0.0f),
      transitionActive(false), transitionTime(0.0f), transitionDuration(0.3f), transitionFadingIn(false)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(W, H, title);
    SetTargetFPS(60);

    if (FileExists(AssetPath("assets/fonts/serif.ttf").c_str()))
        serifFont = LoadFont(AssetPath("assets/fonts/serif.ttf").c_str());
    else
        serifFont = GetFontDefault();

    if (FileExists(AssetPath("assets/fonts/sans.ttf").c_str()))
        sansFont = LoadFont(AssetPath("assets/fonts/sans.ttf").c_str());
    else
        sansFont = GetFontDefault();
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
    DrawRectangleLinesEx({(float)x, (float)y, (float)w, (float)h}, (float)thick, c);
}

void GRenderer::DrawCircle(int cx, int cy, float radius, Color c)
{
    ::DrawCircle(cx, cy, radius, c);
}

void GRenderer::DrawCircleLines(int cx, int cy, float radius, Color c, int thick)
{
    // raylib only has DrawCircleLines with fixed 1px thickness
    // Draw multiple concentric circles for thicker appearance
    for (int i = 0; i < thick; ++i)
        ::DrawCircleLines(cx, cy, radius + (float)i, c);
}

void GRenderer::DrawLine(int x1, int y1, int x2, int y2, Color c, int thick)
{
    ::DrawLineEx({(float)x1, (float)y1}, {(float)x2, (float)y2}, (float)thick, c);
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

void GRenderer::DrawTexture(Texture2D tex, int x, int y, float scale)
{
    if (tex.id == 0) return;
    DrawTextureEx(tex, { static_cast<float>(x), static_cast<float>(y) }, 0.0f, scale, WHITE);
}

void GRenderer::DrawTextureCentered(Texture2D tex, int y, float scale)
{
    if (tex.id == 0) return;
    int drawW = static_cast<int>(tex.width * scale);
    int x = (W - drawW) / 2;
    DrawTextureEx(tex, { static_cast<float>(x), static_cast<float>(y) }, 0.0f, scale, WHITE);
}

void GRenderer::DrawTextureFit(Texture2D tex, int x, int y, int w, int h)
{
    if (tex.id == 0) return;
    float scaleX = static_cast<float>(w) / static_cast<float>(tex.width);
    float scaleY = static_cast<float>(h) / static_cast<float>(tex.height);
    float scale = (scaleX < scaleY) ? scaleX : scaleY;
    int drawW = static_cast<int>(tex.width * scale);
    int drawH = static_cast<int>(tex.height * scale);
    int offX = x + (w - drawW) / 2;
    int offY = y + (h - drawH) / 2;
    DrawTextureEx(tex, { static_cast<float>(offX), static_cast<float>(offY) }, 0.0f, scale, WHITE);
}

void GRenderer::DrawTextureCropped(Texture2D tex, Rectangle src, int x, int y, int w, int h)
{
    if (tex.id == 0) return;
    Rectangle dst = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h) };
    DrawTexturePro(tex, src, dst, {0, 0}, 0.0f, WHITE);
}

void GRenderer::StartSlideIn()
{
    slideOffsetX = 200.0f;
    slideOffsetY = 0.0f;
}

void GRenderer::DrawTextFont(const std::string& text, int x, int y, int size, Color c, Font font)
{
    if (font.glyphCount == 0) return;
    DrawTextEx(font, text.c_str(), { static_cast<float>(x), static_cast<float>(y) }, static_cast<float>(size), 1.0f, c);
}

void GRenderer::DrawCenteredTextFont(const std::string& text, int y, int size, Color c, Font font)
{
    if (font.glyphCount == 0) return;
    Vector2 m = MeasureTextEx(font, text.c_str(), static_cast<float>(size), 1.0f);
    float x = (W - m.x) / 2.0f;
    DrawTextEx(font, text.c_str(), { x, static_cast<float>(y) }, static_cast<float>(size), 1.0f, c);
}

Font GRenderer::GetSerifFont() const { return serifFont; }
Font GRenderer::GetSansFont() const { return sansFont; }

void GRenderer::DrawPanel(int x, int y, int w, int h, const std::string& title)
{
    int sx = x + static_cast<int>(slideOffsetX);
    int sy = y + static_cast<int>(slideOffsetY);

    // Panel background with subtle gradient effect
    DrawRect(sx, sy, w, h, CQColors::BgPanel);

    // Inner highlight (top edge lighter)
    Color highlight = {60, 42, 28, 80};
    DrawRect(sx + 1, sy + 1, w - 2, 2, highlight);

    // Inner shadow (bottom edge darker)
    Color shadow = {15, 8, 5, 100};
    DrawRect(sx + 1, sy + h - 3, w - 2, 2, shadow);

    // Border
    DrawRectLines(sx, sy, w, h, CQColors::BorderLight, 1);

    if (!title.empty())
    {
        // Title bar with gradient effect
        DrawRect(sx, sy, w, 30, CQColors::BtnBg);
        Color titleHighlight = {100, 70, 45, 60};
        DrawRect(sx, sy, w, 1, titleHighlight);
        DrawText(title, sx + 8, sy + 5, 18, CQColors::TextGold);
    }

    // Smoothly interpolate slide offset toward 0
    float dt = GetFrameTime();
    slideOffsetX *= std::max(0.0f, 1.0f - dt * 12.0f);
    slideOffsetY *= std::max(0.0f, 1.0f - dt * 12.0f);
    if (std::abs(slideOffsetX) < 0.5f) slideOffsetX = 0.0f;
    if (std::abs(slideOffsetY) < 0.5f) slideOffsetY = 0.0f;
}

void GRenderer::DrawBar(int val, int max, int x, int y, int w, int h, Color fg, Color bg)
{
    // Bar background with inner shadow
    DrawRect(x, y, w, h, bg);
    Color innerShadow = {0, 0, 0, 60};
    DrawRect(x, y + h - 2, w, 2, innerShadow);

    if (max > 0)
    {
        float pct = std::min(1.0f, static_cast<float>(val) / static_cast<float>(max));
        int fillW = static_cast<int>((w - 4) * pct);
        if (fillW > 0)
        {
            // Main fill
            DrawRect(x + 2, y + 2, fillW, h - 4, fg);

            // Gradient highlight on top half
            Color highlight = {255, 255, 255, 40};
            DrawRect(x + 2, y + 2, fillW, (h - 4) / 2, highlight);

            // Darker edge on bottom
            Color edge = {0, 0, 0, 30};
            DrawRect(x + 2, y + h / 2, fillW, (h - 4) / 2, edge);
        }
    }
}

void GRenderer::DrawBarLabeled(int val, int max, int x, int y, int w, int h,
                                Color fg, Color bg, const std::string& label)
{
    DrawBar(val, max, x, y, w, h, fg, bg);
    int fontSize = (h >= 16) ? 14 : (h >= 14) ? 12 : 10;
    std::string txt = label + " " + std::to_string(val) + "/" + std::to_string(max);
    int tw = MeasureText(txt.c_str(), fontSize);
    DrawText(txt, x + (w - tw) / 2, y + (h - fontSize) / 2, fontSize, CQColors::TextLight);
}

void GRenderer::StartTransition(float duration)
{
    transitionActive = true;
    transitionTime = 0.0f;
    transitionDuration = duration;
    transitionFadingIn = true;
}

void GRenderer::DrawTransition()
{
    if (!transitionActive) return;

    transitionTime += GetFrameTime();
    float progress = transitionTime / transitionDuration;

    if (transitionFadingIn)
    {
        // Fade to black
        unsigned char alpha = static_cast<unsigned char>(255.0f * std::min(progress, 1.0f));
        DrawRect(0, 0, W, H, {0, 0, 0, alpha});

        if (progress >= 1.0f)
        {
            transitionFadingIn = false;
            transitionTime = 0.0f;
        }
    }
    else
    {
        // Fade from black
        unsigned char alpha = static_cast<unsigned char>(255.0f * (1.0f - std::min(progress, 1.0f)));
        DrawRect(0, 0, W, H, {0, 0, 0, alpha});

        if (progress >= 1.0f)
            transitionActive = false;
    }
}

void GRenderer::DrawTooltip(const std::string& text, int mx, int my, int maxW)
{
    if (text.empty()) return;

    // Word-wrap tooltip text into lines
    std::vector<std::string> lines;
    std::string currentLine;
    int fontSize = 14;
    int padding = 8;
    int lineH = fontSize + 4;

    for (size_t i = 0; i < text.size(); ++i)
    {
        char c = text[i];
        if (c == '\n')
        {
            lines.push_back(currentLine);
            currentLine.clear();
            continue;
        }
        currentLine += c;
        int w = MeasureText(currentLine.c_str(), fontSize);
        if (w > maxW - padding * 2)
        {
            // Find last space to break word
            size_t lastSpace = currentLine.rfind(' ');
            if (lastSpace != std::string::npos && lastSpace > 0)
            {
                lines.push_back(currentLine.substr(0, lastSpace));
                currentLine = currentLine.substr(lastSpace + 1);
            }
            else
            {
                lines.push_back(currentLine);
                currentLine.clear();
            }
        }
    }
    if (!currentLine.empty()) lines.push_back(currentLine);

    // Calculate tooltip size
    int tipW = maxW;
    int tipH = static_cast<int>(lines.size()) * lineH + padding * 2;

    // Position tooltip near cursor, keep on screen
    int tipX = mx + 14;
    int tipY = my - tipH - 8;
    if (tipX + tipW > W) tipX = mx - tipW - 14;
    if (tipY < 4) tipY = my + 20;
    if (tipX < 4) tipX = 4;
    if (tipY + tipH > H - 4) tipY = H - tipH - 4;

    // Semi-transparent background
    DrawRect(tipX, tipY, tipW, tipH, {10, 10, 15, 230});
    DrawRectLines(tipX, tipY, tipW, tipH, {100, 100, 120, 200});

    // Draw text lines
    for (size_t i = 0; i < lines.size(); ++i)
    {
        DrawText(lines[i], tipX + padding, tipY + padding + static_cast<int>(i) * lineH,
                 fontSize, CQColors::TextLight);
    }
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

bool GRenderer::Button(const std::string& text, int x, int y, int w, int h, int focusIndex, int fontSize)
{
    bool hover = IsMouseInRect(x, y, w, h);
    bool pressed = hover && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    bool focused = (focusIndex >= 0 && focusIndex == currentFocus);

    bool clicked = IsMouseClickedOn(x, y, w, h);

    // Keyboard activation: Enter or Space on focused button
    if (focused && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)))
        clicked = true;

    // Invisible button (empty text) — skip all rendering, only detect clicks
    if (text.empty())
        return clicked;

    Color bg;
    if (pressed)
        bg = CQColors::BtnActive;
    else if (hover)
        bg = CQColors::BtnHover;
    else
        bg = CQColors::BtnBg;

    // Button body
    DrawRect(x, y, w, h, bg);

    // Top highlight
    if (!pressed)
    {
        Color btnHighlight = {120, 85, 55, 50};
        DrawRect(x + 1, y + 1, w - 2, 1, btnHighlight);
    }

    // Border
    DrawRectLines(x, y, w, h, CQColors::BtnBorder, 1);

    // Hover glow
    if (hover && !pressed)
    {
        Color glow = {180, 140, 80, 30};
        DrawRect(x - 1, y - 1, w + 2, h + 2, glow);
    }

    // Draw focus highlight
    if (focused)
    {
        unsigned char alpha = static_cast<unsigned char>(
            180 + 75.0 * std::sin(::GetTime() * 4.0));
        Color focusColor = CQColors::FocusBorder;
        focusColor.a = alpha;
        DrawRectLines(x - 1, y - 1, w + 2, h + 2, focusColor);
        DrawRectLines(x - 2, y - 2, w + 4, h + 4, focusColor);
    }

    std::string displayText = text;
    auto pos = displayText.find("##");
    if (pos != std::string::npos)
        displayText = displayText.substr(0, pos);

    int tw = MeasureText(displayText.c_str(), fontSize);
    // Slight text offset when pressed
    int textOffY = pressed ? 1 : 0;
    ::DrawText(displayText.c_str(), x + (w - tw) / 2, y + (h - fontSize) / 2 + textOffY, fontSize, CQColors::BtnText);

    return clicked;
}

int GRenderer::ButtonList(const std::vector<std::string>& items, int x, int y,
                           int w, int h, int spacing, int focusStart)
{
    for (size_t i = 0; i < items.size(); ++i)
    {
        int by = y + static_cast<int>(i) * (h + spacing);
        int fi = (focusStart >= 0) ? focusStart + static_cast<int>(i) : -1;
        if (Button(items[i], x, by, w, h, fi))
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

void GRenderer::SetCurrentFocus(int index)
{
    currentFocus = index;
}

int GRenderer::GetCurrentFocus() const
{
    return currentFocus;
}

void GRenderer::DrawFocusHighlight(int x, int y, int w, int h)
{
    unsigned char alpha = static_cast<unsigned char>(
        180 + 75.0 * std::sin(::GetTime() * 4.0));
    Color focusColor = CQColors::FocusBorder;
    focusColor.a = alpha;
    DrawRectLines(x - 1, y - 1, w + 2, h + 2, focusColor);
    DrawRectLines(x - 2, y - 2, w + 4, h + 4, focusColor);
}
