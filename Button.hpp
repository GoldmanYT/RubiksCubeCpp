#include "Data.hpp"

enum ButtonConstraint {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_RIGHT,
    BOTTOM_LEFT
}

class Button {
public:
    // Конструктор по умолчанию
    Button();

    // Конструктор:
    //  buttonConstraint - положение кнопки
    //  width - ширина кнопки (0..1) относительно ширины окна
    //  height - высота кнопки (0..1) относительно высота окна
    //  text - текст, отображаемый на кнопке
    Button(ButtonConstraint buttonConstraint, float width, float height, string text);

    // Метод для отрисовки кнопки
    void draw(int screenWidth, int screenHeight);

private:
    // Положение кнопки
    ButtonConstraint constraint;

    // Ширина кнопки (0..1) относительно ширины окна
    float width;
    // Высота кнопки (0..1) относительно высота окна
    float height;

    // Текст, отображаемый на кнопке
    string text;
};