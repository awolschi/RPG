#ifndef KEYBOARDNAV_HPP
#define KEYBOARDNAV_HPP

#include <vector>
#include "raylib.h"

class KeyboardNav
{
public:
    KeyboardNav();

    void Reset();
    void SetFocusCount(int count);
    int  GetFocus() const;
    void SetFocus(int index);

    // Call once per frame to process input. Returns:
    //   -1: no action
    //   -2: confirm (Enter/Space)
    //   -3: back (Escape)
    //  >=0: the index of a confirmed element (same as -2 but with index)
    int  Update();

    // Draw a focus highlight around a rect
    void DrawFocusRect(int x, int y, int w, int h) const;
    void DrawFocusRect(int x, int y, int w, int h, Color tint) const;

    bool IsFocused(int index) const;

private:
    int focusIndex;
    int focusCount;
    double lastMoveTime;
    static constexpr double COOLDOWN = 0.08; // seconds between key repeats
};

#endif
