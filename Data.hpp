#pragma once

// ������ ������ ������ �� ���������
const int DEFAULT_SIZE = 3;

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
    WHITE,
    GREEN,
    RED,
    YELLOW,
    BLUE,
    ORANGE
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