#pragma once

#include "Data.hpp"

class OrbitalCamera : Camera {
public:
    // ����������� �� ���������
    OrbitalCamera();

    // ����� ��� ��������� �������
    void recalculatePosition();

    // ����� ��� ������ ���������
    void beginMode3D();

    // ����� ��� ���������� ��������� ������
    void update();

private:
    // ���� �������� ������ �� �����������
    float angleX;

    // ���� �������� ������ �� ���������
    float angleY;

    // ���������� �� ����� (0, 0, 0)
    float radius;

    // ������ �� ���
    bool mouseDown;

    // ���������������� ������ �� ��� X
    const float SENSIVITY_X = 0.005f;

    // ���������������� ������ �� ��� Y
    const float SENSIVITY_Y = 0.005f;
};