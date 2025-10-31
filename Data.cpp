#include "Data.hpp"

// Начальная ширина окна
const int WINDOW_WIDTH = 800;

// Начальная высота окна
const int WINDOW_HEIGHT = 600;

// Используемая камера
Camera camera;

// Размер кубика Рубика по умолчанию
const int DEFAULT_SIZE = 3;

// Максимальный размер кубика Рубика
const int MAX_SIZE = 25;

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
const float TINY_OFFSET = 0.001f;

// Размер меша стикера
float STICKER_SIZE = 0.9f;

// Меш для стикера
Mesh stickerMesh;

// Размер кусочка
float PIECE_SIZE = 1.0f;

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
const float FONT_SIZE = 36.0f;

// Индекс цвета шрифта из массива цветов
const int FONT_COLOR = SIDE_COUNT;

// Используемый шрифт
Font font;

// Индекс надписи с количеством ходов
const int LABEL_MOVE_COUNT = 0;

// Индекс надписи с состоянием кубика Рубика (собран/нет)
const int LABEL_SOLVED = 1;

void initData()
{
    srand((unsigned int)time(0));

    for (int colorIndex = 0; colorIndex < colors.size(); ++colorIndex) {
        materials.push_back(LoadMaterialDefault());
        MaterialMap& map = materials[colorIndex].maps[MATERIAL_MAP_DIFFUSE];
        map.color = colors[colorIndex];
    }

    stickerMesh = GenMeshPlane(STICKER_SIZE, STICKER_SIZE, 1, 1);
    pieceMesh = GenMeshCube(PIECE_SIZE, PIECE_SIZE, PIECE_SIZE);
    selectedRowMesh = GenMeshCube(1.0f, 1.0f, 1.0f);

    camera = Camera {
        { DEFAULT_SIZE * 3.0f, DEFAULT_SIZE * 3.0f, DEFAULT_SIZE * 3.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        DEFAULT_SIZE * 3.0f,
        CAMERA_ORTHOGRAPHIC
    };

    font = GetFontDefault();
}
