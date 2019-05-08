#if !defined(RC_COLOR_H_)
#define RC_COLOR_H_

#include <iostream>

namespace Rc {

enum class RCColor : char {
    WHITE = 'W',
    YELLOW = 'Y',
    RED = 'R',
    ORANGE = 'O',
    BLUE = 'B',
    GREEN = 'G',
    NUL = '-',
};

std::ostream &operator<<(std::ostream &strm, RCColor color);
std::istream &operator>>(std::istream &strm, RCColor &color);

} // namespace Rc

#endif // RC_COLOR_H_