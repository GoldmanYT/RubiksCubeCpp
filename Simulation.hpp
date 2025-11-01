#pragma once

#include "Button.hpp"
#include "OrbitalCamera.hpp"
#include "RubiksCube.hpp"
#include "RubiksCubeModel.hpp"

extern RubiksCubeModel rubiksCubeModel;

extern array<Button, BUTTON_COUNT> buttons;
extern array<Label, LABEL_COUNT> labels;

extern OrbitalCamera camera;

struct KeyboardCallback {
    int key;
    int param;
    void (*handler)(int);
};

extern vector<KeyboardCallback> keyboardCallbacks;

void simulation();
void logKeyboardPress(int key);
