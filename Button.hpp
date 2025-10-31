#pragma once

#include "Data.hpp"
#include "Label.hpp"

class Button : Label {
public:
    // Конструктор по умолчанию
    Button();

    // Конструктор
    Button(LayoutConstraint buttonConstraint, float offsetX, float offsetY, string text);

    // Конструктор
    Button(LayoutConstraint buttonConstraint, float offsetX, float offsetY, string text, int param, void (*callback)(int));

    // Метод для отрисовки кнопки
    void draw(int screenWidth, int screenHeight);

    // Метод для обновления кнопки
    void update(bool isMousePressed, Vector2 mousePos, int screenWidth, int screenHeight);

private:
    // Параметр для обработчика события нажатия
    int param;

    // Обработчик события нажатия для кнопки
    void (*callback)(int);
};