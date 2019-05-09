#include "RCCube.h"
#include "RCSurface.h"

#include <chrono>
#include <random>
#include <sstream>
#include <string>

namespace Rc {

RCCube::RCCube() {
    this->left = RCSurface(RCColor::GREEN);
    this->right = RCSurface(RCColor::BLUE);
    this->up = RCSurface(RCColor::WHITE);
    this->down = RCSurface(RCColor::YELLOW);
    this->front = RCSurface(RCColor::RED);
    this->back = RCSurface(RCColor::ORANGE);
}
RCCube::RCCube(RCSurface left, RCSurface right, RCSurface up, RCSurface down, RCSurface front, RCSurface back) {
    this->left = left;
    this->right = right;
    this->up = up;
    this->down = down;
    this->front = front;
    this->back = back;
}

void RCCube::randomize(RCCube &cube) {
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0, 5);

    for (size_t i = 0; i < 64; i++) {
        int r = distribution(generator);
        switch (r) {
        case 0:
            cube.l();
            break;
        case 1:
            cube.r();
            break;
        case 2:
            cube.u();
            break;
        case 3:
            cube.d();
            break;
        case 4:
            cube.f();
            break;
        case 5:
            cube.b();
            break;
        }
    }
}

RCSurface RCCube::getLeft() const {
    return this->left;
}
RCSurface RCCube::getRight() const {
    return this->right;
}
RCSurface RCCube::getUp() const {
    return this->up;
}
RCSurface RCCube::getDown() const {
    return this->down;
}
RCSurface RCCube::getFront() const {
    return this->front;
}
RCSurface RCCube::getBack() const {
    return this->back;
}

RCSurface RCCube::get(RCFacing facing) const {
    switch (facing) {
    case RCFacing::LEFT:
        return getLeft();
    case RCFacing::RIGHT:
        return getRight();
    case RCFacing::UP:
        return getUp();
    case RCFacing::DOWN:
        return getDown();
    case RCFacing::FRONT:
        return getFront();
    case RCFacing::BACK:
        return getBack();
    default:
        return RCSurface(RCColor::NUL);
    }
}

RCCube &RCCube::l() {
    RCCube temp = *this;
    front = front.withLeftColumn(temp.up.getLeftColumn());
    down = down.withLeftColumn(temp.front.getLeftColumn());
    back = back.withRightColumn(temp.down.getLeftColumn().reverse());
    up = up.withLeftColumn(temp.back.getRightColumn().reverse());
    left = left.rotateClockwise();
    return *this;
}
RCCube &RCCube::r() {
    RCCube temp = *this;
    front = front.withRightColumn(temp.down.getRightColumn());
    up = up.withRightColumn(temp.front.getRightColumn());
    back = back.withLeftColumn(temp.up.getRightColumn().reverse());
    down = down.withRightColumn(temp.back.getLeftColumn().reverse());
    right = right.rotateClockwise();
    return *this;
}
RCCube &RCCube::u() {
    RCCube temp = *this;
    front = front.withTopRow(temp.right.getTopRow());
    left = left.withTopRow(temp.front.getTopRow());
    back = back.withTopRow(temp.left.getTopRow());
    right = right.withTopRow(temp.back.getTopRow());
    up = up.rotateClockwise();
    return *this;
}
RCCube &RCCube::d() {
    RCCube temp = *this;
    front = front.withBottomRow(temp.left.getBottomRow());
    right = right.withBottomRow(temp.front.getBottomRow());
    back = back.withBottomRow(temp.right.getBottomRow());
    left = left.withBottomRow(temp.back.getBottomRow());
    down = down.rotateClockwise();
    return *this;
}
RCCube &RCCube::f() {
    RCCube temp = *this;
    up = up.withBottomRow(temp.left.getRightColumn().reverse());
    right = right.withLeftColumn(temp.up.getBottomRow());
    down = down.withTopRow(temp.right.getLeftColumn().reverse());
    left = left.withRightColumn(temp.down.getTopRow());
    front = front.rotateClockwise();
    return *this;
}
RCCube &RCCube::b() {
    RCCube temp = *this;
    up = up.withTopRow(temp.right.getRightColumn());
    left = left.withLeftColumn(temp.up.getTopRow().reverse());
    down = down.withBottomRow(temp.left.getLeftColumn());
    right = right.withRightColumn(temp.down.getBottomRow().reverse());
    back = back.rotateClockwise();
    return *this;
}

RCCube &RCCube::ri() {
    r();
    r();
    r();
    return *this;
}
RCCube &RCCube::li() {
    l();
    l();
    l();
    return *this;
}
RCCube &RCCube::ui() {
    u();
    u();
    u();
    return *this;
}
RCCube &RCCube::di() {
    d();
    d();
    d();
    return *this;
}
RCCube &RCCube::fi() {
    f();
    f();
    f();
    return *this;
}
RCCube &RCCube::bi() {
    b();
    b();
    b();
    return *this;
}

