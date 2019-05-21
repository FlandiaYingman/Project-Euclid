#include "RCSurface.h"

#include <array>
#include <initializer_list>
#include <iostream>

namespace Rc {

RCLine::RCLine() {
}
RCLine::RCLine(const std::initializer_list<RCColor> initList) {
    int i = 0;
    for (auto color : initList) {
        line[i++] = color;
    }
}

RCColor RCLine::operator[](const size_t i) const {
    return line[i];
}

RCLine RCLine::reverse() const {
    return { line[2], line[1], line[0] };
}


RCSurface::RCSurface() {
}
RCSurface::RCSurface(const RCColor color) {
    RCLine line = { color, color, color };
    surface = { line, line, line };
}
RCSurface::RCSurface(const std::initializer_list<RCLine> initList) {
    int i = 0;
    for (auto line : initList) {
        surface[i++] = line;
    }
}

RCLine RCSurface::operator[](const size_t i) const {
    return surface[i];
}

RCColor RCSurface::get(RCDirection direction) const {
    switch (direction) {
    case RCDirection::UP:
        return (*this)[0][1];
    case RCDirection::DOWN:
        return (*this)[2][1];
    case RCDirection::LEFT:
        return (*this)[1][0];
    case RCDirection::RIGHT:
        return (*this)[1][2];
    case RCDirection::UPPER_LEFT:
        return (*this)[0][0];
    case RCDirection::UPPER_RIGHT:
        return (*this)[0][2];
    case RCDirection::LOWER_LEFT:
        return (*this)[2][0];
    case RCDirection::LOWER_RIGHT:
        return (*this)[2][2];
    default:
        return RCColor::NUL;
    }
}

RCSurface RCSurface::rotateClockwise() const {
    return RCSurface({ { surface[2][0], surface[1][0], surface[0][0] }, { surface[2][1], surface[1][1], surface[0][1] }, { surface[2][2], surface[1][2], surface[0][2] } });
}
RCSurface RCSurface::rotateAntiClockwise() const {
    return rotateClockwise().rotateClockwise().rotateClockwise();
}

RCSurface RCSurface::reverseX() const {
    return RCSurface({ { surface[0][2], surface[0][1], surface[0][0] }, { surface[1][2], surface[1][1], surface[1][0] }, { surface[2][2], surface[2][1], surface[2][0] } });
}
RCSurface RCSurface::reverseY() const {
    return RCSurface({ { surface[2][0], surface[2][1], surface[2][2] }, { surface[1][0], surface[1][1], surface[1][2] }, { surface[0][0], surface[0][1], surface[0][2] } });
}

RCLine RCSurface::getTopRow() const {
    return { surface[0][0], surface[0][1], surface[0][2] };
}
RCLine RCSurface::getMiddleRow() const {
    return { surface[1][0], surface[1][1], surface[1][2] };
}
RCLine RCSurface::getBottomRow() const {
    return { surface[2][0], surface[2][1], surface[2][2] };
}
RCLine RCSurface::getLeftColumn() const {
    return { surface[0][0], surface[1][0], surface[2][0] };
}
RCLine RCSurface::getMiddleColumn() const {
    return { surface[0][1], surface[1][1], surface[2][1] };
}
RCLine RCSurface::getRightColumn() const {
    return { surface[0][2], surface[1][2], surface[2][2] };
}

RCSurface RCSurface::withTopRow(const RCLine line) const {
    return { { line[0], line[1], line[2] }, { surface[1][0], surface[1][1], surface[1][2] }, { surface[2][0], surface[2][1], surface[2][2] } };
}
RCSurface RCSurface::withMiddleRow(const RCLine line) const {
    return { { surface[0][0], surface[0][1], surface[0][2] }, { line[0], line[1], line[2] }, { surface[2][0], surface[2][1], surface[2][2] } };
}
RCSurface RCSurface::withBottomRow(const RCLine line) const {
    return { { surface[0][0], surface[0][1], surface[0][2] }, { surface[1][0], surface[1][1], surface[1][2] }, { line[0], line[1], line[2] } };
}
RCSurface RCSurface::withLeftColumn(const RCLine line) const {
    return { { line[0], surface[0][1], surface[0][2] }, { line[1], surface[1][1], surface[1][2] }, { line[2], surface[2][1], surface[2][2] } };
}
RCSurface RCSurface::withMiddleColumn(const RCLine line) const {
    return { { surface[0][0], line[0], surface[0][2] }, { surface[1][0], line[1], surface[1][2] }, { surface[2][0], line[2], surface[2][2] } };
}
RCSurface RCSurface::withRightColumn(const RCLine line) const {
    return { { surface[0][0], surface[0][1], line[0] }, { surface[1][0], surface[1][1], line[1] }, { surface[2][0], surface[2][1], line[2] } };
}

std::ostream &operator<<(std::ostream &strm, const RCSurface surface) {
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            strm << surface[i][j] << " ";
        }
        strm << "\n";
    }
    return strm;
}
std::istream &operator>>(std::istream &strm, RCSurface &surface) {
    RCColor colors[9];
    for (size_t i = 0; i < 9; i++) {
        strm >> colors[i];
    }
    surface = {
        { colors[0], colors[1], colors[2] },
        { colors[3], colors[4], colors[5] },
        { colors[6], colors[7], colors[8] },
    };
    return strm;
}

}; // namespace Rc