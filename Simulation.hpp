#pragma once

#include "Button.hpp"
#include "OrbitalCamera.hpp"
#include "RubiksCube.hpp"
#include "RubiksCubeModel.hpp"

extern RubiksCubeModel rubiksCubeModel;

extern array<Button, BUTTON_COUNT> buttons;
extern array<Label, LABEL_COUNT> labels;

extern OrbitalCamera camera;

enum KeyboardCallbackType {
    CALLBACK_TYPE_PRESS,
    CALLBACK_TYPE_DOWN
};

struct KeyboardCallback {
    KeyboardCallbackType type;
    int key;
    int param;
    void (*handler)(int);
};

extern vector<KeyboardCallback> keyboardCallbacks;

void simulation();
