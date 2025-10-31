#include "RubiksCubeModel.hpp"

RubiksCubeModel::RubiksCubeModel()
    : rubiksCube()
    , size(DEFAULT_SIZE)
    , selectedRow(DEFAULT_SIZE)
    , scrambler()
    , moveCount(0)
{
}

RubiksCubeModel::RubiksCubeModel(int size)
    : rubiksCube(size)
    , size(size)
    , selectedRow(size)
    , scrambler(size)
    , moveCount(0)
{
}

void RubiksCubeModel::reset(int size)
{
    this->size = size;
    rubiksCube = RubiksCube(size);
    selectedRow.size = size;
    selectedRow.selectedRowIndex = 0;
    scrambler = Scrambler(size);
    moveCount = 0;

    for (int side = 0; side < SIDE_COUNT; ++side) {
        stickers[side].resize(size);
        for (int x = 0; x < size; ++x) {
            stickers[side][x].resize(size);
            for (int y = 0; y < size; ++y) {
                stickers[side][x][y] = GraphicObjectData {
                    materials[side],
                    getStickerTransform(side, x, y)
                };
            }
        }
    }
    pieces.resize(size);
    for (int x = 0; x < size; ++x) {
        pieces[x].resize(size);
        for (int y = 0; y < size; ++y) {
            pieces[x][y].resize(size);
            for (int z = 0; z < size; ++z) {
                pieces[x][y][z] = GraphicObjectData {
                    materials[CUBE_MATERIAL],
                    getPieceTransform(x, y, z)
                };
            }
        }
    }
    selectedRow.selectedPlane = ZOX;
    selectedRow.selectedRowIndex = 0;

    camera = Camera {
        { size * 3.0f, size * 3.0f, size * 3.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        size * 3.0f,
        CAMERA_ORTHOGRAPHIC
    };
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
    if (!scrambler.isScrambling()) {
        moveCount++;
    }
    rubiksCube.rotate(selectedRow.selectedPlane, selectedRow.selectedRowIndex, bool(direction));
    selectedRow.rotate(direction != ROTATION_DIRECTION[selectedRow.selectedPlane]);

    updateStickers();
}

Scrambler::Scrambler()
    : size(DEFAULT_SIZE)
    , rotationCount(ROTATION_COUNT_K * size)
    , ROTATION_COUNT(rotationCount)
{
}

Scrambler::Scrambler(int size)
    : size(size)
    , rotationCount(ROTATION_COUNT_K * size)
    , ROTATION_COUNT(rotationCount)
{
}

void Scrambler::scramble()
{
    rotationCount = 0;
}

void Scrambler::update(RubiksCubeModel& rubiksCubeModel, SelectedRow& selectedRow)
{
    if (!isScrambling()) {
        return;
    }
    if (selectedRow.angle == 0.0f) {
        CubePlane plane = CubePlane(rand() % (SIDE_COUNT / 2));
        int rowIndex = rand() % size;
        bool direction = rand() % 2;

        selectedRow.selectedPlane = plane;
        selectedRow.selectedRowIndex = rowIndex;
        rubiksCubeModel.rotate(direction);
        ++rotationCount;
    }
}

bool Scrambler::isScrambling()
{
    return rotationCount < ROTATION_COUNT;
}

void RubiksCubeModel::scramble()
{
    scrambler.scramble();
    moveCount = 0;
}

void RubiksCubeModel::solve()
{
    rubiksCube.solve();
    updateStickers();
    moveCount = 0;
}

void RubiksCubeModel::increaseSize()
{
    reset(min(MAX_SIZE, size + 1));
}

void RubiksCubeModel::decreaseSize()
{
    reset(max(MIN_SIZE, size - 1));
}

int RubiksCubeModel::getMoveCount()
{
    return moveCount;
}

bool RubiksCubeModel::isSolved()
{
    return rubiksCube.isSolved();
}

SelectedRow::SelectedRow(int size)
    : size(size)
    , selectedPlane(ZOX)
    , selectedRowIndex(0)
    , angle(0.0f)
    , rotationDirection(false)
    , alpha(0)
    , animationDirection(false)
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

    Matrix rotation = MatrixIdentity();

    switch (selectedPlane) {
    case ZOX:
        scaleX = bigScale;
        offsetY = bigOffset;
        scaleZ = bigScale;
        rotation = MatrixRotateY(angle);
        break;
    case XOY:
        scaleX = bigScale;
        scaleY = bigScale;
        offsetZ = bigOffset;
        rotation = MatrixRotateZ(angle);
        break;
    case YOZ:
        offsetX = bigOffset;
        scaleY = bigScale;
        scaleZ = bigScale;
        rotation = MatrixRotateX(angle);
        break;
    }

    Matrix transform = MatrixMultiply(
        MatrixMultiply(
            MatrixScale(scaleX, scaleY, scaleZ),
            MatrixTranslate(offsetX, offsetY, offsetZ)),
        rotation);

    DrawMesh(selectedRowMesh, materials[SELECTED_ROW_MATERIAL], transform);
}

