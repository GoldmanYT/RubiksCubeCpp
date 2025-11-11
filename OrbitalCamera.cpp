#include "OrbitalCamera.hpp"

OrbitalCamera::OrbitalCamera()
    : Camera {
        Vector3UnitX * 6.0f,
        Vector3Zeros,
        Vector3UnitY,
        30.0f,
        CAMERA_PERSPECTIVE
    }
    , mouseDown(false)
{
    Vector2 initialRotation = { PI / 4, PI / 4 };

    rotateByAngle(initialRotation);
}

bool OrbitalCamera::update(bool isMouseButtonDown)
{
    if (isMouseButtonDown) {
        Vector2 mouseDelta = mouseDown ? GetMouseDelta() : Vector2 { 0.0f, 0.0f };
        mouseDelta *= SENSITIVITY;

        rotateByAngle(mouseDelta);

        mouseDown = true;
    } else {
        mouseDown = false;
    }

    return mouseDown;
}

void OrbitalCamera::rotateByAngle(Vector2 rotation)
{
    position = Vector3RotateByAxisAngle(position, up, -rotation.x);

    Vector3 right = Vector3CrossProduct(position, up);

    position = Vector3RotateByAxisAngle(position, right, rotation.y);
    up = Vector3RotateByAxisAngle(up, right, rotation.y);
}
