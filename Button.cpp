#include "Button.hpp"

Button::Button()
    : constraint(TOP_LEFT)
    , width(DEFAULT_BUTTON_WIDTH)
    , height(DEFAULT_BUTTON_HEIGHT)
{
}

Button::Button(ButtonConstraint buttonConstraint, float width, float height, string text)
    : constraint(buttonConstraint)
    , width(width)
    , height(height)
    , text(text)
{
}


void Button::draw(int screenWidth, int screenHeight)
{
    float buttonX, buttonY;
    float buttonWidth = width * screenWidth;
    float buttonHeight = height * screenHeight;

    switch (constraint) {
    case TOP_LEFT:
        buttonX = BUTTON_OFFSET;
        buttonY = BUTTON_OFFSET;
        break;
    case TOP_RIGHT:
        buttonX = screenWidth * (1 - width) - BUTTON_OFFSET;
        buttonY = BUTTON_OFFSET;
        break;
    case BOTTOM_RIGHT:
        buttonX = screenWidth * (1 - width) - BUTTON_OFFSET;
        buttonY = screenHeight * (1 - height) - BUTTON_OFFSET;
        break;
    case BOTTOM_LEFT:
        buttonX = BUTTON_OFFSET;
        buttonY = screenHeight * (1 - height) - BUTTON_OFFSET;
        break;
    }

    DrawRectangle(buttonX, buttonY, buttonWidth, buttonHeight, colors[BUTTON_COLOR]);

    float textX, textY;
    Vector2 textSize = MeasureTextEx(font, text.data(), FONT_SIZE, 0.0f);

    textX = buttonX + buttonWidth / 2.0f - textSize.x;
    textY = buttonY + buttonHeight / 2.0f - textSize.y;

    DrawText(text.data(), textX, textY, FONT_SIZE, colors[TEXT_COLOR]);
}
