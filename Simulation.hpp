#pragma once

#include "Button.hpp"
#include "RubiksCube.hpp"
#include "RubiksCubeModel.hpp"

extern RubiksCubeModel rubiksCubeModel;

extern array<Button, BUTTON_COUNT> buttons;

struct KeyboardCallback {
    int key;
    int param;
    void (*handler)(int);
};

extern vector<KeyboardCallback> keyboardCallbacks;

void simulation();
void logKeyboardPress(int key);
