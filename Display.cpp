#include "Display.hpp"
#include "Data.hpp"

void display()
{
    UpdateCamera(&camera, CAMERA_ORBITAL);

    ClearBackground(SKYBLUE);
    BeginDrawing();
    BeginMode3D(camera);

    // DrawGrid(4, 1);

    rubiksCubeModel.draw();

    EndMode3D();
    EndDrawing();
}

void init()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    initData();
    setHotkeys();

    rubiksCubeModel.reset();
}

void mainloop()
{
    simulation();
    display();
}

void setHotkeys()
{
    for (int key = 0; key <= 348; ++key) {
        keyboardCallbacks.push_back(
            KeyboardCallback { key, key, logKeyboardPress });
    }

    auto selectPlane = [](int plane) { rubiksCubeModel.setPlane(CubePlane(plane)); };

    keyboardCallbacks.push_back(KeyboardCallback { KEY_W, ZOX, selectPlane });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_D, XOY, selectPlane });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_A, YOZ, selectPlane });

    auto selectRowIndex = [](int rowIndex) { rubiksCubeModel.setRowIndex(rowIndex); };

    for (int key = 0; key < 9; ++key) {
        keyboardCallbacks.push_back(KeyboardCallback { key + KEY_ONE, key, selectRowIndex });
        keyboardCallbacks.push_back(KeyboardCallback { key + KEY_KP_1, key, selectRowIndex });
    }

    auto rotate = [](int direction) { rubiksCubeModel.rotate(direction); };

    keyboardCallbacks.push_back(KeyboardCallback { KEY_SPACE, true, rotate });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_LEFT_SHIFT, false, rotate });

    auto nextRowIndex = [](int) { rubiksCubeModel.nextRowIndex(); };
    auto previousRowIndex = [](int) { rubiksCubeModel.previousRowIndex(); };

    keyboardCallbacks.push_back(KeyboardCallback { KEY_UP, 0, nextRowIndex });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_DOWN, 0, previousRowIndex });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_RIGHT, 0, nextRowIndex });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_LEFT, 0, previousRowIndex });

    auto scramble = [](int) { rubiksCubeModel.scramble(); };

    keyboardCallbacks.push_back(KeyboardCallback { KEY_BACKSPACE, 0, scramble });

    auto solve = [](int) { rubiksCubeModel.solve(); };

    keyboardCallbacks.push_back(KeyboardCallback { KEY_ENTER, 0, solve });
}
