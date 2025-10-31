#include "Label.hpp"

Label::Label()
    : constraint(TOP_LEFT)
    , width(DEFAULT_LAYOUT_WIDTH)
    , height(DEFAULT_LAYOUT_HEIGHT)
    , offsetX(DEFAULT_LAYOUT_OFFSET)
    , offsetY(DEFAULT_LAYOUT_OFFSET)
    , symbols(false)
{
}

Label::Label(LayoutConstraint labelConstraint, float offsetX, float offsetY, string text, bool symbols)
    : constraint(labelConstraint)
    , width(DEFAULT_LAYOUT_WIDTH)
    , height(DEFAULT_LAYOUT_HEIGHT)
    , offsetX(offsetX)
    , offsetY(offsetY)
    , text(text)
    , symbols(symbols)
{
}

Label::Label(LayoutConstraint labelConstraint, float width, float height, float offsetX, float offsetY, string text, bool symbols)
    : constraint(labelConstraint)
    , width(width)
    , height(height)
    , offsetX(offsetX)
    , offsetY(offsetY)
    , text(text)
    , symbols(symbols)
{
}

void Label::draw(int screenWidth, int screenHeight)
{
    Font labelFont = symbols ? symbolFont : font;
    Rectangle rectangle = getRectangle(screenWidth, screenHeight);
    Vector2 textSize = MeasureTextEx(labelFont, text.data(), FONT_SIZE, 0.0f);
    Vector2 pos = { rectangle.x + (rectangle.width - textSize.x) / 2.0f,
        rectangle.y + (rectangle.height - textSize.y) / 2.0f };

    DrawTextEx(
        labelFont, text.data(), pos,
        (int)FONT_SIZE, 0.0f, colors[FONT_COLOR]);
}

Rectangle Label::getRectangle(int screenWidth, int screenHeight)
{
    float layoutX = 0.0f;
    float layoutY = 0.0f;
    float layoutWidth = width * screenWidth;
    float layoutHeight = height * screenHeight;

    switch (constraint) {
    case TOP_LEFT:
        layoutX = screenWidth * offsetX;
        layoutY = screenHeight * offsetY;
        break;
    case TOP_RIGHT:
        layoutX = screenWidth * (1 - width - offsetX);
        layoutY = screenHeight * offsetY;
        break;
    case BOTTOM_RIGHT:
        layoutX = screenWidth * (1 - width - offsetX);
        layoutY = screenHeight * (1 - height - offsetY);
        break;
    case BOTTOM_LEFT:
        layoutX = screenWidth * offsetX;
        layoutY = screenHeight * (1 - height - offsetY);
        break;
    }

    return Rectangle { layoutX, layoutY, layoutWidth, layoutHeight };
}

void Label::setText(string text)
{
    this->text = text;
}
