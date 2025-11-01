#include "OrbitalCamera.hpp"

OrbitalCamera::OrbitalCamera()
    : angleX(PI / 4)
    , angleY(PI / 4)
    , radius(3 * sqrt(3.0f))
    , Camera {
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        3.0f,
        CAMERA_ORTHOGRAPHIC
    }
{
    recalculatePosition();
}

void OrbitalCamera::reset()
{
    angleX = PI / 4;
    angleY = PI / 4;
    radius = sqrt(3.0f);
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

void OrbitalCamera::beginMode3D()
{
    BeginMode3D(static_cast<Camera>(*this)); // FIXME
}

void OrbitalCamera::update()
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 mouseDelta = GetMouseDelta();
        bool direction = (long long)floor(angleY / acos(-1)) % 2 != 0;

        angleX += mouseDelta.x * SENSIVITY_X * (direction ? -1.0f : 1.0f);
        angleY -= mouseDelta.y * SENSIVITY_Y;
        up.y = 1.0f - 2 * direction;

        recalculatePosition();
    }
}
