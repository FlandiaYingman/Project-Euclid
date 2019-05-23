#include "RCCubeRecorder.h"

namespace Rc {

RCCube &RCCubeRecorder::l() {
    osstream << "l"
             << " ";
    return RCCube::l();
}
RCCube &RCCubeRecorder::r() {
    osstream << "r"
             << " ";
    return RCCube::r();
}
RCCube &RCCubeRecorder::u() {
    osstream << "u"
             << " ";
    return RCCube::u();
}
RCCube &RCCubeRecorder::d() {
    osstream << "d"
             << " ";
    return RCCube::d();
}
RCCube &RCCubeRecorder::f() {
    osstream << "f"
             << " ";
    return RCCube::f();
}
RCCube &RCCubeRecorder::b() {
    osstream << "b"
             << " ";
    return RCCube::b();
}

RCCube &RCCubeRecorder::li() {
    osstream << "li"
             << " ";
    return RCCube::li();
}
RCCube &RCCubeRecorder::ri() {
    osstream << "ri"
             << " ";
    return RCCube::ri();
}
RCCube &RCCubeRecorder::ui() {
    osstream << "ui"
             << " ";
    return RCCube::ui();
}
RCCube &RCCubeRecorder::di() {
    osstream << "di"
             << " ";
    return RCCube::di();
}
RCCube &RCCubeRecorder::fi() {
    osstream << "fi"
             << " ";
    return RCCube::fi();
}
RCCube &RCCubeRecorder::bi() {
    osstream << "bi"
             << " ";
    return RCCube::bi();
}

RCCube &RCCubeRecorder::l2() {
    osstream << "l2"
             << " ";
    return RCCube::l2();
}
RCCube &RCCubeRecorder::r2() {
    osstream << "r2"
             << " ";
    return RCCube::r2();
}
RCCube &RCCubeRecorder::u2() {
    osstream << "u2"
             << " ";
    return RCCube::u2();
}
RCCube &RCCubeRecorder::d2() {
    osstream << "d2"
             << " ";
    return RCCube::d2();
}
RCCube &RCCubeRecorder::f2() {
    osstream << "f2"
             << " ";
    return RCCube::f2();
}
RCCube &RCCubeRecorder::b2() {
    osstream << "b2"
             << " ";
    return RCCube::b2();
}

RCCube &RCCubeRecorder::xl() {
    osstream << "xl"
             << " ";
    return RCCube::xl();
}
RCCube &RCCubeRecorder::xr() {
    osstream << "xr"
             << " ";
    return RCCube::xr();
}
RCCube &RCCubeRecorder::xu() {
    osstream << "xu"
             << " ";
    return RCCube::xu();
}
RCCube &RCCubeRecorder::xd() {
    osstream << "xd"
             << " ";
    return RCCube::xd();
}
RCCube &RCCubeRecorder::xf() {
    osstream << "xf"
             << " ";
    return RCCube::xf();
}
RCCube &RCCubeRecorder::xb() {
    osstream << "xb"
             << " ";
    return RCCube::xb();
}

RCCube &RCCubeRecorder::cl() {
    osstream << "cl"
             << " ";
    return RCCube::cl();
}
RCCube &RCCubeRecorder::cr() {
    osstream << "cr"
             << " ";
    return RCCube::cr();
}
RCCube &RCCubeRecorder::cu() {
    osstream << "cu"
             << " ";
    return RCCube::cu();
}
RCCube &RCCubeRecorder::cd() {
    osstream << "cd"
             << " ";
    return RCCube::cd();
}
RCCube &RCCubeRecorder::cf() {
    osstream << "cf"
             << " ";
    return RCCube::cf();
}
RCCube &RCCubeRecorder::cb() {
    osstream << "cb"
             << " ";
    return RCCube::cb();
}

RCCube &RCCubeRecorder::doString(const std::string str) {
    return RCCube::doString(str);
}
std::string RCCubeRecorder::getString() const {
    return osstream.str();
}

} // namespace Rc