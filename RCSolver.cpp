#include "RCSolver.h"
#include "RCSurface.h"

#include <iostream>
#include <stdexcept>
#include <string>

namespace Rc {

RCEdgePiece::RCEdgePiece(RCColor first, RCColor second) {
    this->first = first;
    this->second = second;
}


RCEdgePiecePos::RCEdgePiecePos() {
    this->first = RCFacing::NUL;
    this->second = RCFacing::NUL;
}
RCEdgePiecePos::RCEdgePiecePos(RCFacing first, RCFacing second) {
    this->first = first;
    this->second = second;
}


RCCornerPiece::RCCornerPiece(RCColor first, RCColor second, RCColor third) {
    this->first = first;
    this->second = second;
    this->third = third;
}


RCCornerPiecePos::RCCornerPiecePos() {
    this->first = RCFacing::NUL;
    this->second = RCFacing::NUL;
    this->third = RCFacing::NUL;
}
RCCornerPiecePos::RCCornerPiecePos(RCFacing first, RCFacing second, RCFacing third) {
    this->first = first;
    this->second = second;
    this->third = third;
}


bool operator==(const RCEdgePiece &lhs, const RCEdgePiece &rhs) {
    return lhs.first == rhs.first && lhs.second == rhs.second;
}
bool operator==(const RCCornerPiece &lhs, const RCCornerPiece &rhs) {
    return lhs.first == rhs.first && lhs.second == rhs.second && lhs.third == rhs.third;
}

static bool isAdjacent(const RCFacing &facingA, const RCFacing &facingB) {
    switch (facingA) {
    case RCFacing::LEFT:
        return !(facingB == RCFacing::LEFT || facingB == RCFacing::RIGHT);
    case RCFacing::RIGHT:
        return !(facingB == RCFacing::RIGHT || facingB == RCFacing::LEFT);
    case RCFacing::UP:
        return !(facingB == RCFacing::UP || facingB == RCFacing::DOWN);
    case RCFacing::DOWN:
        return !(facingB == RCFacing::DOWN || facingB == RCFacing::UP);
    case RCFacing::FRONT:
        return !(facingB == RCFacing::FRONT || facingB == RCFacing::BACK);
    case RCFacing::BACK:
        return !(facingB == RCFacing::BACK || facingB == RCFacing::FRONT);
    default:
        return false;
    }
}
static bool isAdjacent(const RCFacing &facingA, const RCFacing &facingB, const RCFacing &facingC) {
    switch (facingA) {
    case RCFacing::LEFT:
        if (facingB == RCFacing::BACK && facingC == RCFacing::UP) {
            return true;
        }
        if (facingB == RCFacing::UP && facingC == RCFacing::FRONT) {
            return true;
        }
        if (facingB == RCFacing::DOWN && facingC == RCFacing::BACK) {
            return true;
        }
        if (facingB == RCFacing::FRONT && facingC == RCFacing::DOWN) {
            return true;
        }
        break;
    case RCFacing::RIGHT:
        if (facingB == RCFacing::FRONT && facingC == RCFacing::UP) {
            return true;
        }
        if (facingB == RCFacing::UP && facingC == RCFacing::BACK) {
            return true;
        }
        if (facingB == RCFacing::DOWN && facingC == RCFacing::FRONT) {
            return true;
        }
        if (facingB == RCFacing::BACK && facingC == RCFacing::DOWN) {
            return true;
        }
        break;
    case RCFacing::UP:
        if (facingB == RCFacing::LEFT && facingC == RCFacing::BACK) {
            return true;
        }
        if (facingB == RCFacing::BACK && facingC == RCFacing::RIGHT) {
            return true;
        }
        if (facingB == RCFacing::FRONT && facingC == RCFacing::LEFT) {
            return true;
        }
        if (facingB == RCFacing::RIGHT && facingC == RCFacing::FRONT) {
            return true;
        }
        break;
    case RCFacing::DOWN:
        if (facingB == RCFacing::LEFT && facingC == RCFacing::FRONT) {
            return true;
        }
        if (facingB == RCFacing::FRONT && facingC == RCFacing::RIGHT) {
            return true;
        }
        if (facingB == RCFacing::BACK && facingC == RCFacing::LEFT) {
            return true;
        }
        if (facingB == RCFacing::RIGHT && facingC == RCFacing::BACK) {
            return true;
        }
        break;
    case RCFacing::FRONT:
        if (facingB == RCFacing::LEFT && facingC == RCFacing::UP) {
            return true;
        }
        if (facingB == RCFacing::UP && facingC == RCFacing::RIGHT) {
            return true;
        }
        if (facingB == RCFacing::DOWN && facingC == RCFacing::LEFT) {
            return true;
        }
        if (facingB == RCFacing::RIGHT && facingC == RCFacing::DOWN) {
            return true;
        }
        break;
    case RCFacing::BACK:
        if (facingB == RCFacing::RIGHT && facingC == RCFacing::UP) {
            return true;
        }
        if (facingB == RCFacing::UP && facingC == RCFacing::LEFT) {
            return true;
        }
        if (facingB == RCFacing::DOWN && facingC == RCFacing::RIGHT) {
            return true;
        }
        if (facingB == RCFacing::LEFT && facingC == RCFacing::DOWN) {
            return true;
        }
        break;
    }
    return false;
}
static RCDirection adjacentDirection(const RCFacing &facingA, const RCFacing &facingB) {
    switch (facingA) {
    case RCFacing::LEFT:
        switch (facingB) {
        case RCFacing::UP:
            return RCDirection::UP;
        case RCFacing::DOWN:
            return RCDirection::DOWN;
        case RCFacing::BACK:
            return RCDirection::LEFT;
        case RCFacing::FRONT:
            return RCDirection::RIGHT;
        }
        break;
    case RCFacing::RIGHT:
        switch (facingB) {
        case RCFacing::UP:
            return RCDirection::UP;
        case RCFacing::DOWN:
            return RCDirection::DOWN;
        case RCFacing::FRONT:
            return RCDirection::LEFT;
        case RCFacing::BACK:
            return RCDirection::RIGHT;
        }
        break;
    case RCFacing::UP:
        switch (facingB) {
        case RCFacing::BACK:
            return RCDirection::UP;
        case RCFacing::FRONT:
            return RCDirection::DOWN;
        case RCFacing::LEFT:
            return RCDirection::LEFT;
        case RCFacing::RIGHT:
            return RCDirection::RIGHT;
        }
        break;
    case RCFacing::DOWN:
        switch (facingB) {
        case RCFacing::FRONT:
            return RCDirection::UP;
        case RCFacing::BACK:
            return RCDirection::DOWN;
        case RCFacing::LEFT:
            return RCDirection::LEFT;
        case RCFacing::RIGHT:
            return RCDirection::RIGHT;
        }
        break;
    case RCFacing::FRONT:
        switch (facingB) {
        case RCFacing::UP:
            return RCDirection::UP;
        case RCFacing::DOWN:
            return RCDirection::DOWN;
        case RCFacing::LEFT:
            return RCDirection::LEFT;
        case RCFacing::RIGHT:
            return RCDirection::RIGHT;
        }
        break;
    case RCFacing::BACK:
        switch (facingB) {
        case RCFacing::UP:
            return RCDirection::UP;
        case RCFacing::DOWN:
            return RCDirection::DOWN;
        case RCFacing::RIGHT:
            return RCDirection::LEFT;
        case RCFacing::LEFT:
            return RCDirection::RIGHT;
        }
        break;
    }
    return RCDirection::NUL;
}
static RCDirection adjacentDirection(const RCFacing &facingA, const RCFacing &facingB, const RCFacing &facingC) {
    switch (facingA) {
    case RCFacing::LEFT:
        if (facingB == RCFacing::BACK && facingC == RCFacing::UP) {
            return RCDirection::UPPER_LEFT;
        }
        if (facingB == RCFacing::UP && facingC == RCFacing::FRONT) {
            return RCDirection::UPPER_RIGHT;
        }
        if (facingB == RCFacing::DOWN && facingC == RCFacing::BACK) {
            return RCDirection::LOWER_LEFT;
        }
        if (facingB == RCFacing::FRONT && facingC == RCFacing::DOWN) {
            return RCDirection::LOWER_RIGHT;
        }
        break;
    case RCFacing::RIGHT:
        if (facingB == RCFacing::FRONT && facingC == RCFacing::UP) {
            return RCDirection::UPPER_LEFT;
        }
        if (facingB == RCFacing::UP && facingC == RCFacing::BACK) {
            return RCDirection::UPPER_RIGHT;
        }
        if (facingB == RCFacing::DOWN && facingC == RCFacing::FRONT) {
            return RCDirection::LOWER_LEFT;
        }
        if (facingB == RCFacing::BACK && facingC == RCFacing::DOWN) {
            return RCDirection::LOWER_RIGHT;
        }
        break;
    case RCFacing::UP:
        if (facingB == RCFacing::LEFT && facingC == RCFacing::BACK) {
            return RCDirection::UPPER_LEFT;
        }
        if (facingB == RCFacing::BACK && facingC == RCFacing::RIGHT) {
            return RCDirection::UPPER_RIGHT;
        }
        if (facingB == RCFacing::FRONT && facingC == RCFacing::LEFT) {
            return RCDirection::LOWER_LEFT;
        }
        if (facingB == RCFacing::RIGHT && facingC == RCFacing::FRONT) {
            return RCDirection::LOWER_RIGHT;
        }
        break;
    case RCFacing::DOWN:
        if (facingB == RCFacing::LEFT && facingC == RCFacing::FRONT) {
            return RCDirection::UPPER_LEFT;
        }
        if (facingB == RCFacing::FRONT && facingC == RCFacing::RIGHT) {
            return RCDirection::UPPER_RIGHT;
        }
        if (facingB == RCFacing::BACK && facingC == RCFacing::LEFT) {
            return RCDirection::LOWER_LEFT;
        }
        if (facingB == RCFacing::RIGHT && facingC == RCFacing::BACK) {
            return RCDirection::LOWER_RIGHT;
        }
        break;
    case RCFacing::FRONT:
        if (facingB == RCFacing::LEFT && facingC == RCFacing::UP) {
            return RCDirection::UPPER_LEFT;
        }
        if (facingB == RCFacing::UP && facingC == RCFacing::RIGHT) {
            return RCDirection::UPPER_RIGHT;
        }
        if (facingB == RCFacing::DOWN && facingC == RCFacing::LEFT) {
            return RCDirection::LOWER_LEFT;
        }
        if (facingB == RCFacing::RIGHT && facingC == RCFacing::DOWN) {
            return RCDirection::LOWER_RIGHT;
        }
        break;
    case RCFacing::BACK:
        if (facingB == RCFacing::RIGHT && facingC == RCFacing::UP) {
            return RCDirection::UPPER_LEFT;
        }
        if (facingB == RCFacing::UP && facingC == RCFacing::LEFT) {
            return RCDirection::UPPER_RIGHT;
        }
        if (facingB == RCFacing::DOWN && facingC == RCFacing::RIGHT) {
            return RCDirection::LOWER_LEFT;
        }
        if (facingB == RCFacing::LEFT && facingC == RCFacing::DOWN) {
            return RCDirection::LOWER_RIGHT;
        }
        break;
    }
    return RCDirection::NUL;
}

static RCEdgePiece extractEdgePiece(const RCCube &cube, const RCFacing &facingA, const RCFacing &facingB) {
    RCSurface surfaceA = cube.get(facingA);
    RCSurface surfaceB = cube.get(facingB);
    RCColor colorA = surfaceA.get(adjacentDirection(facingA, facingB));
    RCColor colorB = surfaceB.get(adjacentDirection(facingB, facingA));
    return RCEdgePiece(colorA, colorB);
}
static RCEdgePiecePos extractEdgePiecePos(const RCFacing &facingA, const RCFacing &facingB) {
    return RCEdgePiecePos(facingA, facingB);
}
RCEdgePiecePos findEdge(const RCCube &cube, const RCEdgePiece &edge) {
    for (int i = (int)RCFacing::LEFT; i != (int)RCFacing::NUL; i++) {
        RCFacing iFacing = (RCFacing)i;
        for (int j = (int)RCFacing::LEFT; j != (int)RCFacing::NUL; j++) {
            RCFacing jFacing = (RCFacing)j;
            if (isAdjacent(iFacing, jFacing)) {
                RCEdgePiece extractedEdge = extractEdgePiece(cube, iFacing, jFacing);
                if (edge == extractedEdge) {
                    return extractEdgePiecePos(iFacing, jFacing);
                }
            }
        }
    }
    return RCEdgePiecePos();
}

static RCCornerPiece extractCornerPiece(const RCCube &cube, const RCFacing &facingA, const RCFacing &facingB, const RCFacing &facingC) {
    RCSurface surfaceA = cube.get(facingA);
    RCSurface surfaceB = cube.get(facingB);
    RCSurface surfaceC = cube.get(facingC);
    RCColor colorA = surfaceA.get(adjacentDirection(facingA, facingB, facingC));
    RCColor colorB = surfaceB.get(adjacentDirection(facingB, facingC, facingA));
    RCColor colorC = surfaceC.get(adjacentDirection(facingC, facingA, facingB));
    return RCCornerPiece(colorA, colorB, colorC);
}
static RCCornerPiecePos extractCornerPiecePos(const RCFacing &facingA, const RCFacing &facingB, const RCFacing &facingC) {
    return RCCornerPiecePos(facingA, facingB, facingC);
}
RCCornerPiecePos findCorner(const RCCube &cube, const RCCornerPiece &corner) {
    for (int i = (int)RCFacing::LEFT; i != (int)RCFacing::NUL; i++) {
        RCFacing iFacing = (RCFacing)i;
        for (int j = (int)RCFacing::LEFT; j != (int)RCFacing::NUL; j++) {
            RCFacing jFacing = (RCFacing)j;
            for (int k = (int)RCFacing::LEFT; k != (int)RCFacing::NUL; k++) {
                RCFacing kFacing = (RCFacing)k;
                if (isAdjacent(iFacing, jFacing, kFacing)) {
                    RCCornerPiece extractedCorner = extractCornerPiece(cube, iFacing, jFacing, kFacing);
                    if (corner == extractedCorner) {
                        return extractCornerPiecePos(iFacing, jFacing, kFacing);
                    }
                }
            }
        }
    }
    return RCCornerPiecePos();
}


std::string changeFacing(const RCCube &cube, const RCFacing &front, const RCFacing &down) {
    switch (front) {
    case RCFacing::LEFT:
        switch (down) {
        case RCFacing::UP:
            return "xd";
        case RCFacing::FRONT:
            return "xr xd";
        case RCFacing::DOWN:
            return "xr xr xd";
        case RCFacing::BACK:
            return "xl xd";
        }
        break;
    case RCFacing::RIGHT:
        switch (down) {
        case RCFacing::UP:
            return "xu";
        case RCFacing::BACK:
            return "xl xu";
        case RCFacing::DOWN:
            return "xl xl xu";
        case RCFacing::FRONT:
            return "xr xu";
        }
        break;
    case RCFacing::UP:
        switch (down) {
        case RCFacing::FRONT:
            return "xu xu xl";
        case RCFacing::LEFT:
            return "xd xl";
        case RCFacing::BACK:
            return "xl";
        case RCFacing::RIGHT:
            return "xu xl";
        }
        break;
        ;
    case RCFacing::DOWN:
        switch (down) {
        case RCFacing::FRONT:
            return "xr";
        case RCFacing::RIGHT:
            return "xu xr";
        case RCFacing::BACK:
            return "xu xu xr";
        case RCFacing::LEFT:
            return "xd xr";
        }
        break;
    case RCFacing::FRONT:
        switch (down) {
        case RCFacing::UP:
            return "";
        case RCFacing::RIGHT:
            return "xb";
        case RCFacing::DOWN:
            return "xf xf";
        case RCFacing::LEFT:
            return "xf";
        }
        break;
    case RCFacing::BACK:
        switch (down) {
        case RCFacing::UP:
            return "xu xu";
        case RCFacing::LEFT:
            return "xu xu xb";
        case RCFacing::DOWN:
            return "xu xu xf xf";
        case RCFacing::RIGHT:
            return "xu xu xf";
        }
        break;
    }
    return "null";
}


static void makeTopCross0(RCCube &cube, const RCFacing &facing, const RCDirection &direction) {
    switch (facing) {
    case RCFacing::FRONT:
        switch (direction) {
        case RCDirection::LEFT:
            cube.cd();
            makeTopCross0(cube, RCFacing::RIGHT, RCDirection::LEFT);
            cube.cu();
            break;
        case RCDirection::RIGHT:
            cube.cu();
            makeTopCross0(cube, RCFacing::LEFT, RCDirection::RIGHT);
            cube.cd();
            break;
        case RCDirection::UP:
            cube.f();
            makeTopCross0(cube, RCFacing::FRONT, RCDirection::RIGHT);
            break;
        case RCDirection::DOWN:
            cube.f();
            makeTopCross0(cube, RCFacing::FRONT, RCDirection::LEFT);
            break;
        default:
            throw std::logic_error("direction is invalid");
        }
        break;
    case RCFacing::BACK:
        switch (direction) {
        case RCDirection::LEFT:
            cube.cu().cu();
            makeTopCross0(cube, RCFacing::FRONT, RCDirection::LEFT);
            break;
        case RCDirection::RIGHT:
            cube.cu().cu();
            makeTopCross0(cube, RCFacing::FRONT, RCDirection::RIGHT);
            break;
        case RCDirection::UP:
            cube.cr();
            cube.d2();
            cube.cl();
            cube.d2();
            makeTopCross0(cube, RCFacing::FRONT, RCDirection::DOWN);
            break;
        case RCDirection::DOWN:
            cube.d2();
            makeTopCross0(cube, RCFacing::FRONT, RCDirection::DOWN);
            break;
        default:
            throw std::logic_error("direction is invalid");
        }
        break;
    case RCFacing::LEFT:
        switch (direction) {
        case RCDirection::LEFT:
            cube.cd();
            makeTopCross0(cube, RCFacing::FRONT, RCDirection::LEFT);
            break;
        case RCDirection::RIGHT:
            cube.f();
            break;
        case RCDirection::UP:
            cube.cb();
            cube.d2();
            cube.cf();
            cube.d2();
            makeTopCross0(cube, RCFacing::RIGHT, RCDirection::DOWN);
            break;
        case RCDirection::DOWN:
            cube.d();
            makeTopCross0(cube, RCFacing::FRONT, RCDirection::DOWN);
            break;
        default:
            throw std::logic_error("direction is invalid");
        }
        break;
    case RCFacing::RIGHT:
        switch (direction) {
        case RCDirection::LEFT:
            cube.fi();
            break;
        case RCDirection::RIGHT:
            cube.cd().cd().cd();
            makeTopCross0(cube, RCFacing::FRONT, RCDirection::RIGHT);
            break;
        case RCDirection::UP:
            cube.cf();
            cube.d2();
            cube.cb();
            cube.d2();
            makeTopCross0(cube, RCFacing::LEFT, RCDirection::DOWN);
            break;
        case RCDirection::DOWN:
            cube.di();
            makeTopCross0(cube, RCFacing::FRONT, RCDirection::DOWN);
            break;
        default:
            throw std::logic_error("direction is invalid");
        }
        break;
    case RCFacing::UP:
        switch (direction) {
        case RCDirection::LEFT:
            cube.l();
            makeTopCross0(cube, RCFacing::FRONT, RCDirection::LEFT);
            break;
        case RCDirection::RIGHT:
            cube.ri();
            makeTopCross0(cube, RCFacing::FRONT, RCDirection::RIGHT);
            break;
        case RCDirection::UP:
            cube.b();
            makeTopCross0(cube, RCFacing::LEFT, RCDirection::LEFT);
            break;
        case RCDirection::DOWN:
            break;
        default:
            throw std::logic_error("direction is invalid");
        }
        break;
    case RCFacing::DOWN:
        switch (direction) {
        case RCDirection::LEFT:
            cube.d();
            makeTopCross0(cube, RCFacing::DOWN, RCDirection::UP);
            break;
        case RCDirection::RIGHT:
            cube.di();
            makeTopCross0(cube, RCFacing::DOWN, RCDirection::UP);
            break;
        case RCDirection::UP:
            cube.cl();
            cube.d2();
            cube.cr();
            break;
        case RCDirection::DOWN:
            cube.d2();
            makeTopCross0(cube, RCFacing::DOWN, RCDirection::UP);
            break;
        default:
            throw std::logic_error("direction is invalid");
        }
        break;
    default:
        throw std::logic_error("facing is invalid");
    }
}

void makeTopCross(RCCube &cube) {
    RCEdgePiecePos whiteRedPos = findEdge(cube, RCEdgePiece(RCColor::WHITE, RCColor::RED));
    makeTopCross0(cube, whiteRedPos.first, adjacentDirection(whiteRedPos.first, whiteRedPos.second));
    cube.u();

    RCEdgePiecePos whiteBluePos = findEdge(cube, RCEdgePiece(RCColor::WHITE, RCColor::BLUE));
    makeTopCross0(cube, whiteBluePos.first, adjacentDirection(whiteBluePos.first, whiteBluePos.second));
    cube.u();

    RCEdgePiecePos whiteOrangePos = findEdge(cube, RCEdgePiece(RCColor::WHITE, RCColor::ORANGE));
    makeTopCross0(cube, whiteOrangePos.first, adjacentDirection(whiteOrangePos.first, whiteOrangePos.second));
    cube.u();

    RCEdgePiecePos whiteGreenPos = findEdge(cube, RCEdgePiece(RCColor::WHITE, RCColor::GREEN));
    makeTopCross0(cube, whiteGreenPos.first, adjacentDirection(whiteGreenPos.first, whiteGreenPos.second));
    cube.u();

    while (cube.getFront()[1][1] != RCColor::RED) {
        cube.cu();
    }
}


static void makeTopLayerLhs0(RCCube &cube) {
    cube.di().ri().d().r();
}

static void makeTopLayerRhs0(RCCube &cube) {
    cube.d().f().di().fi();
}

static void makeTopLayer0(RCCube &cube, const RCFacing &facing, const RCDirection &direction) {
    switch (facing) {
    case RCFacing::LEFT:
        switch (direction) {
        case RCDirection::UPPER_LEFT:
            cube.li().d().l();
            makeTopLayer0(cube, RCFacing::DOWN, RCDirection::LOWER_LEFT);
            break;
        case RCDirection::UPPER_RIGHT:
            cube.l().di().li();
            makeTopLayer0(cube, RCFacing::DOWN, RCDirection::UPPER_LEFT);
            break;
        case RCDirection::LOWER_LEFT:
            cube.d();
            makeTopLayer0(cube, RCFacing::FRONT, RCDirection::LOWER_LEFT);
            break;
        case RCDirection::LOWER_RIGHT:
            cube.d();
            makeTopLayer0(cube, RCFacing::FRONT, RCDirection::LOWER_RIGHT);
            break;
        default:
            throw std::logic_error("invalid direction");
        }
        break;
    case RCFacing::RIGHT:
        switch (direction) {
        case RCDirection::UPPER_LEFT:
            makeTopLayerRhs0(cube);
            makeTopLayer0(cube, RCFacing::RIGHT, RCDirection::LOWER_LEFT);
            break;
        case RCDirection::UPPER_RIGHT:
            cube.bi().d().b();
            makeTopLayer0(cube, RCFacing::RIGHT, RCDirection::LOWER_RIGHT);
            break;
        case RCDirection::LOWER_LEFT:
            makeTopLayerRhs0(cube);
            break;
        case RCDirection::LOWER_RIGHT:
            cube.di();
            makeTopLayer0(cube, RCFacing::FRONT, RCDirection::LOWER_RIGHT);
            break;
        default:
            throw std::logic_error("invalid direction");
        }
        break;
    case RCFacing::FRONT:
        switch (direction) {
        case RCDirection::UPPER_LEFT:
            cube.l().di().li();
            makeTopLayer0(cube, RCFacing::FRONT, RCDirection::LOWER_LEFT);
            break;
        case RCDirection::UPPER_RIGHT:
            makeTopLayerLhs0(cube);
            makeTopLayer0(cube, RCFacing::FRONT, RCDirection::LOWER_RIGHT);
            break;
        case RCDirection::LOWER_LEFT:
            cube.d();
            makeTopLayer0(cube, RCFacing::RIGHT, RCDirection::LOWER_LEFT);
            break;
        case RCDirection::LOWER_RIGHT:
            makeTopLayerLhs0(cube);
            break;
        default:
            throw std::logic_error("invalid direction");
        }
        break;
    case RCFacing::BACK:
        switch (direction) {
        case RCDirection::UPPER_LEFT:
            cube.r().di().ri();
            makeTopLayer0(cube, RCFacing::BACK, RCDirection::LOWER_LEFT);
            break;
        case RCDirection::UPPER_RIGHT:
            cube.li().d().l();
            makeTopLayer0(cube, RCFacing::BACK, RCDirection::LOWER_RIGHT);
            break;
        case RCDirection::LOWER_LEFT:
            cube.di();
            makeTopLayer0(cube, RCFacing::RIGHT, RCDirection::LOWER_LEFT);
            break;
        case RCDirection::LOWER_RIGHT:
            cube.di();
            makeTopLayer0(cube, RCFacing::RIGHT, RCDirection::LOWER_RIGHT);
            break;
        default:
            throw std::logic_error("invalid direction");
        }
        break;
    case RCFacing::UP:
        switch (direction) {
        case RCDirection::UPPER_LEFT:
            cube.b().d().bi();
            makeTopLayer0(cube, RCFacing::FRONT, RCDirection::LOWER_LEFT);
            break;
        case RCDirection::UPPER_RIGHT:
            cube.bi().di().b();
            makeTopLayer0(cube, RCFacing::FRONT, RCDirection::LOWER_RIGHT);
            break;
        case RCDirection::LOWER_LEFT:
            cube.l().d().li();
            makeTopLayer0(cube, RCFacing::RIGHT, RCDirection::LOWER_LEFT);
            break;
        case RCDirection::LOWER_RIGHT:
            cube.ri().d().r();
            makeTopLayer0(cube, RCFacing::RIGHT, RCDirection::LOWER_LEFT);
            break;
        default:
            throw std::logic_error("invalid direction");
        }
        break;
    case RCFacing::DOWN:
        switch (direction) {
        case RCDirection::UPPER_LEFT:
            cube.d();
            makeTopLayer0(cube, RCFacing::DOWN, RCDirection::UPPER_RIGHT);
            break;
        case RCDirection::UPPER_RIGHT:
            makeTopLayerLhs0(cube);
            makeTopLayer0(cube, RCFacing::FRONT, RCDirection::UPPER_RIGHT);
            break;
        case RCDirection::LOWER_LEFT:
            cube.d2();
            makeTopLayer0(cube, RCFacing::DOWN, RCDirection::UPPER_RIGHT);
            break;
        case RCDirection::LOWER_RIGHT:
            cube.di();
            makeTopLayer0(cube, RCFacing::DOWN, RCDirection::UPPER_RIGHT);
            break;
        default:
            throw std::logic_error("invalid direction");
        }
        break;
    default:
        throw std::logic_error("invalid facing");
    }
}

void makeTopLayer(RCCube &cube) {
    auto whiteBlueRedPos = findCorner(cube, RCCornerPiece(RCColor::WHITE, RCColor::BLUE, RCColor::RED));
    makeTopLayer0(cube, whiteBlueRedPos.first, adjacentDirection(whiteBlueRedPos.first, whiteBlueRedPos.second, whiteBlueRedPos.third));
    cube.u();
    cube.cu();

    auto whiteOrangeBluePos = findCorner(cube, RCCornerPiece(RCColor::WHITE, RCColor::ORANGE, RCColor::BLUE));
    makeTopLayer0(cube, whiteOrangeBluePos.first, adjacentDirection(whiteOrangeBluePos.first, whiteOrangeBluePos.second, whiteOrangeBluePos.third));
    cube.u();
    cube.cu();

    auto whiteGreenOrangePos = findCorner(cube, RCCornerPiece(RCColor::WHITE, RCColor::GREEN, RCColor::ORANGE));
    makeTopLayer0(cube, whiteGreenOrangePos.first, adjacentDirection(whiteGreenOrangePos.first, whiteGreenOrangePos.second, whiteGreenOrangePos.third));
    cube.u();
    cube.cu();

    auto whiteRedGreenPos = findCorner(cube, RCCornerPiece(RCColor::WHITE, RCColor::RED, RCColor::GREEN));
    makeTopLayer0(cube, whiteRedGreenPos.first, adjacentDirection(whiteRedGreenPos.first, whiteRedGreenPos.second, whiteRedGreenPos.third));
    cube.u();
    cube.cu();
}


static void makeMiddleLayerLhs0(RCCube &cube) {
    cube.fi().di().fi().di().fi().d().f().d().f();
}

static void makeMiddleLayerRhs0(RCCube &cube) {
    cube.r().d().r().d().r().di().ri().di().ri();
}

static void makeMiddleLayerMoveDownRhs0(RCCube &cube) {
    cube.f().d().f().d().f().di().fi().di().fi();
}

static void makeMiddleLayerMoveDown0(RCCube &cube, const RCFacing &facing, const RCDirection &direction) {
    switch (facing) {
    case RCFacing::FRONT:
        switch (direction) {
        case RCDirection::LEFT:
            makeMiddleLayerMoveDownRhs0(cube);
            cube.d2();
            break;
        case RCDirection::RIGHT:
            makeMiddleLayerLhs0(cube);
            cube.d2();
            break;
        default:
            throw std::logic_error("invalid facing");
        }
        break;
    case RCFacing::BACK:
        cube.xu().xu();
        makeMiddleLayerMoveDown0(cube, RCFacing::FRONT, direction);
        cube.xu().xu();
        break;
    case RCFacing::LEFT:
        cube.xu().xu().xu();
        makeMiddleLayerMoveDown0(cube, RCFacing::FRONT, direction);
        cube.xu();
        break;
    case RCFacing::RIGHT:
        cube.xu();
        makeMiddleLayerMoveDown0(cube, RCFacing::FRONT, direction);
        cube.xu().xu().xu();
        break;
    default:
        throw std::logic_error("invalid facing");
    }
}

static void makeMiddleLayer0(RCCube &cube, const RCFacing &facing, const RCDirection &direction) {
    switch (facing) {
    case RCFacing::LEFT:
        switch (direction) {
        case RCDirection::LEFT:
        case RCDirection::RIGHT:
            makeMiddleLayerMoveDown0(cube, facing, direction);
            makeMiddleLayer0(cube, facing, RCDirection::DOWN);
            break;
        case RCDirection::DOWN:
            cube.d();
            makeMiddleLayer0(cube, RCFacing::FRONT, RCDirection::DOWN);
            break;
        default:
            throw std::logic_error("invalid direction");
        }
        break;
    case RCFacing::RIGHT:
        switch (direction) {
        case RCDirection::LEFT:
        case RCDirection::RIGHT:
            makeMiddleLayerMoveDown0(cube, facing, direction);
            makeMiddleLayer0(cube, facing, RCDirection::DOWN);
            break;
        case RCDirection::DOWN:
            cube.di();
            makeMiddleLayer0(cube, RCFacing::FRONT, RCDirection::DOWN);
            break;
        default:
            throw std::logic_error("invalid direction");
        }
        break;
    case RCFacing::FRONT:
        switch (direction) {
        case RCDirection::LEFT:
            makeMiddleLayerMoveDown0(cube, facing, direction);
            makeMiddleLayer0(cube, facing, RCDirection::DOWN);
            break;
        case RCDirection::RIGHT:
            break;
        case RCDirection::DOWN:
            makeMiddleLayerLhs0(cube);
            break;
        default:
            throw std::logic_error("invalid direction");
        }
        break;
    case RCFacing::BACK:
        switch (direction) {
        case RCDirection::LEFT:
        case RCDirection::RIGHT:
            makeMiddleLayerMoveDown0(cube, facing, direction);
            makeMiddleLayer0(cube, facing, RCDirection::DOWN);
            break;
        case RCDirection::DOWN:
            cube.d2();
            makeMiddleLayer0(cube, RCFacing::FRONT, RCDirection::DOWN);
            break;
        default:
            throw std::logic_error("invalid direction");
        }
        break;
    case RCFacing::DOWN:
        switch (direction) {
        case RCDirection::LEFT:
            cube.d2();
            makeMiddleLayer0(cube, RCFacing::DOWN, RCDirection::RIGHT);
            break;
        case RCDirection::RIGHT:
            makeMiddleLayerRhs0(cube);
            break;
        case RCDirection::UP:
            cube.d();
            makeMiddleLayer0(cube, RCFacing::DOWN, RCDirection::RIGHT);
            break;
        case RCDirection::DOWN:
            cube.di();
            makeMiddleLayer0(cube, RCFacing::DOWN, RCDirection::RIGHT);
            break;
        default:
            throw std::logic_error("invalid direction");
        }
        break;
    default:
        throw std::logic_error("invalid facing");
    }
}

void makeMiddleLayer(RCCube &cube) {
    RCEdgePiecePos redBluePos = findEdge(cube, RCEdgePiece(RCColor::RED, RCColor::BLUE));
    makeMiddleLayer0(cube, redBluePos.first, adjacentDirection(redBluePos.first, redBluePos.second));
    cube.xu();

    RCEdgePiecePos blueOrangePos = findEdge(cube, RCEdgePiece(RCColor::BLUE, RCColor::ORANGE));
    makeMiddleLayer0(cube, blueOrangePos.first, adjacentDirection(blueOrangePos.first, blueOrangePos.second));
    cube.xu();

    RCEdgePiecePos orangeGreenPos = findEdge(cube, RCEdgePiece(RCColor::ORANGE, RCColor::GREEN));
    makeMiddleLayer0(cube, orangeGreenPos.first, adjacentDirection(orangeGreenPos.first, orangeGreenPos.second));
    cube.xu();

    RCEdgePiecePos greenRedPos = findEdge(cube, RCEdgePiece(RCColor::GREEN, RCColor::RED));
    makeMiddleLayer0(cube, greenRedPos.first, adjacentDirection(greenRedPos.first, greenRedPos.second));
    cube.xu();
}


static bool makeBottomCrossIsStraight0(const RCCube &cube) {
    auto down = cube.getDown();
    bool downStraight = down[1][0] == RCColor::YELLOW && down[1][1] == RCColor::YELLOW && down[1][2] == RCColor::YELLOW;
    auto front = cube.getFront();
    bool frontStraight = front[2][1] == RCColor::YELLOW;
    auto back = cube.getBack();
    bool backStraight = back[2][1] == RCColor::YELLOW;
    return downStraight && frontStraight && backStraight;
}

static bool makeBottomCrossIsBent0(const RCCube &cube) {
    auto down = cube.getDown();
    bool downStraight = down[1][1] == RCColor::YELLOW && down[1][2] == RCColor::YELLOW && down[2][1] == RCColor::YELLOW;
    auto front = cube.getFront();
    bool frontStraight = front[2][1] == RCColor::YELLOW;
    auto left = cube.getLeft();
    bool leftStraight = left[2][1] == RCColor::YELLOW;
    return downStraight && frontStraight && leftStraight;
}

static bool makeBottomCrossIsFinished0(const RCCube &cube) {
    auto down = cube.getDown();
    return down[1][0] == RCColor::YELLOW && down[1][1] == RCColor::YELLOW && down[1][2] == RCColor::YELLOW && down[0][1] == RCColor::YELLOW && down[2][1] == RCColor::YELLOW;
}

static bool makeBottomCrossIsPoint0(const RCCube &cube) {
    auto down = cube.getDown();
    return down[1][0] != RCColor::YELLOW && down[1][1] == RCColor::YELLOW && down[1][2] != RCColor::YELLOW && down[0][1] != RCColor::YELLOW && down[2][1] != RCColor::YELLOW;
}

static void makeBottomCrossStraight0(RCCube &cube) {
    cube.l().d().li().di().li().f().l().fi();
}

static void makeBottomCrossBent0(RCCube &cube) {
    cube.f().l().di().li().di().l().d().li().fi();
}

void makeBottomCross(RCCube &cube) {
    if (makeBottomCrossIsFinished0(cube)) {
        return;
    }
    if (makeBottomCrossIsPoint0(cube)) {
        makeBottomCrossStraight0(cube);
    }

    for (int i = 0; i < 4; i++) {
        if (makeBottomCrossIsStraight0(cube)) {
            makeBottomCrossStraight0(cube);
            return;
        }
        if (makeBottomCrossIsBent0(cube)) {
            makeBottomCrossBent0(cube);
            return;
        }
        cube.d();
    }

    throw std::logic_error("can't match any pattern with the cube");
}


static bool makeBottomSurfaceIsLhsFish(const RCCube &cube) {
    auto down = cube.getDown();
    auto front = cube.getFront();
    auto right = cube.getRight();
    auto back = cube.getBack();
    bool downFish = down[0][0] == RCColor::YELLOW && down[0][1] == RCColor::YELLOW && down[1][0] == RCColor::YELLOW && down[1][1] == RCColor::YELLOW && down[1][2] == RCColor::YELLOW && down[2][1] == RCColor::YELLOW;
    bool sideFish = front[2][2] == RCColor::YELLOW && right[2][2] == RCColor::YELLOW && back[2][2] == RCColor::YELLOW;
    return downFish && sideFish;
}

static bool makeBottomSurfaceIsRhsFish(const RCCube &cube) {
    auto down = cube.getDown();
    auto front = cube.getFront();
    auto left = cube.getLeft();
    auto back = cube.getBack();
    bool downFish = down[0][1] == RCColor::YELLOW && down[0][2] == RCColor::YELLOW && down[1][0] == RCColor::YELLOW && down[1][1] == RCColor::YELLOW && down[1][2] == RCColor::YELLOW && down[2][1] == RCColor::YELLOW;
    bool sideFish = front[2][0] == RCColor::YELLOW && left[2][0] == RCColor::YELLOW && back[2][0] == RCColor::YELLOW;
    return downFish && sideFish;
}

static bool makeBottomSurfaceIsException(const RCCube &cube) {
    return cube.getFront()[2][2] == RCColor::YELLOW;
}

static bool makeBottomSurfaceIsFinished(const RCCube &cube) {
    auto down = cube.getDown();
    return down[0][0] == RCColor::YELLOW && down[0][1] == RCColor::YELLOW && down[0][2] == RCColor::YELLOW && down[1][0] == RCColor::YELLOW && down[1][1] == RCColor::YELLOW && down[1][2] == RCColor::YELLOW && down[2][0] == RCColor::YELLOW &&
           down[2][1] == RCColor::YELLOW && down[2][2] == RCColor::YELLOW;
}

static void makeBottomSurfaceLhsFish(RCCube &cube) {
    cube.ri().di().r().di().ri().d2().r();
}

static void makeBottomSurfaceRhsFish(RCCube &cube) {
    cube.l().d().li().d().l().d2().li();
}

void makeBottomSurface(RCCube &cube) {
    if (makeBottomSurfaceIsFinished(cube)) {
        return;
    }
    for (int i = 0; i < 4; i++) {
        if (makeBottomSurfaceIsLhsFish(cube)) {
            makeBottomSurfaceLhsFish(cube);
            return;
        }
        if (makeBottomSurfaceIsRhsFish(cube)) {
            makeBottomSurfaceRhsFish(cube);
            return;
        }
        cube.d();
    }
    for (int i = 0; i < 4; i++) {
        if (makeBottomSurfaceIsException(cube)) {
            makeBottomSurfaceRhsFish(cube);
            makeBottomSurface(cube);
            return;
        }
        cube.d();
    }
    throw std::logic_error("can't match any pattern with the cube");
}


static bool makeBottomCornerAreEyes(const RCCube &cube) {
    auto right = cube.getRight();
    return right[2][0] == right[2][2];
}

static bool makeBottomCornerIsFinished(const RCCube &cube) {
    auto eyesCount = 0;
    auto front = cube.getFront();
    if (front[2][0] == front[2][2]) {
        eyesCount++;
    }
    auto back = cube.getBack();
    if (back[2][0] == back[2][2]) {
        eyesCount++;
    }
    auto left = cube.getLeft();
    if (left[2][0] == left[2][2]) {
        eyesCount++;
    }
    auto right = cube.getRight();
    if (right[2][0] == right[2][2]) {
        eyesCount++;
    }
    return eyesCount >= 3;
}

static void makeBottomCornerStraight(RCCube &cube) {
    cube.l().d().li().di().li().f().l().fi();
}

static void makeBottomCornerBent(RCCube &cube) {
    cube.f().l().di().li().di().l().d().li().fi();
}

static void makeBottomCornerAlign(RCCube &cube) {
    while (cube.getFront()[2][2] != RCColor::RED) {
        cube.d();
    }
}

void makeBottomCorner(RCCube &cube) {
    if (makeBottomCornerIsFinished(cube)) {
        makeBottomCornerAlign(cube);
        return;
    }
    for (int i = 0; i < 4; i++) {
        if (makeBottomCornerAreEyes(cube)) {
            makeBottomCornerStraight(cube);
            makeBottomCornerBent(cube);
            makeBottomCornerAlign(cube);
            return;
        }
        cube.d();
    }
    makeBottomCornerBent(cube);
    makeBottomCornerStraight(cube);
    makeBottomCornerAlign(cube);
}


static bool makeBottomEdgeAreEyes(RCSurface surface) {
    return surface[2][0] == surface[2][2] && surface[2][0] != surface[2][1];
}

static void makeBottomEdgeLhsFish(RCCube &cube) {
    cube.ri().di().r().di().ri().d2().r();
}

static void makeBottomEdgeRhsFish(RCCube &cube) {
    cube.l().d().li().d().l().d2().li();
}

static void makeBottomEdgeAlign(RCCube &cube) {
    while (cube.getFront()[2][2] != RCColor::RED) {
        cube.d();
    }
}

void makeBottomEdge(RCCube &cube) {
    bool leftEyes = makeBottomEdgeAreEyes(cube.getLeft());
    bool frontEyes = makeBottomEdgeAreEyes(cube.getFront());
    bool rightEyes = makeBottomEdgeAreEyes(cube.getRight());
    bool backEyes = makeBottomEdgeAreEyes(cube.getBack());
    auto eyesCount = 0;
    if (leftEyes) {
        eyesCount++;
    }
    if (frontEyes) {
        eyesCount++;
    }
    if (rightEyes) {
        eyesCount++;
    }
    if (backEyes) {
        eyesCount++;
    }
    if (eyesCount == 3) {
        if (!leftEyes) {
            cube.di();
        }
        if (!frontEyes) {
            cube.d2();
        }
        if (!rightEyes) {
            cube.d();
        }
        auto frontEyesColor = cube.getFront()[2][0];
        auto leftColor = cube.getLeft()[2][1];
        if (leftColor == frontEyesColor) {
            makeBottomEdgeRhsFish(cube);
            cube.d();
            makeBottomEdgeLhsFish(cube);
        } else {
            makeBottomEdgeLhsFish(cube);
            cube.di();
            makeBottomEdgeRhsFish(cube);
        }
        makeBottomCornerAlign(cube);
    } else if (eyesCount == 4) {
        makeBottomEdgeRhsFish(cube);
        cube.d();
        makeBottomEdgeLhsFish(cube);
        makeBottomEdge(cube);
    } else if (eyesCount == 0) {
        return;
    } else {
        throw std::logic_error("there are not 3 or 4 eyes");
    }
}

} // namespace Rc
