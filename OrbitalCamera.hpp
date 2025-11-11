#pragma once

#include "Data.hpp"

class OrbitalCamera : public Camera {
public:
    // Конструктор по умолчанию
    OrbitalCamera();

    // Метод для обновления положения камеры
    bool update(bool isMouseButtonDown);

    void rotateByAngle(Vector2 rotation);

private:
    // Нажата ли ЛКМ
    // Поле необходимо для сенсорных устройств
    bool mouseDown;

    // Чувствительность камеры
    const float SENSITIVITY = 0.005f;
};