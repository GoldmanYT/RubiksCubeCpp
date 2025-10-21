#pragma once

#include "RubiksCube.hpp"
#include "RubiksCubeModel.hpp"

extern RubiksCubeModel rubiksCubeModel;

struct KeyboardCallback {
    int key;
    int param;
    void (*handler)(int);
};

extern vector<KeyboardCallback> keyboardCallbacks;

void simulation();
void logKeyboardPress(int key);
