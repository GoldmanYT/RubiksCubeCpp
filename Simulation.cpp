#include "Simulation.hpp"

RubiksCubeModel rubiksCubeModel;
vector<KeyboardCallback> keyboardCallbacks;

void simulation()
{
    for (KeyboardCallback& callback : keyboardCallbacks) {
        if (callback.handler != nullptr && IsKeyPressed(callback.key)) {
            callback.handler(callback.param);
        }
    }
}

void logKeyboardPress(int key)
{
    cout << "Pressed key: " << key << endl;
}
