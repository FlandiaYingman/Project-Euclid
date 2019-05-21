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
    RCLine(const std::initializer_list<RCColor> initList);

    RCColor operator[](const size_t i) const;

    RCLine reverse() const;
};

class RCSurface {
  private:
    std::array<RCLine, 3> surface = { RCLine(), RCLine(), RCLine() };

  public:
    RCSurface();
    RCSurface(const RCColor color);
    RCSurface(const std::initializer_list<RCLine> initList);

    RCLine operator[](const size_t i) const;

    RCColor get(const RCDirection direction) const;

    RCSurface rotateClockwise() const;
    RCSurface rotateAntiClockwise() const;

    RCSurface reverseX() const;
    RCSurface reverseY() const;

    RCLine getTopRow() const;
    RCLine getMiddleRow() const;
    RCLine getBottomRow() const;
    RCLine getLeftColumn() const;
    RCLine getMiddleColumn() const;
    RCLine getRightColumn() const;

    RCSurface withTopRow(const RCLine line) const;
    RCSurface withMiddleRow(const RCLine line) const;
    RCSurface withBottomRow(const RCLine line) const;
    RCSurface withLeftColumn(const RCLine line) const;
    RCSurface withMiddleColumn(const RCLine line) const;
    RCSurface withRightColumn(const RCLine line) const;
};

std::ostream &operator<<(std::ostream &strm, const RCSurface surface);
std::istream &operator>>(std::istream &strm, RCSurface &surface);

} // namespace Rc

#endif // RC_SURFACE_H_
