#include "Simulation.hpp"

RubiksCubeModel rubiksCubeModel;
vector<KeyboardCallback> keyboardCallbacks;

array<Button, BUTTON_COUNT> buttons;
array<Label, LABEL_COUNT> labels;

OrbitalCamera camera;

void simulation()
{
    for (KeyboardCallback& callback : keyboardCallbacks) {
        if (callback.handler != nullptr && IsKeyPressed(callback.key)) {
            callback.handler(callback.param);
        }
    }

    bool isMouseDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    bool isMousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    Vector2 mousePos = GetMousePosition();
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    bool isPressed = false;

    if (!isMouseDown) {
        selectedElement = SELECTED_NOTHING;
    }

    if (selectedElement == SELECTED_BUTTON || selectedElement == SELECTED_NOTHING) {
        for (int i = 0; i < buttons.size(); ++i) {
            Button& button = buttons[i];
            if (i <= BUTTON_TOGGLE || i > BUTTON_TOGGLE && rotationMode == MODE_BUTTONS) {
                isPressed = button.update(isMousePressed, mousePos, screenWidth, screenHeight);
                if (isPressed) {
                    selectedElement = SELECTED_BUTTON;
                }
            }
        }
    }

    if (selectedElement == SELECTED_STICKER || selectedElement == SELECTED_NOTHING) {
        isPressed = rubiksCubeModel.update(camera, isMouseDown, mousePos);
        if (isPressed) {
            selectedElement = SELECTED_STICKER;
        }
    }

    if (selectedElement == SELECTED_CAMERA || selectedElement == SELECTED_NOTHING) {
        isPressed = camera.update(isMouseDown);
        if (isPressed) {
            selectedElement = SELECTED_CAMERA;
        }
    }
}

void logKeyboardPress(int key)
{
    TraceLog(LOG_INFO, "Pressed key: %i", key);
}
