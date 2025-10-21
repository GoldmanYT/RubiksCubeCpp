#pragma once

#include <array>
#include <iostream>
#include <memory>
#include <vector>

#include <raylib.h>
#include <raymath.h>

using namespace std;

// ������ ����
extern const int WINDOW_WIDTH;

// ������ ����
extern const int WINDOW_HEIGHT;

// ������ ������ ������ �� ���������
extern const int DEFAULT_SIZE;

// ������������ ������
extern Camera camera;

// ���������� ������ ����
const int SIDE_COUNT = 6;

// ������������ ���� ���� ����
enum CubePlane {
    XOY,
    YOZ,
    ZOX
};

// ������������ ���� ������ ������ ������
enum StickerColor {
    STICKER_WHITE,
    STICKER_GREEN,
    STICKER_RED,
    STICKER_YELLOW,
    STICKER_BLUE,
    STICKER_ORANGE
};

// ����������, ����������� ��� �������� �����
struct RotationData {
    // ��������� ������ (������� �������� �� ����� �������):
    //  false - �� [i0][i1] ������� i0
    //  true - �� [i0][i1] ������� i1
    bool initialIndex;

    // �������, ������� �������� �� ����� ��������
    // � ������� �� ����� (������ ������� �������)
    int sides[4];
};

// ������ ������ �������� ������ � ����������� �� ��������� (XOY, YOZ, ZOX):
//  ROTATION_SIDES[plane]
//  plane - ��������� �������� �����
extern RotationData ROTATION_SIDES[3];

// ����� ����� � ����������� �� ��������� (XOY, YOZ, ZOX)
//  ROTATION_SIDE[plane][2]
//  plane - ��������� �������� �����
//  ROTATION_SIDE[plane][0] - �����, ���� ������ index = 0
//  ROTATION_SIDE[plane][1] - �����, ���� ������ index = size - 1
extern int ROTATION_SIDE[3][2];

// �����, ������������ ��� ����������
extern array<Color, SIDE_COUNT + 2> colors;

// ��������� ����� (����� �������� � ��������)
extern const float TINY_OFFSET;

// ������ ���� �������
extern float STICKER_SIZE;

// ��� ��� �������
extern Mesh stickerMesh;

extern float PIECE_SIZE;

// ������ ��������� ���� �� ������� ����������
extern const int CUBE_MATERIAL;

extern Mesh pieceMesh;

// ��������� �������� � ������ ������
extern vector<Material> materials;

// ������ ��������� ��������� "�������"
extern const int SELECTED_ROW_MATERIAL;

// ��� ��������� "�������"
extern Mesh selectedRowMesh;

// ���������� ������� ����� ��������� � ��������� "��������"
extern int COUNT_MARGINS;

// ------------ ������� ---------------

// �������, ���������������� ��� ������
void initData();
