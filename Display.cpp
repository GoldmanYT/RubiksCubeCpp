#include "Display.hpp"
#include "Data.hpp"

void display()
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
    }

    ClearBackground(SKYBLUE);
    BeginDrawing();
    BeginMode3D(camera);

    rubiksCubeModel.draw();

    EndMode3D();

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    for (Button& button : buttons) {
        button.draw(screenWidth, screenHeight);
    }
    for (Label& label : labels) {
        label.draw(screenWidth, screenHeight);
    }

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
        keyboardCallbacks.push_back(KeyboardCallback { key, key, logKeyboardPress });
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

    auto rotate = [](int direction) {
        rubiksCubeModel.rotate(direction);
        updateLabels();
    };

    keyboardCallbacks.push_back(KeyboardCallback { KEY_SPACE, true, rotate });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_LEFT_SHIFT, false, rotate });

    auto nextRowIndex = [](int) { rubiksCubeModel.nextRowIndex(); };
    auto previousRowIndex = [](int) { rubiksCubeModel.previousRowIndex(); };

    keyboardCallbacks.push_back(KeyboardCallback { KEY_UP, 0, nextRowIndex });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_DOWN, 0, previousRowIndex });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_RIGHT, 0, nextRowIndex });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_LEFT, 0, previousRowIndex });

    auto scramble = [](int) {
        rubiksCubeModel.scramble();
        updateLabels();
    };

    keyboardCallbacks.push_back(KeyboardCallback { KEY_BACKSPACE, 0, scramble });

    auto solve = [](int) {
        rubiksCubeModel.solve();
        updateLabels();
    };

    keyboardCallbacks.push_back(KeyboardCallback { KEY_ENTER, 0, solve });

    auto increaseSize = [](int) {
        rubiksCubeModel.increaseSize();
        updateLabels();
    };
    auto decreaseSize = [](int) {
        rubiksCubeModel.decreaseSize();
        updateLabels();
    };

    keyboardCallbacks.push_back(KeyboardCallback { KEY_EQUAL, 0, increaseSize });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_KP_ADD, 0, increaseSize });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_MINUS, 0, decreaseSize });
    keyboardCallbacks.push_back(KeyboardCallback { KEY_KP_SUBTRACT, 0, decreaseSize });

    int i = 0;
    buttons[i] = Button(
        TOP_LEFT,
        DEFAULT_LAYOUT_OFFSET, DEFAULT_LAYOUT_OFFSET,
        "Solve", 0, solve);
    buttons[++i] = Button(
        TOP_LEFT,
        DEFAULT_LAYOUT_OFFSET, 2 * DEFAULT_LAYOUT_OFFSET + DEFAULT_LAYOUT_HEIGHT,
        "Scramble", 0, scramble);

    buttons[++i] = Button(
        TOP_RIGHT,
        DEFAULT_LAYOUT_OFFSET, DEFAULT_LAYOUT_OFFSET,
        "Increase size", 0, increaseSize);
    buttons[++i] = Button(
        TOP_RIGHT,
        DEFAULT_LAYOUT_OFFSET, 2 * DEFAULT_LAYOUT_OFFSET + DEFAULT_LAYOUT_HEIGHT,
        "Decrease size", 0, decreaseSize);

    buttons[++i] = Button(
        BOTTOM_RIGHT,
        DEFAULT_LAYOUT_OFFSET, 2 * DEFAULT_LAYOUT_OFFSET + DEFAULT_LAYOUT_HEIGHT,
        "Clockwise", true, rotate);
    buttons[++i] = Button(
        BOTTOM_RIGHT,
        DEFAULT_LAYOUT_OFFSET, DEFAULT_LAYOUT_OFFSET,
        "Counterclockwise", false, rotate);
    buttons[++i] = Button(
        BOTTOM_RIGHT,
        2 * DEFAULT_LAYOUT_OFFSET + DEFAULT_LAYOUT_WIDTH, 2 * DEFAULT_LAYOUT_OFFSET + DEFAULT_LAYOUT_HEIGHT,
        "+", 0, nextRowIndex);
    buttons[++i] = Button(
        BOTTOM_RIGHT,
        2 * DEFAULT_LAYOUT_OFFSET + DEFAULT_LAYOUT_WIDTH, DEFAULT_LAYOUT_OFFSET,
        "-", 0, previousRowIndex);

    buttons[++i] = Button(
        BOTTOM_LEFT,
        DEFAULT_LAYOUT_OFFSET, 3 * DEFAULT_LAYOUT_OFFSET + 2 * DEFAULT_LAYOUT_HEIGHT,
        "ZOX", ZOX, selectPlane);
    buttons[++i] = Button(
        BOTTOM_LEFT,
        DEFAULT_LAYOUT_OFFSET, 2 * DEFAULT_LAYOUT_OFFSET + DEFAULT_LAYOUT_HEIGHT,
        "XOY", XOY, selectPlane);
    buttons[++i] = Button(
        BOTTOM_LEFT,
        DEFAULT_LAYOUT_OFFSET, DEFAULT_LAYOUT_OFFSET,
        "YOZ", YOZ, selectPlane);

    i = 0;
    labels[i] = Label(
        TOP_LEFT,
        1.0f - 2 * DEFAULT_LAYOUT_OFFSET, DEFAULT_LAYOUT_HEIGHT,
        DEFAULT_LAYOUT_OFFSET, DEFAULT_LAYOUT_OFFSET,
        "Moves: 0");
    labels[++i] = Label(
        TOP_RIGHT,
        1.0f - 2 * DEFAULT_LAYOUT_OFFSET, DEFAULT_LAYOUT_HEIGHT,
        DEFAULT_LAYOUT_OFFSET, 2 * DEFAULT_LAYOUT_OFFSET + DEFAULT_LAYOUT_HEIGHT,
        "Solved");
}

void updateLabels()
{
    int moveCount = rubiksCubeModel.getMoveCount();
    bool solved = rubiksCubeModel.isSolved();

    labels[LABEL_MOVE_COUNT].setText(TextFormat("Moves: %i", moveCount));
    labels[LABEL_SOLVED].setText(solved ? "Solved" : "");
}
