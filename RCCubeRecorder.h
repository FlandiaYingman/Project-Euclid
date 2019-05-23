#if !defined(RC_CUBE_RECORDER_H_)
#define RC_CUBE_RECORDER_H_

#include "RCCube.h"
#include "RCSurface.h"

#include <iostream>
#include <sstream>
#include <string>

namespace Rc {

class RCCubeRecorder : public RCCube {
  private:
    std::ostringstream osstream;

  public:
    RCCubeRecorder() : RCCube() {
    }
    RCCubeRecorder(const RCSurface left, const RCSurface right, const RCSurface up, const RCSurface down, const RCSurface front, const RCSurface back) : RCCube(left, right, up, down, front, back) {
    }

    RCCube &l();
    RCCube &r();
    RCCube &u();
    RCCube &d();
    RCCube &f();
    RCCube &b();

    RCCube &li();
    RCCube &ri();
    RCCube &ui();
    RCCube &di();
    RCCube &fi();
    RCCube &bi();

    RCCube &l2();
    RCCube &r2();
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

    RCCube &doString(const std::string str);
    std::string getString() const;
};

} // namespace Rc


#endif // RC_CUBE_RECORDER_H_
