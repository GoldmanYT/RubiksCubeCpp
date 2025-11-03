#pragma once

#include "Data.hpp"

class OrbitalCamera : public Camera {
public:
    // Конструктор по умолчанию
    OrbitalCamera();

    // Метод для пересчёта позиции
    void recalculatePosition();

    // Метод для обновления положения камеры
    void update();

private:
    // Угол поворота камеры по горизонтали
    float angleX;

    // Угол поворота камеры по вертикали
    float angleY;

    // Расстояние до точки (0, 0, 0)
    float radius;

    // Нажата ли ЛКМ
    bool mouseDown;

    // Чувствительность камеры по оси X
    const float SENSIVITY_X = 0.005f;

    // Чувствительность камеры по оси Y
    const float SENSIVITY_Y = 0.005f;
};