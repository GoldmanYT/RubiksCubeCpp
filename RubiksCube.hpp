#pragma once

#include <array>
#include <vector>

#include "Data.hpp"

using namespace std;

// ����� ��� ������������� ������ ������
class RubiksCube {
public:
    // ����������� ��� 3x3x3
    RubiksCube();

    // ����������� ��� size x size x size
    RubiksCube(int size);

    // �����, ������������ ��� � �������� ���������
    void reset(int size = 3);

    // �����, ����������� ������� �����
    //  plane: ��� �������� �����
    //  rowIndex: ����� "�������", ������� ���������
    //  direction: ����������� �������� (false = ������ ��, true = �� ��)
    void rotate(CubePlane plane, int rowIndex, bool direction);

    // Getter ��� ���� size
    int getSize() const;

    // Getter ��� ���� stickers
    array<vector<vector<StickerColor>>, SIDE_COUNT>& getStickers();

private:
    int size;
    array<vector<vector<StickerColor>>, SIDE_COUNT> stickers;
};