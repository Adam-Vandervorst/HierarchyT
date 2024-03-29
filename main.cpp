#include "PicoVector/vectorND.h"

#define uop(f) [](auto a){return f(a);}
#define bop(f) [](auto a, auto b){return f(a, b);}

using namespace std;


int main() {
    Vector<3> v({1, 2, 3}), w({-1, 0, 1});
    // √|v| - w²
    cout << v.map(uop(abs)).map(uop(sqrt)) - w*w << endl;
    // 𝟙 + max(v/||w||, w/||v||)
    cout << 1. + (v/w.norm()).partial(bop(max))(w/v.norm()) << endl;
    // maxⱼ(minᵢ((3*w/||w|| + e₃)ᵢ, 3)ⱼ, -3)
    cout << (3.*w.normalized() + Vector<3>::one_hot(2)).clip({-3., 3.}) << endl;
    // 𝟙/(v + w) < inf
    cout << finite((1./(v + w)).bounds().second) << endl;
    // v·w = ||v|| ||w|| cos(v∠w)
    cout << ((v | w) == v.norm()*w.norm()*cos(v ^ w)) << endl;
    return 0;
}
