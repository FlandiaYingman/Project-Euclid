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
        if (facingB == RCFacing::LEFT && facingC == RCFacing::UP) {
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
        if (facingB == RCFacing::LEFT && facingC == RCFacing::UP) {
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


std::string changeFacing(const RCCube &cube, const RCFacing &front, const RCFacing &up) {
    switch (front) {
    case RCFacing::LEFT:
        switch (up) {
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
        switch (up) {
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
        switch (up) {
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
        switch (up) {
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
        switch (up) {
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
        switch (up) {
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

} // namespace Rc
