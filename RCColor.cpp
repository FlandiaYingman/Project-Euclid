#include "RCColor.h"

namespace Rc {

std::ostream &operator<<(std::ostream &strm, RCColor color) {
    return strm << (char)color;
}
std::istream &operator>>(std::istream &strm, RCColor &color) {
    char ch;
    strm >> ch;
    color = (RCColor)ch;
    return strm;
}

} // namespace Rc