RCCube &RCCube::r2() {
    r();
    r();
    return *this;
}
RCCube &RCCube::l2() {
    l();
    l();
    return *this;
}
RCCube &RCCube::u2() {
    u();
    u();
    return *this;
}
RCCube &RCCube::d2() {
    d();
    d();
    return *this;
}
RCCube &RCCube::f2() {
    f();
    f();
    return *this;
}
RCCube &RCCube::b2() {
    b();
    b();
    return *this;
}

RCCube &RCCube::xl() {
    RCCube temp = *this;
    left = left.rotateClockwise();
    right = right.rotateAntiClockwise();
    front = temp.getUp();
    down = temp.getFront();
    back = temp.getDown().reverseX().reverseY();
    up = temp.getBack().reverseX().reverseY();
    return *this;
}
RCCube &RCCube::xr() {
    return xl().xl().xl();
}
RCCube &RCCube::xu() {
    RCCube temp = *this;
    up = up.rotateClockwise();
    down = down.rotateAntiClockwise();
    front = temp.getRight();
    left = temp.getFront();
    back = temp.getLeft();
    right = temp.getBack();
    return *this;
}
RCCube &RCCube::xd() {
    return xu().xu().xu();
}
RCCube &RCCube::xf() {
    RCCube temp = *this;
    front = front.rotateClockwise();
    back = back.rotateAntiClockwise();
    up = temp.getLeft().rotateClockwise();
    right = temp.getUp().rotateClockwise();
    down = temp.getRight().rotateClockwise();
    left = temp.getDown().rotateClockwise();
    return *this;
}
RCCube &RCCube::xb() {
    return xf().xf().xf();
}

RCCube &RCCube::cl() {
    return li().r().xl();
}
RCCube &RCCube::cr() {
    return ri().l().xr();
}
RCCube &RCCube::cu() {
    return ui().d().xu();
}
RCCube &RCCube::cd() {
    return di().u().xd();
}
RCCube &RCCube::cf() {
    return fi().b().xf();
}
RCCube &RCCube::cb() {
    return bi().f().xb();
}

RCCube &RCCube::doString(std::string str) {
    std::stringstream sstream(str);
    while (!sstream.eof()) {
        std::string unit;
        sstream >> unit;
        if (unit == "l") {
            l();
        } else if (unit == "r") {
            r();
        } else if (unit == "u") {
            u();
        } else if (unit == "d") {
            d();
        } else if (unit == "f") {
            f();
        } else if (unit == "b") {
            b();
        } else if (unit == "l'") {
            li();
        } else if (unit == "r'") {
            ri();
        } else if (unit == "u'") {
            ui();
        } else if (unit == "d'") {
            di();
        } else if (unit == "f'") {
            fi();
        } else if (unit == "b'") {
            bi();
        } else if (unit == "l2") {
            l2();
        } else if (unit == "r2") {
            r2();
        } else if (unit == "u2") {
            u2();
        } else if (unit == "d2") {
            d2();
        } else if (unit == "f2") {
            f2();
        } else if (unit == "b2") {
            b2();
        } else {
            std::cerr << "Unknown unit: \"" << unit << "\"" << std::endl;
        }
    }
    return *this;
}


std::ostream &operator<<(std::ostream &strm, const RCFacing &facing) {
    std::string str;
    switch (facing) {
    case RCFacing::LEFT:
        str = "LEFT";
        break;
    case RCFacing::RIGHT:
        str = "RIGHT";
        break;
    case RCFacing::UP:
        str = "UP";
        break;
    case RCFacing::DOWN:
        str = "DOWN";
        break;
    case RCFacing::FRONT:
        str = "FRONT";
        break;
    case RCFacing::BACK:
        str = "BACK";
        break;
    case RCFacing::NUL:
        str = "NUL";
        break;
    }
    strm << str;
    return strm;
}

std::ostream &operator<<(std::ostream &strm, RCCube cube) {
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3 * 4; j++) {
            if (j >= 3 && j <= 5) {
                strm << cube.getUp()[i][j % 3] << " ";
            } else {
                strm << " "
                     << " ";
            }
        }
        strm << "\n";
    }
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3 * 4; j++) {
            if (j <= 2) {
                strm << cube.getLeft()[i][j % 3] << " ";
            } else if (j <= 5) {
                strm << cube.getFront()[i][j % 3] << " ";
            } else if (j <= 8) {
                strm << cube.getRight()[i][j % 3] << " ";
            } else {
                strm << cube.getBack()[i][j % 3] << " ";
            }
        }
        strm << "\n";
    }
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3 * 4; j++) {
            if (j >= 3 && j <= 5) {
                strm << cube.getDown()[i][j % 3] << " ";
            } else {
                strm << " "
                     << " ";
            }
        }
        strm << "\n";
    }
    return strm;
}
std::istream &operator>>(std::istream &strm, RCCube &cube) {
    RCSurface surfaces[6];
    for (size_t i = 0; i < 6; i++) {
        strm >> surfaces[i];
    }
    cube = RCCube(surfaces[1], surfaces[3], surfaces[0], surfaces[5], surfaces[2], surfaces[4]);
    return strm;
}

} // namespace Rc
