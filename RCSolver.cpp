#include "RCSolver.h"
#include "RCSurface.h"

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

} // namespace Rc
