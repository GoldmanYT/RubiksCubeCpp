#pragma once

#include "Data.hpp"

// Класс для представления кубика Рубика
class RubiksCube {
public:
    // Конструктор для 3x3x3
    RubiksCube();

    // Конструктор для size x size x size
    RubiksCube(int size);

    // Метод, возвращающий куб в исходное положение
    void reset(int size = DEFAULT_SIZE);

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
    // Размер кубика Рубика
    int size;

    // Логическое представление стикеров кубика
    array<vector<vector<StickerColor>>, SIDE_COUNT> stickers;
};