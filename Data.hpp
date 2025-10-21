#pragma once

#include <array>
#include <iostream>
#include <memory>
#include <vector>

#include <raylib.h>
#include <raymath.h>

using namespace std;

// Ширина окна
extern const int WINDOW_WIDTH;

// Высота окна
extern const int WINDOW_HEIGHT;

// Размер кубика Рубика по умолчанию
extern const int DEFAULT_SIZE;

// Используемая камера
extern Camera camera;

// Количество сторон куба
const int SIDE_COUNT = 6;

// Перечисление всех осей куба
enum CubePlane {
    XOY,
    YOZ,
    ZOX
};

// Перечисление всех цветок кубика Рубика
enum StickerColor {
    STICKER_WHITE,
    STICKER_GREEN,
    STICKER_RED,
    STICKER_YELLOW,
    STICKER_BLUE,
    STICKER_ORANGE
};

// Информация, необходимая для вращения грани
struct RotationData {
    // Начальный индекс (который меняется на одной стороне):
    //  false - из [i0][i1] выбрать i0
    //  true - из [i0][i1] выбрать i1
    bool initialIndex;

    // Стороны, которые меняются во время вращения
    // в порядке их смены (против часовой стрелки)
    int sides[4];
};

// Логика выбора соседних сторон в зависимости от плоскости (XOY, YOZ, ZOX):
//  ROTATION_SIDES[plane]
//  plane - плоскость вращения грани
extern RotationData ROTATION_SIDES[3];

// Выбор грани в зависимости от плоскости (XOY, YOZ, ZOX)
//  ROTATION_SIDE[plane][2]
//  plane - плоскость вращения грани
//  ROTATION_SIDE[plane][0] - грань, если выбран index = 0
//  ROTATION_SIDE[plane][1] - грань, если выбран index = size - 1
extern int ROTATION_SIDE[3][2];

// Цвета, используемые для материалов
extern array<Color, SIDE_COUNT + 2> colors;

// Маленький зазор (между кусочком и стикером)
extern const float TINY_OFFSET;

// Размер меша стикера
extern float STICKER_SIZE;

// Меш для стикера
extern Mesh stickerMesh;

extern float PIECE_SIZE;

// Индекс материала куба из массива материалов
extern const int CUBE_MATERIAL;

extern Mesh pieceMesh;

// Материалы стикеров и самого кубика
extern vector<Material> materials;

// Индекс материала выбранной "полоски"
extern const int SELECTED_ROW_MATERIAL;

// Меш выбранной "полоски"
extern Mesh selectedRowMesh;

// Количество зазоров между кусочками и выбранной "полоской"
extern int COUNT_MARGINS;

// ------------ Функции ---------------

// Функция, инициализирующая все данные
void initData();
