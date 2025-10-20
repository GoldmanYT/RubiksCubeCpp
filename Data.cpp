#include "Data.hpp"

const int FPS = 60;

const int WINDOW_WIDTH = 800;

const int WINDOW_HEIGHT = 600;

Camera camera;

const int DEFAULT_SIZE = 3;

RotationData ROTATION_SIDES[3] = {
    { false, { STICKER_GREEN, STICKER_ORANGE, STICKER_BLUE, STICKER_RED } },
    { true, { STICKER_WHITE, STICKER_ORANGE, STICKER_YELLOW, STICKER_RED } },
    { false, { STICKER_WHITE, STICKER_BLUE, STICKER_YELLOW, STICKER_GREEN } }
};

int ROTATION_SIDE[3][2] = {
    { STICKER_YELLOW, STICKER_WHITE },
    { STICKER_BLUE, STICKER_GREEN },
    { STICKER_ORANGE, STICKER_RED }
};

array<Color, SIDE_COUNT + 2> colors = { WHITE, GREEN, RED, YELLOW, BLUE, ORANGE, BLACK, LIGHTGRAY };

const float TINY_OFFSET = 0.001f;

float STICKER_SIZE = 0.9f;

Mesh stickerMesh;

float PIECE_SIZE = 1.0f;

const int CUBE_MATERIAL = SIDE_COUNT;

Mesh pieceMesh;

vector<Material> materials;

const int SELECTED_ROW_MATERIAL = SIDE_COUNT + 1;

Mesh selectedRowMesh;

int COUNT_MARGINS = 3;

void initData()
{
    for (int colorIndex = 0; colorIndex < colors.size(); ++colorIndex) {
        materials.push_back(LoadMaterialDefault());
        MaterialMap& map = materials[colorIndex].maps[MATERIAL_MAP_DIFFUSE];
        map.color = colors[colorIndex];
    }
    stickerMesh = GenMeshPlane(STICKER_SIZE, STICKER_SIZE, 1, 1);
    pieceMesh = GenMeshCube(PIECE_SIZE, PIECE_SIZE, PIECE_SIZE);
    selectedRowMesh = GenMeshCube(1.0f, 1.0f, 1.0f);
    camera = Camera { { 10, 10, 10 }, { 0, 0, 0 }, { 0, 1, 0 }, 5, CAMERA_ORTHOGRAPHIC };
}
