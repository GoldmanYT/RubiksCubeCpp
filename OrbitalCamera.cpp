#include "OrbitalCamera.hpp"

OrbitalCamera::OrbitalCamera()
    : Camera {
        Vector3Zeros,
        Vector3Zeros,
        Vector3UnitY,
        30.0f,
        CAMERA_PERSPECTIVE
    }
    , angleX(PI / 4)
    , angleY(PI / 4)
    , radius(3 * sqrt(3.0f))
    , mouseDown(false)
{
    recalculatePosition();
}

void OrbitalCamera::recalculatePosition()
{
    position = {
        radius * sin(angleY) * cos(angleX),
        radius * cos(angleY),
        radius * sin(angleY) * sin(angleX)
    };

    up = {
        -cos(angleY) * cos(angleX),
        sin(angleY),
        -cos(angleY) * sin(angleX)
    };
    up = Vector3Normalize(up);
}

bool OrbitalCamera::update(bool isMouseButtonDown)
{
    if (isMouseButtonDown) {
        Vector2 mouseDelta = mouseDown ? GetMouseDelta() : Vector2 { 0.0f, 0.0f };
        bool direction = (long long)floor(angleY / PI) % 2 != 0;
        mouseDown = true;

        angleX += mouseDelta.x * SENSITIVITY_X * (direction ? -1.0f : 1.0f);
        angleY -= mouseDelta.y * SENSITIVITY_Y;

        recalculatePosition();
        mouseDown = true;
    } else {
        mouseDown = false;
    }

    return mouseDown;
}
