#include "Button.hpp"

Button::Button()
    : Label()
    , param(0)
    , callback(nullptr)
{
}

Button::Button(LayoutConstraint buttonConstraint, float offsetX, float offsetY, string text)
    : Label(buttonConstraint, offsetX, offsetY, text)
    , param(0)
    , callback(nullptr)
{
}

Button::Button(LayoutConstraint buttonConstraint, float offsetX, float offsetY, string text, int param, void (*callback)(int))
    : Label(buttonConstraint, offsetX, offsetY, text)
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

void Button::update(bool isMousePressed, Vector2 mousePos, int screenWidth, int screenHeight)
{
    Rectangle rectangle = getRectangle(screenWidth, screenHeight);
    if (callback != nullptr && isMousePressed && CheckCollisionPointRec(mousePos, rectangle)) {
        callback(param);
    }
}
