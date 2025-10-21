#pragma once

#include "RubiksCube.hpp"

struct GraphicObjectData {
    // Материал стикера
    Material material;

    // Матрицы изменения позиций и поворотов
    vector<Matrix> transforms;
};

// Класс, моделирующий выбранную "полоску"
class SelectedRow {
public:
    // Конструктор
    SelectedRow(int size);

    // Метод для отрисовки выбранной "полоски"
    void draw();

    // Размера кубика Рубика
    int size;

    // Выбранная плоскость вращения
    CubePlane selectedPlane;

    // Выбранный индекс "полоски"
    int selectedRowIndex;

    // Прозрачность выбранной "полоски"
    float alpha;

    // Направление анимации
    //  false = ++
    //  true  = --
    bool animationDirection;

    // Скорость анимации
    const float ANIMATION_SPEED = 200.0f;

    // Минимальная прозрачность
    const float MIN_ALPHA = 0.0f;

    // Максимальная прозрачность
    const float MAX_ALPHA = 128.0f;
};

// Класс для представления модели кубика Рубика
class RubiksCubeModel {
public:
    // Конструктор по умолчанию
    RubiksCubeModel();

    // Конструктор для куба size x size x size
    RubiksCubeModel(int size);

    // Метод для отрисовки модели кубика Рубика
    void draw();

    // Метод для создания новой модели с заданным размером
    void reset(int size = DEFAULT_SIZE);

    // Метод, обновляющий материалы стикеров
    void updateStickers();

    // Метод для выбора плоскости вращения
    void setPlane(CubePlane plane);

    // Метод для выбора "полоски" вращения
    void setRowIndex(int rowIndex);

    // Метод для выбора следующей "полоски" вращения
    void nextRowIndex();

    // Метод для выбора предыдущей "полоски" вращения
    void previousRowIndex();

    // Метод для поворота вдоль выбранной плоскости выбранной "полоски" по направлению direction
    //  direction = false => против ЧС
    //  direction = true  => по ЧС
    void rotate(bool direction);

private:
    // Размер кубика Рубика
    int size;

    // Логическое представление кубика Рубика
    RubiksCube rubiksCube;

    // Физическое представление стикеров кубика в виде Мешей
    array<vector<vector<GraphicObjectData>>, SIDE_COUNT> stickers;

    // Физическое представление кусочков кубика в виде Мешей
    vector<GraphicObjectData> pieces;

    // Выбранная полоска
    SelectedRow selectedRow;

private:
    Matrix getStickerTransform(int side, int x, int y, float angle = 0.0f);
};