void SelectedRow::update()
{
    float frameTime = GetFrameTime();
    float deltaAlpha = frameTime * ANIMATION_SPEED;
    if (alpha >= MAX_ALPHA) {
        animationDirection = true;
        alpha = max(MIN_ALPHA, alpha - deltaAlpha);
    } else if (alpha <= MIN_ALPHA) {
        animationDirection = false;
        alpha = min(MAX_ALPHA, alpha + deltaAlpha);
    } else if (animationDirection) {
        alpha = max(MIN_ALPHA, alpha - deltaAlpha);
    } else {
        alpha = min(MAX_ALPHA, alpha + deltaAlpha);
    }

    materials[SELECTED_ROW_MATERIAL].maps[MATERIAL_MAP_DIFFUSE].color.a = char(alpha);

    // По часовой стрелке
    if (rotationDirection) {
        angle -= frameTime * ROTATION_SPEED;
        angle = max(angle, 0.0f);
    } else {
        angle += frameTime * ROTATION_SPEED;
        angle = min(angle, 0.0f);
    }
}

void SelectedRow::rotate(bool direction)
{
    // По часовой стрелке
    if (direction) {
        angle = PI / 2;
    } else {
        angle = -PI / 2;
    }
    rotationDirection = direction;
}

Matrix RubiksCubeModel::getStickerTransform(int side, int x, int y)
{
    float offset = PIECE_SIZE * (size - 1) / 2;
    float stickerOffset = offset + PIECE_SIZE / 2 + TINY_OFFSET;
    float offsetX = -offset + x * PIECE_SIZE;
    float offsetY = -offset + y * PIECE_SIZE;

    Matrix rotation = MatrixIdentity();
    Matrix rotation2 = MatrixIdentity();
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
        rotation = MatrixRotateX(PI);
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

    float angle = selectedRow.angle;
    if (isStickerRotated(side, x, y)) {
        switch (selectedRow.selectedPlane) {
        case ZOX:
            rotation2 = MatrixRotateY(angle);
            break;
        case XOY:
            rotation2 = MatrixRotateZ(angle);
            break;
        case YOZ:
            rotation2 = MatrixRotateX(angle);
            break;
        }
    }

    return MatrixMultiply(MatrixMultiply(rotation, translation), rotation2);
}

bool RubiksCubeModel::isStickerRotated(int side, int x, int y)
{
    int plane = selectedRow.selectedPlane;
    int rowIndex = selectedRow.selectedRowIndex;
    int rotationSide = -1;

    if (rowIndex == 0) {
        // Самая ближняя сторона к плоскости
        rotationSide = ROTATION_SIDE[plane][0];
    } else if (rowIndex == size - 1) {
        // Самая дальняя сторона от плоскости
        rotationSide = ROTATION_SIDE[plane][1];
    }

    bool rotated = rotationSide == side;
    RotationData rotationData = ROTATION_SIDES[plane];
    bool indexOrder = rotationData.initialIndex;
    int* sides = rotationData.sides;

    for (int sideIndex = 0; sideIndex < 4; ++sideIndex, indexOrder = !indexOrder) {
        if (side == sides[sideIndex]) {
            if (indexOrder) {
                rotated = rowIndex == x;
            } else {
                rotated = rowIndex == y;
            }
        }
    }

    return rotated;
}

Matrix RubiksCubeModel::getPieceTransform(int x, int y, int z)
{
    float offset = PIECE_SIZE * (size - 1) / 2;
    float posX = x * PIECE_SIZE - offset;
    float posY = y * PIECE_SIZE - offset;
    float posZ = z * PIECE_SIZE - offset;

    Matrix translate = MatrixTranslate(posX, posY, posZ);
    Matrix rotation = MatrixIdentity();

    float angle = selectedRow.angle;
    if (isPieceRotated(x, y, z)) {
        switch (selectedRow.selectedPlane) {
        case ZOX:
            rotation = MatrixRotateY(angle);
            break;
        case XOY:
            rotation = MatrixRotateZ(angle);
            break;
        case YOZ:
            rotation = MatrixRotateX(angle);
            break;
        }
    }

    return MatrixMultiply(translate, rotation);
}

bool RubiksCubeModel::isPieceRotated(int x, int y, int z)
{
    CubePlane plane = selectedRow.selectedPlane;
    int rowIndex = selectedRow.selectedRowIndex;

    switch (plane) {
    case ZOX:
        return rowIndex == y;
    case XOY:
        return rowIndex == z;
    case YOZ:
        return rowIndex == x;
    }
    return false;
}

void RubiksCubeModel::draw()
{
    // Отрисовка кусочков кубика Рубика
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            for (int z = 0; z < size; ++z) {
                GraphicObjectData& piece = pieces[x][y][z];
                piece.transform = getPieceTransform(x, y, z);
                DrawMesh(pieceMesh, piece.material, piece.transform);
            }
        }
    }

    // Отрисовка стикеров кубика Рубика
    for (int side = 0; side < SIDE_COUNT; ++side) {
        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                GraphicObjectData& sticker = stickers[side][x][y];
                sticker.transform = getStickerTransform(side, x, y);
                DrawMesh(stickerMesh, sticker.material, sticker.transform);
            }
        }
    }

    // Отрисовка выбранной "полоски"
    selectedRow.draw();
}

void RubiksCubeModel::update()
{
    // Обновление выбранной "полоски"
    selectedRow.update();

    // Обновление замешивателя кубика Рубика
    scrambler.update(*this, selectedRow);
}
