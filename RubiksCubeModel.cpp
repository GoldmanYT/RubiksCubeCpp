#include "RubiksCubeModel.hpp"

RubiksCubeModel::RubiksCubeModel()
    : rubiksCube()
    , size(DEFAULT_SIZE)
    , selectedRow(DEFAULT_SIZE)
{
}

RubiksCubeModel::RubiksCubeModel(int size)
    : rubiksCube(size)
    , size(size)
    , selectedRow(size)
{
}

void RubiksCubeModel::reset(int size)
{
    for (int side = 0; side < SIDE_COUNT; ++side) {
        stickers[side].resize(size);
        for (int x = 0; x < size; ++x) {
            stickers[side][x].resize(size);
            for (int y = 0; y < size; ++y) {
                stickers[side][x][y] = GraphicObjectData {
                    materials[side],
                    vector<Matrix> { getStickerTransform(side, x, y) }
                };
            }
        }
    }
    pieces.resize(size * size * size);
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            for (int z = 0; z < size; ++z) {
                float posX = x * PIECE_SIZE - PIECE_SIZE * (size - 1) / 2;
                float posY = y * PIECE_SIZE - PIECE_SIZE * (size - 1) / 2;
                float posZ = z * PIECE_SIZE - PIECE_SIZE * (size - 1) / 2;

                pieces[x * size * size + y * size + z] = GraphicObjectData {
                    materials[CUBE_MATERIAL],
                    vector<Matrix> { MatrixTranslate(posX, posY, posZ) }
                };
            }
        }
    }
    selectedRow.selectedPlane = XOY;
    selectedRow.selectedRowIndex = 0;
}

void RubiksCubeModel::updateStickers()
{
    array<vector<vector<StickerColor>>, SIDE_COUNT>& cubeStickers = rubiksCube.getStickers();
    for (int side = 0; side < SIDE_COUNT; ++side) {
        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                int materialColor = cubeStickers[side][x][y];
                stickers[side][x][y].material = materials[materialColor];
            }
        }
    }
}

void RubiksCubeModel::setPlane(CubePlane plane)
{
    selectedRow.selectedPlane = plane;
}

void RubiksCubeModel::setRowIndex(int rowIndex)
{
    selectedRow.selectedRowIndex = rowIndex % size;
}

void RubiksCubeModel::nextRowIndex()
{
    selectedRow.selectedRowIndex = (++selectedRow.selectedRowIndex) % size;
}

void RubiksCubeModel::previousRowIndex()
{
    selectedRow.selectedRowIndex = (--selectedRow.selectedRowIndex + size) % size;
}

void RubiksCubeModel::rotate(bool direction)
{
    rubiksCube.rotate(selectedRow.selectedPlane, selectedRow.selectedRowIndex, bool(direction));
    updateStickers();
}

SelectedRow::SelectedRow(int size)
    : size(size)
    , selectedPlane(XOY)
    , selectedRowIndex(0)
{
}

void SelectedRow::draw()
{
    float smallScale = PIECE_SIZE + COUNT_MARGINS * TINY_OFFSET;
    float bigScale = PIECE_SIZE * size + COUNT_MARGINS * TINY_OFFSET;

    float scaleX = smallScale;
    float scaleY = smallScale;
    float scaleZ = smallScale;

    float bigOffset = PIECE_SIZE * selectedRowIndex - PIECE_SIZE * (size - 1) / 2;

    float offsetX = 0.0f;
    float offsetY = 0.0f;
    float offsetZ = 0.0f;

    switch (selectedPlane) {
    case XOY:
        scaleX = bigScale;
        offsetY = bigOffset;
        scaleZ = bigScale;
        break;
    case YOZ:
        scaleX = bigScale;
        scaleY = bigScale;
        offsetZ = bigOffset;
        break;
    case ZOX:
        offsetX = bigOffset;
        scaleY = bigScale;
        scaleZ = bigScale;
        break;
    }

    Matrix transform = MatrixMultiply(
        MatrixScale(scaleX, scaleY, scaleZ),
        MatrixTranslate(offsetX, offsetY, offsetZ));

    if (alpha >= MAX_ALPHA) {
        animationDirection = true;
        alpha = max(MIN_ALPHA, alpha - ANIMATION_SPEED);
    } else if (alpha <= MIN_ALPHA) {
        animationDirection = false;
        alpha = min(MAX_ALPHA, alpha + ANIMATION_SPEED);
    } else if (animationDirection) {
        alpha = max(MIN_ALPHA, alpha - ANIMATION_SPEED);
    } else {
        alpha = min(MAX_ALPHA, alpha + ANIMATION_SPEED);
    }

    materials[SELECTED_ROW_MATERIAL].maps[MATERIAL_MAP_DIFFUSE].color.a = alpha;
    DrawMesh(selectedRowMesh, materials[SELECTED_ROW_MATERIAL], transform);
}

Matrix RubiksCubeModel::getStickerTransform(int side, int x, int y, float angle)
{
    float offset = PIECE_SIZE * (size - 1) / 2;
    float stickerOffset = offset + PIECE_SIZE / 2 + TINY_OFFSET;
    float offsetX = -offset + x * PIECE_SIZE;
    float offsetY = -offset + y * PIECE_SIZE;

    Matrix scale = MatrixScale(1.0f, 1.0f, 1.0f);
    Matrix rotation = MatrixIdentity();
    Matrix translation = MatrixIdentity();

    switch (side) {
    case STICKER_WHITE:
        rotation = MatrixRotateY(0.0f);
        translation = MatrixTranslate(offsetY, stickerOffset, offsetX);
        break;
    case STICKER_GREEN:
        rotation = MatrixRotateX(PI / 2);
        translation = MatrixTranslate(offsetX, offsetY, stickerOffset);
        break;
    case STICKER_RED:
        rotation = MatrixRotateZ(-PI / 2);
        translation = MatrixTranslate(stickerOffset, offsetX, offsetY);
        break;
    case STICKER_YELLOW:
        rotation = MatrixRotateY(PI);
        translation = MatrixTranslate(offsetY, -stickerOffset, offsetX);
        break;
    case STICKER_BLUE:
        rotation = MatrixRotateX(-PI / 2);
        translation = MatrixTranslate(offsetX, offsetY, -stickerOffset);
        break;
    case STICKER_ORANGE:
        rotation = MatrixRotateZ(PI / 2);
        translation = MatrixTranslate(-stickerOffset, offsetX, offsetY);
        break;
    }

    return MatrixMultiply(MatrixMultiply(scale, rotation), translation);
}

void RubiksCubeModel::draw()
{
    // Отрисовка кусочков кубика Рубика
    for (GraphicObjectData& piece : pieces) {
        Matrix transform = MatrixIdentity();
        for (Matrix& transform2 : piece.transforms) {
            transform = MatrixMultiply(transform, transform2);
        }
        DrawMesh(pieceMesh, piece.material, transform);
    }

    // Отрисовка стикеров кубика Рубика
    for (auto& stickerSide : stickers) {
        for (auto& stickerRow : stickerSide) {
            for (GraphicObjectData& sticker : stickerRow) {
                Matrix transform = MatrixIdentity();
                for (Matrix& transform2 : sticker.transforms) {
                    transform = MatrixMultiply(transform, transform2);
                }
                DrawMesh(stickerMesh, sticker.material, transform);
            }
        }
    }

    selectedRow.draw();
}
