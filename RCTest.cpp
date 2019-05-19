#include <array>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "RC.h"


using namespace std;
using namespace Rc;

static void testMakeTopCross();
static void testMakeTopCross(string str);
static void testMakeTopLayer();
static void testMakeTopLayer(string str);
static void testMakeMiddleLayer();
static void testMakeMiddleLayer(string str);


int main() {
    using namespace Rc;
    using namespace std;

    std::string testStr0 = "O W R R W R R Y R  Y B B R G O B G O  W B Y Y R B G R W  B W G O B G G W R  Y O B O O G G B O  Y Y O Y Y G W W W";
    std::string testStr1 = "R O O G W O G O W  Y R O Y G Y B W R  Y Y B B R W W W B  O G G G B Y O B W  W W G R O G R B R  B R Y B Y R Y O G";
    std::string testStr2 = "B R G G W W W G B  W Y R R G Y R G Y  B O Y B R B G R B  O B O O B R Y W W  Y Y O G O W G O W  R B R W Y O G Y O";


    testMakeMiddleLayer();

    return 0;
}


static void testMakeTopCross() {
    auto cube = RCCube();
    while (true) {
        RCCube::randomize(cube);
        auto cubeCopy = cube;
        makeTopCross(cube);

        cout << cubeCopy << endl;
        cout << cube << endl;

        auto upSurface = cube.getUp();
        if (!(upSurface[0][1] == RCColor::WHITE && upSurface[1][0] == RCColor::WHITE && upSurface[1][2] == RCColor::WHITE && upSurface[2][1] == RCColor::WHITE)) {

            throw logic_error("e");
        }
    }
}

static void testMakeTopCross(string str) {
    stringstream sstream(str);
    RCCube cube;
    sstream >> cube;

    cout << cube << endl;
    makeTopCross(cube);
    cout << cube << endl;
}

static void testMakeTopLayer() {
    auto cube = RCCube();
    while (true) {
        RCCube::randomize(cube);
        auto cubeCopy = cube;
        cout << cubeCopy << endl;

        makeTopCross(cube);
        makeTopLayer(cube);

        cout << cube << endl;

        auto upSurface = cube.getUp();
        if (!(upSurface[0][0] == RCColor::WHITE && upSurface[0][1] == RCColor::WHITE && upSurface[0][2] == RCColor::WHITE && upSurface[1][0] == RCColor::WHITE && upSurface[1][1] == RCColor::WHITE && upSurface[1][1] == RCColor::WHITE &&
              upSurface[2][0] == RCColor::WHITE && upSurface[2][1] == RCColor::WHITE && upSurface[2][2] == RCColor::WHITE)) {
            throw logic_error("e");
        }
        auto frontSurface = cube.getFront();
        if (!(frontSurface[0][0] == RCColor::RED && frontSurface[0][1] == RCColor::RED && frontSurface[0][2] == RCColor::RED)) {
            throw logic_error("e");
        }
        auto backSurface = cube.getBack();
        if (!(backSurface[0][0] == RCColor::ORANGE && backSurface[0][1] == RCColor::ORANGE && backSurface[0][2] == RCColor::ORANGE)) {
            throw logic_error("e");
        }
        auto leftSurface = cube.getLeft();
        if (!(leftSurface[0][0] == RCColor::GREEN && leftSurface[0][1] == RCColor::GREEN && leftSurface[0][2] == RCColor::GREEN)) {
            throw logic_error("e");
        }
        auto rightSurface = cube.getRight();
        if (!(rightSurface[0][0] == RCColor::BLUE && rightSurface[0][1] == RCColor::BLUE && rightSurface[0][2] == RCColor::BLUE)) {
            throw logic_error("e");
        }
    }
}

static void testMakeTopLayer(string str) {
    stringstream sstream(str);
    RCCube cube;
    sstream >> cube;

    cout << cube << endl;
    makeTopCross(cube);
    makeTopLayer(cube);
    cout << cube << endl;
}

static void testMakeMiddleLayer() {
    auto cube = RCCube();
    while (true) {
        RCCube::randomize(cube);
        auto cubeCopy = cube;
        cout << cubeCopy << endl;

        makeTopCross(cube);
        makeTopLayer(cube);
        makeMiddleLayer(cube);

        cout << cube << endl;

        auto frontSurface = cube.getFront();
        if (!(frontSurface[1][0] == RCColor::RED && frontSurface[1][1] == RCColor::RED && frontSurface[1][2] == RCColor::RED)) {
            throw logic_error("e");
        }
        auto backSurface = cube.getBack();
        if (!(backSurface[1][0] == RCColor::ORANGE && backSurface[1][1] == RCColor::ORANGE && backSurface[1][2] == RCColor::ORANGE)) {
            throw logic_error("e");
        }
        auto leftSurface = cube.getLeft();
        if (!(leftSurface[1][0] == RCColor::GREEN && leftSurface[1][1] == RCColor::GREEN && leftSurface[1][2] == RCColor::GREEN)) {
            throw logic_error("e");
        }
        auto rightSurface = cube.getRight();
        if (!(rightSurface[1][0] == RCColor::BLUE && rightSurface[1][1] == RCColor::BLUE && rightSurface[1][2] == RCColor::BLUE)) {
            throw logic_error("e");
        }
    }
}

static void testMakeMiddleLayer(string str) {
    stringstream sstream(str);
    RCCube cube;
    sstream >> cube;

    cout << cube << endl;
    makeTopCross(cube);
    makeTopLayer(cube);
    makeMiddleLayer(cube);
    cout << cube << endl;
}