#pragma once

#include "Data.hpp"

enum ButtonConstraint {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_RIGHT,
    BOTTOM_LEFT
};

class Button {
public:
    // Конструктор по умолчанию
    Button();

    // Конструктор
    Button(ButtonConstraint buttonConstraint, float offsetX, float offsetY, string text);

    // Конструктор
    Button(ButtonConstraint buttonConstraint, float offsetX, float offsetY, string text, int param, void (*callback)(int));

    // Метод для отрисовки кнопки
    void draw(int screenWidth, int screenHeight);

    // Метод для обновления кнопки
    void update(bool isMousePressed, Vector2 mousePos, int screenWidth, int screenHeight);

private:
    // Положение кнопки
    ButtonConstraint constraint;

    // Ширина кнопки (0..1) относительно ширины окна
    float width;
    // Высота кнопки (0..1) относительно высота окна
    float height;

    // Смещение по X кнопки (0..1) относительно ширины окна
    float offsetX;
    // Смещение по Y кнопки (0..1) относительно высота окна
    float offsetY;

    // Текст, отображаемый на кнопке
    string text;

    // Параметр для обработчика события нажатия
    int param;

    // Обработчик события нажатия для кнопки
    void (*callback)(int);

private:
    Rectangle getRectangle(int screenWidth, int screenHeight);
};