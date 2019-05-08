#include <array>
#include <iostream>
#include <sstream>
#include <string>

#include "RC.h"

int main() {
    using namespace Rc;
    using namespace std;

    RCCube rcCube;
    string rcCubeStr = " R R O Y O B O G G  Y O B R B G R O O   Y Y O W Y W B B B   Y R G B G R Y G R   W W G Y W G G B B   W Y R W R O W O W";
    stringstream sstream(rcCubeStr);
    sstream >> rcCube;

    cout << rcCube << endl;

    RCCornerPiecePos pos = Rc::findCorner(rcCube, RCCornerPiece(RCColor::ORANGE, RCColor::YELLOW, RCColor::BLUE));

    cout << pos.first << " " << pos.second << " " << pos.third << endl;

    // rcCube.doString("f2 u' r2 l2 b l2 b2 r2 u2 b l' f r2 d f2 l2 d2 b2 d2");

    // rcCube.f2();
    // rcCube.ui();
    // rcCube.r2();
    // rcCube.l2();
    // rcCube.b();
    // rcCube.l2();
    // rcCube.b2();
    // rcCube.r2();
    // rcCube.u2();
    // rcCube.b();
    // rcCube.li();
    // rcCube.f();
    // rcCube.r2();
    // rcCube.d();
    // rcCube.f2();
    // rcCube.l2();
    // rcCube.d2();
    // rcCube.b2();
    // rcCube.d2();

    cout << rcCube << endl;

    return 0;
}