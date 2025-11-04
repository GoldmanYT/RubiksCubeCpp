#include "OrbitalCamera.hpp"

OrbitalCamera::OrbitalCamera()
    : Camera {
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
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
}

bool OrbitalCamera::update(bool isMouseButtonDown)
{
    if (isMouseButtonDown) {
        Vector2 mouseDelta = mouseDown ? GetMouseDelta() : Vector2 { 0.0f, 0.0f };
        bool direction = (long long)floor(angleY / acos(-1)) % 2 != 0;
        mouseDown = true;

        angleX += mouseDelta.x * SENSIVITY_X * (direction ? -1.0f : 1.0f);
        angleY -= mouseDelta.y * SENSIVITY_Y;
        up.y = 1.0f - 2 * direction;

        recalculatePosition();
    } else {
        mouseDown = false;
    }

    return mouseDown;
}
