#include "Display.hpp"

using namespace std;

int main()
{
    init();

    while (!WindowShouldClose()) {
        mainloop();
    }

    CloseWindow();
    return 0;
}