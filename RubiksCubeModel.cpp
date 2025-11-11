#include "RubiksCubeModel.hpp"

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

void Scrambler::stop()
{
    rotationCount = ROTATION_COUNT;
}

void Scrambler::update(RubiksCubeModel& rubiksCubeModel, SelectedRow& selectedRow)
{
    if (!isScrambling()) {
        return;
    }
    if (selectedRow.angle == 0.0f) {
        CubePlane plane = CubePlane(rand() % PLANE_COUNT);
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
    float smallScale = pieceSize + COUNT_MARGINS * TINY_OFFSET;
    float bigScale = pieceSize * size + COUNT_MARGINS * TINY_OFFSET;

    float scaleX = smallScale;
    float scaleY = smallScale;
    float scaleZ = smallScale;

    float bigOffset = pieceSize * selectedRowIndex - pieceSize * (size - 1) / 2;

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

    Matrix scale = MatrixScale(scaleX, scaleY, scaleZ);
    Matrix translate = MatrixTranslate(offsetX, offsetY, offsetZ);
    Matrix transform = scale * translate * rotation;

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

bool SelectedRow::isRotating()
{
    return angle != 0.0f;
}

SelectedSticker::SelectedSticker()
    : selected(false)
    , rotated(false)
    , side(0)
    , x(0)
    , y(0)
    , viewRay()
{
}

void SelectedSticker::select(int side, int x, int y, Ray viewRay)
{
    selected = true;
    this->side = side;
    this->x = x;
    this->y = y;
    this->viewRay = viewRay;
}

void SelectedSticker::deselect()
{
    selected = false;
    rotated = false;
}

RotationData SelectedSticker::getRotationData(Ray currentViewRay)
{
    RotationData rotationData = {};

    rotationData.isRotated = !rotated && Vector3Angle(currentViewRay.direction, viewRay.direction) >= MIN_ROTATION_DEGREE;
    rotated |= rotationData.isRotated;

    Vector3 direction = currentViewRay.direction - viewRay.direction;
    Vector2 projection = {};

    switch (side) {
    case STICKER_WHITE:
        projection = { direction.z, direction.x };
        break;
    case STICKER_GREEN:
        projection = { direction.x, direction.y };
        break;
    case STICKER_RED:
        projection = { direction.y, direction.z };
        break;
    case STICKER_YELLOW:
        projection = { direction.z, direction.x };
        break;
    case STICKER_BLUE:
        projection = { direction.x, direction.y };
        break;
    case STICKER_ORANGE:
        projection = { direction.y, direction.z };
        break;
    }

    bool rotateByX = abs(projection.x) > abs(projection.y);

    switch (side % PLANE_COUNT) {
    case ZOX:
        rotationData.plane = rotateByX ? YOZ : XOY;
        break;
    case XOY:
        rotationData.plane = rotateByX ? ZOX : YOZ;
        break;
    case YOZ:
        rotationData.plane = rotateByX ? XOY : ZOX;
        break;
    }

    if (side < PLANE_COUNT) {
        if (rotateByX) {
            rotationData.direction = projection.x < 0.0f;
        } else {
            rotationData.direction = projection.y > 0.0f;
        }
    } else {
        if (rotateByX) {
            rotationData.direction = projection.x > 0.0f;
        } else {
            rotationData.direction = projection.y < 0.0f;
        }
    }

    if (rotateByX) {
        rotationData.rowIndex = y;
    } else {
        rotationData.rowIndex = x;
    }

    return rotationData;
}

inline bool SelectedSticker::getSelected()
{
    return selected;
}

RubiksCubeModel::RubiksCubeModel()
    : size(DEFAULT_SIZE)
    , moveCount(0)
    , rubiksCube()
    , selectedRow(DEFAULT_SIZE)
    , scrambler()
    , selectedSticker()
{
}

RubiksCubeModel::RubiksCubeModel(int size)
    : size(size)
    , moveCount(0)
    , rubiksCube(size)
    , selectedRow(size)
    , scrambler(size)
    , selectedSticker()
{
}

void RubiksCubeModel::reset(int size)
{
    this->size = size;
    moveCount = 0;
    rubiksCube = RubiksCube(size);
    selectedRow.size = size;
    selectedRow.selectedRowIndex = 0;
    scrambler = Scrambler(size);
    selectedSticker.deselect();

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

    pieceSize = PIECE_SIZE_K / size;
    stickerSize = STICKER_SIZE_K / size;
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

void RubiksCubeModel::rotate(bool direction, bool byButton)
{
    if (!scrambler.isScrambling()) {
        moveCount++;
    }
    if (byButton) {
        direction ^= selectedRow.selectedRowIndex < size / 2;
    }
    rubiksCube.rotate(selectedRow.selectedPlane, selectedRow.selectedRowIndex, direction);
    selectedRow.rotate(direction);

    updateStickers();
}

void RubiksCubeModel::scramble()
{
    solve();
    scrambler.scramble();
    moveCount = 0;
}

void RubiksCubeModel::solve()
{
    rubiksCube.solve();
    scrambler.stop();
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

Matrix RubiksCubeModel::getStickerTransform(int side, int x, int y, bool forSwipe)
{
    float offset = pieceSize * (size - 1) / 2;
    float stickerOffset = offset + pieceSize / 2 + TINY_OFFSET;
    float offsetX = -offset + x * pieceSize;
    float offsetY = -offset + y * pieceSize;

    Matrix rotation = MatrixIdentity();
    Matrix rotation2 = MatrixIdentity();
    Matrix translation = MatrixIdentity();
    Matrix scale = MatrixIdentity();
    if (forSwipe) {
        scale = MatrixScale(pieceSize, pieceSize, pieceSize);
    } else {
        scale = MatrixScale(stickerSize, stickerSize, stickerSize);
    }

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

    return scale * rotation * translation * rotation2;
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
    int* sides = ROTATION_SIDES[plane];

    for (int sideIndex = 0; sideIndex < 4; ++sideIndex) {
        if (side == sides[sideIndex]) {
            if (sideIndex % 2 != 0) {
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
    float offset = pieceSize * (size - 1) / 2;
    float posX = x * pieceSize - offset;
    float posY = y * pieceSize - offset;
    float posZ = z * pieceSize - offset;

    Matrix translate = MatrixTranslate(posX, posY, posZ);
    Matrix rotation = MatrixIdentity();
    Matrix scale = MatrixScale(pieceSize, pieceSize, pieceSize);

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

    return scale * translate * rotation;
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
                bool pieceDraw = x == 0 || x == size - 1 || y == 0 || y == size - 1 || z == 0 || z == size - 1 || isPieceRotated(x, y, z);

                if (pieceDraw) {
                    DrawMesh(pieceMesh, piece.material, piece.transform);
                }
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

    if (rotationMode == MODE_BUTTONS) {
        // Отрисовка выбранной "полоски"
        selectedRow.draw();
    }
}

bool RubiksCubeModel::update(Camera camera, bool isMouseDown, Vector2 mousePos)
{
    // Обновление выбранной "полоски"
    selectedRow.update();

    // Обновление замешивателя кубика Рубика
    scrambler.update(*this, selectedRow);

    if (!isMouseDown || rotationMode == MODE_BUTTONS) {
        selectedSticker.deselect();
        return false;
    }

    if (selectedElement != SELECTED_STICKER && selectedElement != SELECTED_NOTHING) {
        return false;
    }

    Ray viewRay = GetScreenToWorldRay(mousePos, camera);

    if (selectedSticker.getSelected()) {
        RotationData rotationData = selectedSticker.getRotationData(viewRay);

        if (rotationData.isRotated) {
            selectedRow.selectedRowIndex = rotationData.rowIndex;
            selectedRow.selectedPlane = rotationData.plane;
            rotate(rotationData.direction);
        }

        return true;
    }

    for (int side = 0; side < SIDE_COUNT; ++side) {
        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                RayCollision collision = GetRayCollisionMesh(viewRay, stickerMesh, getStickerTransform(side, x, y, true));

                if (collision.hit && Vector3DotProduct(collision.normal, viewRay.direction) < 0.0f && isMouseDown) {
                    selectedSticker.select(side, x, y, viewRay);
                }
            }
        }
    }

    return selectedSticker.getSelected() || (selectedRow.isRotating() && !scrambler.isScrambling());
}
