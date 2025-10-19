#include <iostream>

#include "RubiksCube.hpp"

using namespace std;

int main()
{
    RubiksCube cube;

    cube.rotate(XOY, 2, 1);
    cube.rotate(XOY, 2, 1);

    array<vector<vector<StickerColor>>, 6>& stickers = cube.getStickers();
    int size = cube.getSize();

    for (int side = 0; side < SIDE_COUNT; ++side) {
        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                cout << stickers[side][x][y] << " ";
            }
            cout << endl;
        }
        cout << "----------------" << endl;
    }

    return 0;
}