#if !defined(RC_CUBE_H_)
#define RC_CUBE_H_

#include "RCSurface.h"

#include <iostream>
#include <string>

namespace Rc {

enum class RCFacing {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    FRONT,
    BACK,
    NUL,
};

class RCCube {
  private:
    RCSurface left, right, up, down, front, back;

  public:
    RCCube();
    RCCube(RCSurface left, RCSurface right, RCSurface up, RCSurface down, RCSurface front, RCSurface back);

    RCSurface getLeft() const;
    RCSurface getRight() const;
    RCSurface getUp() const;
    RCSurface getDown() const;
    RCSurface getFront() const;
    RCSurface getBack() const;

    RCSurface get(RCFacing facing) const;

    RCCube &l();
    RCCube &r();
    RCCube &u();
    RCCube &d();
    RCCube &f();
    RCCube &b();

    RCCube &ri();
    RCCube &li();
    RCCube &ui();
    RCCube &di();
    RCCube &fi();
    RCCube &bi();

    RCCube &r2();
    RCCube &l2();
    RCCube &u2();
    RCCube &d2();
    RCCube &f2();
    RCCube &b2();

    RCCube &xl();
    RCCube &xr();
    RCCube &xu();
    RCCube &xd();
    RCCube &xf();
    RCCube &xb();

    RCCube &cl();
    RCCube &cr();
    RCCube &cu();
    RCCube &cd();
    RCCube &cf();
    RCCube &cb();

    RCCube &doString(std::string str);
};

std::ostream &operator<<(std::ostream &strm, const RCFacing &facing);

std::ostream &operator<<(std::ostream &strm, RCCube cube);
std::istream &operator>>(std::istream &strm, RCCube &cube);

} // namespace Rc

#endif // RC_CUBE_H_
