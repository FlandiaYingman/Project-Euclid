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


int main() {
    using namespace Rc;
    using namespace std;

    testMakeTopCross();

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