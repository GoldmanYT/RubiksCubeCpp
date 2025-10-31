#pragma once

#include "Data.hpp"

enum LayoutConstraint {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_RIGHT,
    BOTTOM_LEFT
};

class Label {
public:
    // ����������� �� ���������
    Label();

    // �����������
    Label(LayoutConstraint labelConstraint, float offsetX, float offsetY, string text);

    // �����������
    Label(LayoutConstraint labelConstraint, float width, float height, float offsetX, float offsetY, string text);

    // ����� ��� ��������� �������
    void draw(int screenWidth, int screenHeight);

    // ����� ��� ��������� ������ �������
    Rectangle getRectangle(int screenWidth, int screenHeight);

    // ����� ��� ��������� ������
    void setText(string text);

protected:
    // ��������� �������
    LayoutConstraint constraint;

    // ������ ������� (0..1) ������������ ������ ����
    float width;
    // ������ ������� (0..1) ������������ ������ ����
    float height;

    // �������� �� X ������� (0..1) ������������ ������ ����
    float offsetX;
    // �������� �� Y ������� (0..1) ������������ ������ ����
    float offsetY;

    // ����� �������
    string text;
};