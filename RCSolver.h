#if !defined(RC_SOLVER_H_)
#define RC_SOLVER_H_

#include "RCColor.h"
#include "RCCube.h"

namespace Rc {

struct RCEdgePiece {
    RCColor first, second; // Front and back
    RCEdgePiece(RCColor first, RCColor second);
};

struct RCEdgePiecePos {
    RCFacing first, second; // Front and back
    RCEdgePiecePos();
    RCEdgePiecePos(RCFacing first, RCFacing second);
};

struct RCCornerPiece {
    RCColor first, second, third; // Clockwise
    RCCornerPiece(RCColor first, RCColor second, RCColor third);
};

struct RCCornerPiecePos {
    RCFacing first, second, third; // Clockwise
    RCCornerPiecePos();
    RCCornerPiecePos(RCFacing first, RCFacing second, RCFacing third);
};

RCEdgePiecePos findEdge(const RCCube &cube, const RCEdgePiece &edge);
RCCornerPiecePos findCorner(const RCCube &cube, const RCCornerPiece &corner);

} // namespace Rc

#endif // RC_SOLVER_H_
