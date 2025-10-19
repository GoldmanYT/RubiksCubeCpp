#pragma once

#include <array>
#include <vector>

#include "Data.hpp"

using namespace std;

// Класс для представления кубика Рубика
class RubiksCube {
public:
    // Конструктор для 3x3x3
    RubiksCube();

    // Конструктор для size x size x size
    RubiksCube(int size);

    // Метод, возвращающий куб в исходное положение
    void reset(int size = 3);

    // Метод, реализующий ращение грани
    //  plane: ось вращения грани
    //  rowIndex: номер "полоски", которая вращается
    //  direction: направление вращения (false = против ЧС, true = по ЧС)
    void rotate(CubePlane plane, int rowIndex, bool direction);

    // Getter для поля size
    int getSize() const;

    // Getter для поля stickers
    array<vector<vector<StickerColor>>, SIDE_COUNT>& getStickers();

private:
    int size;
    array<vector<vector<StickerColor>>, SIDE_COUNT> stickers;
};