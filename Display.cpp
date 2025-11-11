#include "Display.hpp"
#include "Data.hpp"

void display()
{
    ClearBackground(SKYBLUE);
    BeginDrawing();
    BeginMode3D((Camera)camera);

    rubiksCubeModel.draw();

    EndMode3D();

    updateLabels();

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    for (int i = 0; i < buttons.size(); ++i) {
        Button& button = buttons[i];
        if (i <= BUTTON_TOGGLE || (i > BUTTON_TOGGLE && rotationMode == MODE_BUTTONS)) {
            button.draw(screenWidth, screenHeight);
        }
    }
    for (Label& label : labels) {
        label.draw(screenWidth, screenHeight);
    }

    EndDrawing();
}

void init()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
#ifdef PLATFORM_ANDROID
    InitWindow(0, 0, "Rubik's Cube");
#else
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rubik's Cube");
#endif
#ifdef PLATFORM_DESKTOP
    SetWindowState(FLAG_WINDOW_RESIZABLE);
#endif
#ifndef PLATFORM_DESKTOP
    rlSetClipPlanes(4.5, 7.0);
#endif
    Image icon = LoadImage("assets/images/icon.png");
    SetWindowIcon(icon);

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
    auto selectPlane = [](int plane) { rubiksCubeModel.setPlane(CubePlane(plane)); };

    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_PRESS, KEY_W, ZOX, selectPlane });
    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_PRESS, KEY_D, XOY, selectPlane });
    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_PRESS, KEY_A, YOZ, selectPlane });

    auto selectRowIndex = [](int rowIndex) { rubiksCubeModel.setRowIndex(rowIndex); };

    for (int key = 0; key < 9; ++key) {
        keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_PRESS, key + KEY_ONE, key, selectRowIndex });
        keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_PRESS, key + KEY_KP_1, key, selectRowIndex });
    }

    auto rotate = [](int direction) { rubiksCubeModel.rotate(direction, true); };

    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_PRESS, KEY_SPACE, true, rotate });
    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_PRESS, KEY_LEFT_SHIFT, false, rotate });

    auto nextRowIndex = [](int) { rubiksCubeModel.nextRowIndex(); };
    auto previousRowIndex = [](int) { rubiksCubeModel.previousRowIndex(); };

    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_PRESS, KEY_UP, 0, nextRowIndex });
    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_PRESS, KEY_DOWN, 0, previousRowIndex });
    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_PRESS, KEY_RIGHT, 0, nextRowIndex });
    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_PRESS, KEY_LEFT, 0, previousRowIndex });

    auto scramble = [](int) { rubiksCubeModel.scramble(); };

    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_PRESS, KEY_BACKSPACE, 0, scramble });

    auto solve = [](int) { rubiksCubeModel.solve(); };

    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_PRESS, KEY_ENTER, 0, solve });

    auto increaseSize = [](int) { rubiksCubeModel.increaseSize(); };
    auto decreaseSize = [](int) { rubiksCubeModel.decreaseSize(); };

    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_PRESS, KEY_EQUAL, 0, increaseSize });
    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_PRESS, KEY_KP_ADD, 0, increaseSize });
    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_PRESS, KEY_MINUS, 0, decreaseSize });
    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_PRESS, KEY_KP_SUBTRACT, 0, decreaseSize });

    enum CameraRotationDirection {
        LEFT,
        RIGHT,
        DOWN,
        UP
    };

    auto cameraRotate = [](int rotationDirection) {
        float frameTime = GetFrameTime();
        float amount = frameTime * CAMERA_ROTATION_SPEED;
        Vector2 direction = Vector2Zeros;
        switch (rotationDirection) {
        case LEFT:
            direction = { -1.0f, 0.0f };
            break;
        case RIGHT:
            direction = { 1.0f, 0.0f };
            break;
        case DOWN:
            direction = { 0.0f, 1.0f };
            break;
        case UP:
            direction = { 0.0f, -1.0f };
            break;
        }
        camera.rotateByAngle(direction * amount);
    };

    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_DOWN, KEY_J, LEFT, cameraRotate });
    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_DOWN, KEY_L, RIGHT, cameraRotate });
    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_DOWN, KEY_K, DOWN, cameraRotate });
    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_DOWN, KEY_I, UP, cameraRotate });

    auto toggleButtons = [](int) {
        rotationMode = (rotationMode == MODE_SWIPES) ? MODE_BUTTONS : MODE_SWIPES;
        if (rotationMode == MODE_SWIPES) {
            buttons[BUTTON_TOGGLE].setOffset(
                DEFAULT_LAYOUT_OFFSET, DEFAULT_LAYOUT_OFFSET);
            buttons[BUTTON_TOGGLE].setText(TEXT_HIDE);
        } else {
            buttons[BUTTON_TOGGLE].setOffset(
                2 * DEFAULT_LAYOUT_OFFSET + DEFAULT_LAYOUT_WIDTH, DEFAULT_LAYOUT_OFFSET);
            buttons[BUTTON_TOGGLE].setText(TEXT_SHOW);
        }
    };

    keyboardCallbacks.push_back(KeyboardCallback { CALLBACK_TYPE_PRESS, KEY_TAB, 0, toggleButtons });

    int i = 0;
    buttons[i] = Button(
        TOP_LEFT,
        DEFAULT_LAYOUT_OFFSET, DEFAULT_LAYOUT_OFFSET,
        "\xEE\xA2\xA9", 0, solve, true);
    buttons[++i] = Button(
        TOP_LEFT,
        DEFAULT_LAYOUT_OFFSET, 2 * DEFAULT_LAYOUT_OFFSET + DEFAULT_LAYOUT_HEIGHT,
        "\xEE\xB2\xA5", 0, scramble, true);

    buttons[++i] = Button(
        TOP_RIGHT,
        DEFAULT_LAYOUT_OFFSET, DEFAULT_LAYOUT_OFFSET,
        "\xEE\x87\x99", 0, increaseSize, true);
    buttons[++i] = Button(
        TOP_RIGHT,
        DEFAULT_LAYOUT_OFFSET, 2 * DEFAULT_LAYOUT_OFFSET + DEFAULT_LAYOUT_HEIGHT,
        "\xEE\x87\x98", 0, decreaseSize, true);

    buttons[++i] = Button(
        BOTTOM_LEFT,
        DEFAULT_LAYOUT_HEIGHT, DEFAULT_LAYOUT_HEIGHT,
        DEFAULT_LAYOUT_OFFSET, DEFAULT_LAYOUT_OFFSET,
        TEXT_HIDE, 0, toggleButtons, true);

    BUTTON_TOGGLE = i;

    buttons[++i] = Button(
        BOTTOM_RIGHT,
        DEFAULT_LAYOUT_OFFSET, 2 * DEFAULT_LAYOUT_OFFSET + DEFAULT_LAYOUT_HEIGHT,
        "\xEE\x9C\xAC", true, rotate, true);
    buttons[++i] = Button(
        BOTTOM_RIGHT,
        DEFAULT_LAYOUT_OFFSET, DEFAULT_LAYOUT_OFFSET,
        "\xEE\x9D\xB7", false, rotate, true);
    buttons[++i] = Button(
        BOTTOM_RIGHT,
        2 * DEFAULT_LAYOUT_OFFSET + DEFAULT_LAYOUT_WIDTH, 2 * DEFAULT_LAYOUT_OFFSET + DEFAULT_LAYOUT_HEIGHT,
        "\xEE\x80\x90", 0, nextRowIndex, true);
    buttons[++i] = Button(
        BOTTOM_RIGHT,
        2 * DEFAULT_LAYOUT_OFFSET + DEFAULT_LAYOUT_WIDTH, DEFAULT_LAYOUT_OFFSET,
        "\xEE\x80\x91", 0, previousRowIndex, true);

    buttons[++i] = Button(
        BOTTOM_LEFT,
        DEFAULT_LAYOUT_OFFSET, 3 * DEFAULT_LAYOUT_OFFSET + 2 * DEFAULT_LAYOUT_HEIGHT,
        TEXT_HORIZONTAL, ZOX, selectPlane);
    buttons[++i] = Button(
        BOTTOM_LEFT,
        DEFAULT_LAYOUT_OFFSET, 2 * DEFAULT_LAYOUT_OFFSET + DEFAULT_LAYOUT_HEIGHT,
        TEXT_VERTICAL_1, XOY, selectPlane);
    buttons[++i] = Button(
        BOTTOM_LEFT,
        DEFAULT_LAYOUT_OFFSET, DEFAULT_LAYOUT_OFFSET,
        TEXT_VERTICAL_2, YOZ, selectPlane);

    i = 0;
    labels[i] = Label(
        TOP_LEFT,
        1.0f - 2 * DEFAULT_LAYOUT_OFFSET, DEFAULT_LAYOUT_HEIGHT,
        DEFAULT_LAYOUT_OFFSET, DEFAULT_LAYOUT_OFFSET,
        TextFormat(TEXT_MOVES, 0));
    labels[++i] = Label(
        TOP_RIGHT,
        1.0f - 2 * DEFAULT_LAYOUT_OFFSET, DEFAULT_LAYOUT_HEIGHT,
        DEFAULT_LAYOUT_OFFSET, 2 * DEFAULT_LAYOUT_OFFSET + DEFAULT_LAYOUT_HEIGHT,
        TEXT_SOLVED);
}

void updateLabels()
{
    int moveCount = rubiksCubeModel.getMoveCount();
    bool solved = rubiksCubeModel.isSolved();

    labels[LABEL_MOVE_COUNT].setText(TextFormat(TEXT_MOVES, moveCount));
    labels[LABEL_SOLVED].setText(solved ? TEXT_SOLVED : "");
}
