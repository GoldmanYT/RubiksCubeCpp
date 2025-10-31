#include "RubiksCube.hpp"

RubiksCube::RubiksCube()
{
    reset(DEFAULT_SIZE);
}

RubiksCube::RubiksCube(int size)
{
    reset(size);
}

void RubiksCube::reset(int size)
{
    this->size = size;
    for (int side = 0; side < SIDE_COUNT; ++side) {
        stickers[side].resize(size);
        for (int x = 0; x < size; ++x) {
            stickers[side][x].resize(size);
            for (int y = 0; y < size; ++y) {
                stickers[side][x][y] = StickerColor(side);
            }
        }
    }
}

void RubiksCube::rotate(CubePlane plane, int rowIndex, bool direction)
{
    StickerColor temp;
    int side = -1;
    if (rowIndex == 0) {
        // Самая ближняя сторона к плоскости
        side = ROTATION_SIDE[plane][0];
    } else if (rowIndex == size - 1) {
        // Самая дальняя сторона от плоскости
        side = ROTATION_SIDE[plane][1];
    }

    // если side = -1, то вращение не требуется
    if (side != -1) {
        // Вращение самой стороны
        for (int x = 0; x < (size + 1) / 2; ++x) {
            for (int y = 0; y < size / 2; ++y) {
                pair<int, int> indexes[4] = {
                    { x, y },
                    { y, size - x - 1 },
                    { size - x - 1, size - y - 1 },
                    { size - y - 1, x }
                };
                temp = stickers[side][x][y];

                // по часовой стрелке
                if (direction != ROTATION_DIRECTION[side]) {
                    swap(indexes[1], indexes[3]);
                }

                for (int i = 1; i < 4; ++i) {
                    stickers[side][indexes[i - 1].first][indexes[i - 1].second] = stickers[side][indexes[i].first][indexes[i].second];
                }

                pair<int, int>& last = indexes[3];
                stickers[side][last.first][last.second] = temp;
            }
        }
    }

    for (int columnIndex = 0; columnIndex < size; ++columnIndex) {
        RotationData rotationData = ROTATION_SIDES[plane];
        pair<int, int> indexes[4];
        bool indexOrder = rotationData.initialIndex;
        int* sides = rotationData.sides;

        for (int sideIndex = 0; sideIndex < 4; ++sideIndex, indexOrder = !indexOrder) {
            if (sideIndex < 2) {
                if (indexOrder) {
                    indexes[sideIndex] = { rowIndex, columnIndex };
                } else {
                    indexes[sideIndex] = { columnIndex, rowIndex };
                }
            } else {
                if (indexOrder) {
                    indexes[sideIndex] = { rowIndex, size - columnIndex - 1 };
                } else {
                    indexes[sideIndex] = { size - columnIndex - 1, rowIndex };
                }
            }
        }

        // по часовой стрелке
        if (direction) {
            swap(indexes[1], indexes[3]);
            swap(sides[1], sides[3]);
        }

        temp = stickers[sides[0]][indexes[0].first][indexes[0].second];
        for (int sideIndex = 1; sideIndex < 4; ++sideIndex) {
            stickers[sides[sideIndex - 1]][indexes[sideIndex - 1].first][indexes[sideIndex - 1].second] = stickers[sides[sideIndex]][indexes[sideIndex].first][indexes[sideIndex].second];
        }

        pair<int, int>& last = indexes[3];
        stickers[sides[3]][last.first][last.second] = temp;
    }
}

int RubiksCube::getSize() const
{
    return size;
}

array<vector<vector<StickerColor>>, SIDE_COUNT>& RubiksCube::getStickers()
{
    return stickers;
}

void RubiksCube::solve()
{
    reset(size);
}

bool RubiksCube::isSolved()
{
    for (int side = 0; side < SIDE_COUNT; ++side) {
        StickerColor sticker = stickers[side][0][0];
        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                if (stickers[side][x][y] != sticker) {
                    return false;
                }
            }
        }
    }
    return true;
}
