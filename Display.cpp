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
    SetTargetFPS(FPS);

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
    for (int key = 0; key <= 336; ++key) {
        keyboardCallbacks.push_back(
            KeyboardCallback { key, key, logKeyboardPress });
    }

    auto selectPlane = [](int plane) { rubiksCubeModel.setPlane(CubePlane(plane)); };
    auto selectRowIndex = [](int rowIndex) { rubiksCubeModel.setRowIndex(rowIndex); };
    auto nextRowIndex = [](int) { rubiksCubeModel.nextRowIndex(); };
    auto previousRowIndex = [](int) { rubiksCubeModel.previousRowIndex(); };
    auto rotate = [](int direction) { rubiksCubeModel.rotate(direction); };

    keyboardCallbacks.push_back(KeyboardCallback { KEY_W, XOY, selectPlane });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_D, YOZ, selectPlane });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_A, ZOX, selectPlane });

    for (int key = KEY_ONE; key <= KEY_NINE; ++key) {
        keyboardCallbacks.push_back(KeyboardCallback { key, key - KEY_ONE, selectRowIndex });
    }

    keyboardCallbacks.push_back(KeyboardCallback { KEY_SPACE, true, rotate });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_LEFT_SHIFT, false, rotate });

    keyboardCallbacks.push_back(KeyboardCallback { KEY_UP, 0, nextRowIndex });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_DOWN, 0, previousRowIndex });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_RIGHT, 0, nextRowIndex });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_LEFT, 0, previousRowIndex });
}
