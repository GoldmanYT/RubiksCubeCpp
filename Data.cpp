﻿#include "Data.hpp"

#ifndef DESKTOP
// Начальная ширина окна
const int WINDOW_WIDTH = 1080;
#else
const int WINDOW_WIDTH = 1024;
#endif

#ifndef DESKTOP
// Начальная высота окна
const int WINDOW_HEIGHT = 1920;
#else
const int WINDOW_HEIGHT = 768;
#endif

// Размер кубика Рубика по умолчанию
const int DEFAULT_SIZE = 3;

// Максимальный размер кубика Рубика
const int MAX_SIZE = 20;

// Минимальный размер кубика Рубика
const int MIN_SIZE = 2;

// Логика выбора направления вращения в зависимости от стороны
//  false - поведение совпадает с принятым по умолчанию
//  true  - поведение противоположно принятому по умолчанию
bool ROTATION_DIRECTION[SIDE_COUNT] = { false, true, false, false, true, false };

// Логика выбора соседних сторон в зависимости от плоскости (XOY, YOZ, ZOX):
//  ROTATION_SIDES[plane]
//  plane - плоскость вращения грани
RotationData ROTATION_SIDES[3] = {
    { false, { STICKER_GREEN, STICKER_ORANGE, STICKER_BLUE, STICKER_RED } },
    { true, { STICKER_WHITE, STICKER_ORANGE, STICKER_YELLOW, STICKER_RED } },
    { false, { STICKER_WHITE, STICKER_BLUE, STICKER_YELLOW, STICKER_GREEN } }
};

// Выбор грани в зависимости от плоскости (XOY, YOZ, ZOX)
//  ROTATION_SIDE[plane][2]
//  plane - плоскость вращения грани
//  ROTATION_SIDE[plane][0] - грань, если выбран index = 0
//  ROTATION_SIDE[plane][1] - грань, если выбран index = size - 1
int ROTATION_SIDE[3][2] = {
    { STICKER_YELLOW, STICKER_WHITE },
    { STICKER_BLUE, STICKER_GREEN },
    { STICKER_ORANGE, STICKER_RED }
};

// Цвета, используемые для материалов
array<Color, SIDE_COUNT + 3> colors = { WHITE, GREEN, RED, YELLOW, BLUE, ORANGE, BLACK, LIGHTGRAY, ColorAlpha(LIGHTGRAY, 0.5f) };

// Маленький зазор (между кусочком и стикером)
const float TINY_OFFSET = 0.002f;

// Коэффициент размера меша стикера
float STICKER_SIZE_K = 0.95f;

// Размер кусочка
float stickerSize = STICKER_SIZE_K / DEFAULT_SIZE;

// Меш для стикера
Mesh stickerMesh;

// Коэффициент размера кусочка
float PIECE_SIZE_K = 1.0f;

// Размер кусочка
float pieceSize = PIECE_SIZE_K / DEFAULT_SIZE;

// Индекс материала куба из массива материалов
const int CUBE_MATERIAL = SIDE_COUNT;

// Меш выбранного кусочка
Mesh pieceMesh;

// Материалы стикеров и самого кубика
vector<Material> materials;

// Индекс материала выбранной "полоски"
const int SELECTED_ROW_MATERIAL = SIDE_COUNT + 1;

// Меш выбранной "полоски"
Mesh selectedRowMesh;

// Количество зазоров между кусочками и выбранной "полоской"
int COUNT_MARGINS = 3;

// Коэффициент количества вращений при запутывании
const int ROTATION_COUNT_K = 10;

// Ширина элемента по умолчанию
const float DEFAULT_LAYOUT_WIDTH = 1.0f / 4.0f;

// Высота кнопки по умолчанию
const float DEFAULT_LAYOUT_HEIGHT = 1.0f / 10.0f;

// Отступ кнопки от краёв экрана
const float DEFAULT_LAYOUT_OFFSET = 0.02f;

// Индекс цвета кнопки из массива цветов
const int BUTTON_COLOR = SIDE_COUNT + 2;

// Размер шрифта
const float FONT_SIZE = 48.0f;

// Индекс цвета шрифта из массива цветов
const int FONT_COLOR = SIDE_COUNT;

// Используемый шрифт
Font font;

// Шрифт для символов
Font symbolFont;

// Индекс надписи с количеством ходов
const int LABEL_MOVE_COUNT = 0;

// Индекс надписи с состоянием кубика Рубика (собран/нет)
const int LABEL_SOLVED = 1;

char TEXT_SOLVED[] = "\xD0\xA1\xD0\xBE\xD0\xB1\xD1\x80\xD0\xB0\xD0\xBD";

char TEXT_MOVES[] = "\xD0\xA5\xD0\xBE\xD0\xB4\xD0\xBE\xD0\xB2\x3A\x20%i";

char TEXT_HORIZONTAL[] = "\xD0\x93\xD0\xBE\xD1\x80\xD0\xB8\xD0\xB7\xD0\xBE\xD0\xBD\xD1\x82\xD0\xB0\xD0\xBB\xD1\x8C\xD0\xBD\xD0\xBE";

char TEXT_VERTICAL_1[] = "\xD0\x92\xD0\xB5\xD1\x80\xD1\x82\xD0\xB8\xD0\xBA\xD0\xB0\xD0\xBB\xD1\x8C\xD0\xBD\xD0\xBE\x20\x31";

char TEXT_VERTICAL_2[] = "\xD0\x92\xD0\xB5\xD1\x80\xD1\x82\xD0\xB8\xD0\xBA\xD0\xB0\xD0\xBB\xD1\x8C\xD0\xBD\xD0\xBE\x20\x32";

void initData()
{
    srand((unsigned int)time(0));

    for (int colorIndex = 0; colorIndex < colors.size(); ++colorIndex) {
        materials.push_back(LoadMaterialDefault());
        MaterialMap& map = materials[colorIndex].maps[MATERIAL_MAP_DIFFUSE];
        map.color = colors[colorIndex];
    }

    stickerMesh = GenMeshPlane(STICKER_SIZE_K, STICKER_SIZE_K, 1, 1);
    pieceMesh = GenMeshCube(PIECE_SIZE_K, PIECE_SIZE_K, PIECE_SIZE_K);
    selectedRowMesh = GenMeshCube(1.0f, 1.0f, 1.0f);

    vector<int> codepoints(0x0FCC + 1);
    for (int i = 0; i <= 0x0FCC; ++i) {
        codepoints[i] = i;
    }
    font = LoadFontEx("assets/font/l_10646.ttf", FONT_SIZE, codepoints.data(), codepoints.size());

    vector<int> symbolCodepoints(0xF8CC - 0xE001 + 1);
    for (int i = 0xE001; i <= 0xF8CC; i++) {
        symbolCodepoints[i - 0xE001] = i;
    }
    symbolFont = LoadFontEx("assets/font/SegoeIcons.ttf", FONT_SIZE, symbolCodepoints.data(), symbolCodepoints.size());
}
