#pragma once

#include "Data.hpp"

class OrbitalCamera : Camera {
public:
    // Конструктор по умолчанию
    OrbitalCamera();

    // Метод для изменения положения по размеру кубика Рубика
    void reset();

    // Метод для пересчёта позиции
    void recalculatePosition();

    // Метод для начала отрисовки
    void beginMode3D();

    // Метод для обновления положения камеры
    void update();

private:
    // Угол поворота камеры по горизонтали
    float angleX;

    // Угол поворота камеры по вертикали
    float angleY;

    // Расстояние до точки (0, 0, 0)
    float radius;

    // Чувствительность камеры по оси X
    const float SENSIVITY_X = 0.05f;

    // Чувствительность камеры по оси Y
    const float SENSIVITY_Y = 0.05f;
};