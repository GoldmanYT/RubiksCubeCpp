#pragma once

#define PLATFORM_DESKTOP

#include <array>
#include <memory>
#include <random>
#include <vector>

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

using namespace std;

// Начальная ширина окна
extern const int WINDOW_WIDTH;

// Начальная высота окна
extern const int WINDOW_HEIGHT;

// Размер кубика Рубика по умолчанию
extern const int DEFAULT_SIZE;

// Максимальный размер кубика Рубика
extern const int MAX_SIZE;

// Минимальный размер кубика Рубика
extern const int MIN_SIZE;

// Количество сторон куба
const int SIDE_COUNT = 6;

// Количество плоскостей
const int PLANE_COUNT = SIDE_COUNT / 2;

// Перечисление выбранного элемента
enum SelectedElement {
    SELECTED_NOTHING,
    SELECTED_BUTTON,
    SELECTED_CAMERA,
    SELECTED_STICKER
};

// Выбранный элемент
// Необходим для того, чтобы обновлять
// только те элементы, которые выбраны
extern SelectedElement selectedElement;

// Режимы вращения кубика Рубика
enum RotationMode {
    MODE_SWIPES,
    MODE_BUTTONS
};

// Выбранный режим вращения
extern RotationMode rotationMode;

// Скорость вращения камеры
extern const float CAMERA_ROTATION_SPEED;

// Перечисление всех плоскостей куба
enum CubePlane {
    ZOX,
    XOY,
    YOZ
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

// Логика выбора соседних сторон в зависимости от плоскости (XOY, YOZ, ZOX):
// Стороны, которые меняются во время вращения
// в порядке их смены (против часовой стрелки)
extern int ROTATION_SIDES[3][4];

// Выбор грани в зависимости от плоскости (XOY, YOZ, ZOX)
//  ROTATION_SIDE[plane][2]
//  plane - плоскость вращения грани
//  ROTATION_SIDE[plane][0] - грань, если выбран index = 0
//  ROTATION_SIDE[plane][1] - грань, если выбран index = size - 1
extern int ROTATION_SIDE[3][2];

// Цвета, используемые для материалов
extern array<Color, SIDE_COUNT + 3> colors;

// Маленький зазор (между кусочком и стикером)
extern const float TINY_OFFSET;

// Коэффициент размера меша стикера
extern float STICKER_SIZE_K;

// Размер стикера
extern float stickerSize;

// Меш для стикера
extern Mesh stickerMesh;

// Коэффициент размера кусочка
extern float PIECE_SIZE_K;

// Размер кусочка
extern float pieceSize;

// Индекс материала куба из массива материалов
extern const int CUBE_MATERIAL;

// Меш выбранного кусочка
extern Mesh pieceMesh;

// Материалы стикеров и самого кубика
extern vector<Material> materials;

// Индекс материала выбранной "полоски"
extern const int SELECTED_ROW_MATERIAL;

// Меш выбранной "полоски"
extern Mesh selectedRowMesh;

// Количество зазоров между кусочками и выбранной "полоской"
extern int COUNT_MARGINS;

// Коэффициент количества вращений при запутывании
extern const int ROTATION_COUNT_K;

// Ширина кнопки по умолчанию
extern const float DEFAULT_LAYOUT_WIDTH;

// Высота кнопки по умолчанию
extern const float DEFAULT_LAYOUT_HEIGHT;

// Отступ кнопки от краёв экрана по умолчанию
extern const float DEFAULT_LAYOUT_OFFSET;

// Индекс цвета кнопки из массива цветов
extern const int BUTTON_COLOR;

// Размер шрифта
extern const float FONT_SIZE;

// Индекс цвета шрифта из массива цветов
extern const int FONT_COLOR;

// Используемый шрифт
extern Font font;

// Шрифт для символов
extern Font symbolFont;

// Количество кнопок
const int BUTTON_COUNT = 12;

// Количество надписей
const int LABEL_COUNT = 2;

// Индекс надписи с количеством ходов
extern const int LABEL_MOVE_COUNT;

// Индекс надписи с состоянием кубика Рубика (собран/нет)
extern const int LABEL_SOLVED;

// Индекс кнопки с "Глазом"
extern int BUTTON_TOGGLE;

// Текст в кодировке UTF-8 "Собран"
extern char TEXT_SOLVED[];

// Текст в кодировке UTF-8 "Ходов: %i"
extern char TEXT_MOVES[];

// Текст в кодировке UTF-8 "Горизонтально"
extern char TEXT_HORIZONTAL[];

// Текст в кодировке UTF-8 "Вертикально 1"
extern char TEXT_VERTICAL_1[];

// Текст в кодировке UTF-8 "Вертикально 2"
extern char TEXT_VERTICAL_2[];

// Текст в кодировке UTF-8 для шрифта символов "Глаз"
extern char TEXT_SHOW[];

// Текст в кодировке UTF-8 для шрифта символов "Зачёркнутый глаз"
extern char TEXT_HIDE[];

// ------------ Функции ---------------

// Функция, инициализирующая все данные
void initData();
