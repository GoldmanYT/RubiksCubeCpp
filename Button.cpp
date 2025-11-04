#include "Button.hpp"

Button::Button()
    : Label()
    , param(0)
    , callback(nullptr)
{
}

Button::Button(LayoutConstraint buttonConstraint, float offsetX, float offsetY, string text, bool symbols)
    : Label(buttonConstraint, offsetX, offsetY, text, symbols)
    , param(0)
    , callback(nullptr)
{
}

Button::Button(LayoutConstraint buttonConstraint, float offsetX, float offsetY, string text, int param, void (*callback)(int), bool symbols)
    : Label(buttonConstraint, offsetX, offsetY, text, symbols)
    , param(param)
    , callback(callback)
{
}

Button::Button(LayoutConstraint buttonConstraint,
    float width, float height, float offsetX, float offsetY,
    string text, int param, void (*callback)(int), bool symbols)
    : Label(buttonConstraint, width, height, offsetX, offsetY, text, symbols)
    , param(param)
    , callback(callback)
{
}

void Button::draw(int screenWidth, int screenHeight)
{
    Rectangle rectangle = getRectangle(screenWidth, screenHeight);

    DrawRectangleRec(rectangle, colors[BUTTON_COLOR]);

    Label::draw(screenWidth, screenHeight);
}

bool Button::update(bool isMousePressed, Vector2 mousePos, int screenWidth, int screenHeight)
{
    Rectangle rectangle = getRectangle(screenWidth, screenHeight);

    if (callback != nullptr && isMousePressed && CheckCollisionPointRec(mousePos, rectangle)) {
        callback(param);
        return true;
    }

    return false;
}
