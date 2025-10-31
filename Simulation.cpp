#include "Simulation.hpp"

RubiksCubeModel rubiksCubeModel;
vector<KeyboardCallback> keyboardCallbacks;
array<Button, BUTTON_COUNT> buttons;

void simulation()
{
    for (KeyboardCallback& callback : keyboardCallbacks) {
        if (callback.handler != nullptr && IsKeyPressed(callback.key)) {
            callback.handler(callback.param);
        }
    }

    rubiksCubeModel.update();

    bool isMousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    Vector2 mousePos = GetMousePosition();
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    for (Button& button : buttons) {
        button.update(isMousePressed, mousePos, screenWidth, screenHeight);
    }
}

void logKeyboardPress(int key)
{
    cout << "Pressed key: " << key << endl;
}
