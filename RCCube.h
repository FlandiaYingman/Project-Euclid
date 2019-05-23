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
    RCCube(const RCSurface left, const RCSurface right, const RCSurface up, const RCSurface down, const RCSurface front, const RCSurface back);

    static void randomize(RCCube &cube);

    RCSurface getLeft() const;
    RCSurface getRight() const;
    RCSurface getUp() const;
    RCSurface getDown() const;
    RCSurface getFront() const;
    RCSurface getBack() const;

    RCSurface get(const RCFacing facing) const;

    virtual RCCube &l();
    virtual RCCube &r();
    virtual RCCube &u();
    virtual RCCube &d();
    virtual RCCube &f();
    virtual RCCube &b();

    virtual RCCube &li();
    virtual RCCube &ri();
    virtual RCCube &ui();
    virtual RCCube &di();
    virtual RCCube &fi();
    virtual RCCube &bi();

    virtual RCCube &l2();
    virtual RCCube &r2();
    virtual RCCube &u2();
    virtual RCCube &d2();
    virtual RCCube &f2();
    virtual RCCube &b2();

    virtual RCCube &xl();
    virtual RCCube &xr();
    virtual RCCube &xu();
    virtual RCCube &xd();
    virtual RCCube &xf();
    virtual RCCube &xb();

    virtual RCCube &cl();
    virtual RCCube &cr();
    virtual RCCube &cu();
    virtual RCCube &cd();
    virtual RCCube &cf();
    virtual RCCube &cb();

    virtual RCCube &doString(const std::string str);
};

std::ostream &operator<<(std::ostream &strm, const RCFacing facing);

std::ostream &operator<<(std::ostream &strm, const RCCube cube);
std::istream &operator>>(std::istream &strm, RCCube &cube);

} // namespace Rc

#endif // RC_CUBE_H_
