#include "OrbitalCamera.hpp"

OrbitalCamera::OrbitalCamera()
    : angleX(PI / 4)
    , angleY(PI / 4)
    , radius(sqrt(3.0f))
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
        angleX += mouseDelta.x * SENSIVITY_X;
        angleY += mouseDelta.y * SENSIVITY_Y;
        recalculatePosition();
    }
}
