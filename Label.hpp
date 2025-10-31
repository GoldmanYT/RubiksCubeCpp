#pragma once

#include "Data.hpp"

enum LayoutConstraint {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_RIGHT,
    BOTTOM_LEFT
};

class Label {
public:
    // Конструктор по умолчанию
    Label();

    // Конструктор
    Label(LayoutConstraint labelConstraint, float offsetX, float offsetY, string text, bool symbols = false);

    // Конструктор
    Label(LayoutConstraint labelConstraint, float width, float height, float offsetX, float offsetY, string text, bool symbols = false);

    // Метод для отрисовки надписи
    void draw(int screenWidth, int screenHeight);

    // Метод для получения границ надписи
    Rectangle getRectangle(int screenWidth, int screenHeight);

    // Метод для установки текста
    void setText(string text);

protected:
    // Положение надписи
    LayoutConstraint constraint;

    // Ширина надписи (0..1) относительно ширины окна
    float width;
    // Высота надписи (0..1) относительно высота окна
    float height;

    // Смещение по X надписи (0..1) относительно ширины окна
    float offsetX;
    // Смещение по Y надписи (0..1) относительно высота окна
    float offsetY;

    // Текст надписи
    string text;

    // Текст состоит из символов
    bool symbols;
};