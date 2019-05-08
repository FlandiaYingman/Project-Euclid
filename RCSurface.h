#if !defined(RC_SURFACE_H_)
#define RC_SURFACE_H_

#include <array>
#include <initializer_list>
#include <iostream>

#include "RCColor.h"

namespace Rc {

enum class RCDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UPPER_LEFT,
    UPPER_RIGHT,
    LOWER_LEFT,
    LOWER_RIGHT,
    NUL,
};

class RCLine {
  private:
    std::array<RCColor, 3> line = { RCColor::NUL, RCColor::NUL, RCColor::NUL };

  public:
    RCLine();
    RCLine(std::initializer_list<RCColor> initList);

    RCColor operator[](size_t i);

    RCLine reverse();
};

class RCSurface {
  private:
    std::array<RCLine, 3> surface = { RCLine(), RCLine(), RCLine() };

  public:
    RCSurface();
    RCSurface(RCColor color);
    RCSurface(std::initializer_list<RCLine> initList);

    RCLine operator[](size_t i);

    RCColor get(RCDirection direction);

    RCSurface rotateClockwise();
    RCSurface rotateAntiClockwise();

    RCLine getTopRow();
    RCLine getMiddleRow();
    RCLine getBottomRow();
    RCLine getLeftColumn();
    RCLine getMiddleColumn();
    RCLine getRightColumn();

    RCSurface withTopRow(RCLine line);
    RCSurface withMiddleRow(RCLine line);
    RCSurface withBottomRow(RCLine line);
    RCSurface withLeftColumn(RCLine line);
    RCSurface withMiddleColumn(RCLine line);
    RCSurface withRightColumn(RCLine line);
};

std::ostream &operator<<(std::ostream &strm, RCSurface surface);
std::istream &operator>>(std::istream &strm, RCSurface &surface);

} // namespace Rc

#endif // RC_SURFACE_H_
