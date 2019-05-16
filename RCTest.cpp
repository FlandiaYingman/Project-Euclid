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


int main() {
    using namespace Rc;
    using namespace std;

    testMakeTopLayer();

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