#include "src/Vector.h"

int main() {
    Vector<4> v({1., 2., 3., 4.});

    std::cout << (v/(1. - v.one_hot(2))).bounds().second << std::endl;

    std::cout << (v | v) << std::endl;
    return 0;
}
