#pragma once

// –азмер кубика –убика по умолчанию
const int DEFAULT_SIZE = 3;

//  оличество сторон куба
const int SIDE_COUNT = 6;

// ѕеречисление всех осей куба
enum CubePlane {
    XOY,
    YOZ,
    ZOX
};

// ѕеречисление всех цветок кубика –убика
enum StickerColor {
    WHITE,
    GREEN,
    RED,
    YELLOW,
    BLUE,
    ORANGE
};

// »нформаци€, необходима€ дл€ вращени€ грани
struct RotationData {
    // Ќачальный индекс (который мен€етс€ на одной стороне):
    //  false - из [i0][i1] выбрать i0
    //  true - из [i0][i1] выбрать i1
    bool initialIndex;

    // —тороны, которые мен€ютс€ во врем€ вращени€
    // в пор€дке их смены (против часовой стрелки)
    int sides[4];
};

// Ћогика выбора соседних сторон в зависимости от плоскости (XOY, YOZ, ZOX):
//  ROTATION_SIDES[plane]
//  plane - плоскость вращени€ грани
extern RotationData ROTATION_SIDES[3];

// ¬ыбор грани в зависимости от плоскости (XOY, YOZ, ZOX)
//  ROTATION_SIDE[plane][2]
//  plane - плоскость вращени€ грани
//  ROTATION_SIDE[plane][0] - грань, если выбран index = 0
//  ROTATION_SIDE[plane][1] - грань, если выбран index = size - 1
extern int ROTATION_SIDE[3][2];