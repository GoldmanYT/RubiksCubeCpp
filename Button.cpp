#include "Button.hpp"

Button::Button()
    : constraint(TOP_LEFT)
    , width(DEFAULT_BUTTON_WIDTH)
    , height(DEFAULT_BUTTON_HEIGHT)
    , offsetX(DEFAULT_BUTTON_OFFSET)
    , offsetY(DEFAULT_BUTTON_OFFSET)
    , param(0)
    , callback(nullptr)
{
}

Button::Button(ButtonConstraint buttonConstraint, float offsetX, float offsetY, string text)
    : constraint(buttonConstraint)
    , width(DEFAULT_BUTTON_WIDTH)
    , height(DEFAULT_BUTTON_HEIGHT)
    , text(text)
    , offsetX(offsetX)
    , offsetY(offsetY)
    , param(0)
    , callback(nullptr)
{
}

Button::Button(ButtonConstraint buttonConstraint, float offsetX, float offsetY, string text, int param, void (*callback)(int))
    : constraint(buttonConstraint)
    , width(DEFAULT_BUTTON_WIDTH)
    , height(DEFAULT_BUTTON_HEIGHT)
    , text(text)
    , offsetX(offsetX)
    , offsetY(offsetY)
    , param(param)
    , callback(callback)
{
}

void Button::draw(int screenWidth, int screenHeight)
{
    Rectangle rectangle = getRectangle(screenWidth, screenHeight);

    DrawRectangleRec(rectangle, colors[BUTTON_COLOR]);

    float textX, textY;
    Vector2 textSize = MeasureTextEx(font, text.data(), FONT_SIZE, 0.0f);

    textX = rectangle.x + (rectangle.width - textSize.x) / 2.0f;
    textY = rectangle.y + (rectangle.height - textSize.y) / 2.0f;

    DrawText(
        text.data(),
        (int)textX, (int)textY,
        (int)FONT_SIZE, colors[FONT_COLOR]);
}

void Button::update(bool isMousePressed, Vector2 mousePos, int screenWidth, int screenHeight)
{
    Rectangle rectangle = getRectangle(screenWidth, screenHeight);
    if (callback != nullptr && isMousePressed && CheckCollisionPointRec(mousePos, rectangle)) {
        callback(param);
    }
}

Rectangle Button::getRectangle(int screenWidth, int screenHeight)
{
    float buttonX = 0.0f;
    float buttonY = 0.0f;
    float buttonWidth = width * screenWidth;
    float buttonHeight = height * screenHeight;

    switch (constraint) {
    case TOP_LEFT:
        buttonX = screenWidth * offsetX;
        buttonY = screenHeight * offsetY;
        break;
    case TOP_RIGHT:
        buttonX = screenWidth * (1 - width - offsetX);
        buttonY = screenHeight * offsetY;
        break;
    case BOTTOM_RIGHT:
        buttonX = screenWidth * (1 - width - offsetX);
        buttonY = screenHeight * (1 - height - offsetY);
        break;
    case BOTTOM_LEFT:
        buttonX = screenWidth * offsetX;
        buttonY = screenHeight * (1 - height - offsetY);
        break;
    }

    return Rectangle { buttonX, buttonY, buttonWidth, buttonHeight };
}
