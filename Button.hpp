#pragma once

#include "Data.hpp"
#include "Label.hpp"

class Button : public Label {
public:
    // Конструктор по умолчанию
    Button();

    // Конструктор
    Button(LayoutConstraint buttonConstraint, float offsetX, float offsetY, string text, bool symbols = false);

    // Конструктор
    Button(LayoutConstraint buttonConstraint, float offsetX, float offsetY, string text, int param, void (*callback)(int), bool symbols = false);

    // Конструктор
    Button(LayoutConstraint buttonConstraint,
        float width, float height, float offsetX, float offsetY,
        string text, int param, void (*callback)(int), bool symbols = false);

    // Метод для отрисовки кнопки
    void draw(int screenWidth, int screenHeight);

    // Метод для обновления кнопки
    // Возарвщает true, если кнопка была нажата
    bool update(bool isMousePressed, Vector2 mousePos, int screenWidth, int screenHeight);

private:
    // Параметр для обработчика события нажатия
    int param;

    // Обработчик события нажатия для кнопки
    void (*callback)(int);
};