#pragma once

#include "RubiksCube.hpp"

struct GraphicObjectData {
    // �������� �������
    Material material;

    // ������� ��������� ������� � ���������
    vector<Matrix> transforms;
};

// �����, ������������ ��������� "�������"
class SelectedRow {
public:
    // �����������
    SelectedRow(int size);

    // ����� ��� ��������� ��������� "�������"
    void draw();

    // ������� ������ ������
    int size;

    // ��������� ��������� ��������
    CubePlane selectedPlane;

    // ��������� ������ "�������"
    int selectedRowIndex;

    // ������������ ��������� "�������"
    float alpha;

    // ����������� ��������
    //  false = ++
    //  true  = --
    bool animationDirection;

    // �������� ��������
    const float ANIMATION_SPEED = 200.0f;

    // ����������� ������������
    const float MIN_ALPHA = 0.0f;

    // ������������ ������������
    const float MAX_ALPHA = 128.0f;
};

// ����� ��� ������������� ������ ������ ������
class RubiksCubeModel {
public:
    // ����������� �� ���������
    RubiksCubeModel();

    // ����������� ��� ���� size x size x size
    RubiksCubeModel(int size);

    // ����� ��� ��������� ������ ������ ������
    void draw();

    // ����� ��� �������� ����� ������ � �������� ��������
    void reset(int size = DEFAULT_SIZE);

    // �����, ����������� ��������� ��������
    void updateStickers();

    // ����� ��� ������ ��������� ��������
    void setPlane(CubePlane plane);

    // ����� ��� ������ "�������" ��������
    void setRowIndex(int rowIndex);

    // ����� ��� ������ ��������� "�������" ��������
    void nextRowIndex();

    // ����� ��� ������ ���������� "�������" ��������
    void previousRowIndex();

    // ����� ��� �������� ����� ��������� ��������� ��������� "�������" �� ����������� direction
    //  direction = false => ������ ��
    //  direction = true  => �� ��
    void rotate(bool direction);

private:
    // ������ ������ ������
    int size;

    // ���������� ������������� ������ ������
    RubiksCube rubiksCube;

    // ���������� ������������� �������� ������ � ���� �����
    array<vector<vector<GraphicObjectData>>, SIDE_COUNT> stickers;

    // ���������� ������������� �������� ������ � ���� �����
    vector<GraphicObjectData> pieces;

    // ��������� �������
    SelectedRow selectedRow;

private:
    Matrix getStickerTransform(int side, int x, int y, float angle = 0.0f);
};