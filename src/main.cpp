#include <iostream>
#include <thread>
#include "Layer.h"
#include "Benchmarks.h"

namespace test_graphs {
    auto flat() {
        auto l = Layer();
        auto [a, b, c, p] = l.wrap("a", "b", "c", "p");
        l.connect(a, p, b);
        l.connect(a, p, c);
        l.connect(c, p, a);
        return std::make_tuple(l);
    }

    auto layered() {
        auto l = new Layer();
        auto l0 = l->add_layer("nested"); l->add_layer(); l->add_layer();
        auto l00 = l0->add_layer("final");
        return std::make_tuple(l, l0, l00);
    }

    auto edges() {
        auto l0 = new Layer();
        auto l1 = new Layer(l0, "l1");
        auto l2 = new Layer(l1, "l2");
        auto l3 = new Layer(l2, "l3");
        auto [a0, b0, p0] = l0->wrap("A0", "B0", "P0");
        auto [a1, b1, p1] = l1->wrap("A1", "B1", "P1");
        auto [a2, b2, p2] = l2->wrap("A2", "B2", "P2");
        auto [a3, b3, p3] = l3->wrap("A3", "B3", "P3");
        l0->connect(a0, p0, b0);
        l1->connect(b0, p0, a0);
        l2->connect(a0, p1, b0);
        l3->connect(a0, p3, b0);
        return std::make_tuple(l0, l1, l2, l3);
    }

    auto scene_rewrite() {
        auto universe = new Layer();
        auto scene = new Layer(universe, "Scene");
        auto r1 = new Layer(scene, "Rewrite1");
        auto r2 = new Layer(scene, "Rewrite2");
        auto [box, pyr, bal] = universe->wrap("Box", "Pyramid", "Ball");
        auto [l, r, t, b, o] = universe->wrap("Left", "Right", "Top", "Bottom", "Opposite");
        universe->connect(l, o, r);
        universe->connect(r, o, l);
        universe->connect(t, o, b);
        universe->connect(b, o, t);
        scene->connect(box, l, pyr);
        scene->connect(box, t, bal);
        r1->connect(pyr, r, box);
        r2->connect(bal, b, box);
        return std::make_tuple(universe, scene, r1, r2);
    }
}


int main() {
    auto [U, scene, r1, r2] = test_graphs::scene_rewrite();
    *scene <<= r1;
    std::cout << "digraph G {" << std::endl;
    r2->draw();
    std::cout << "}" << std::endl;
    return 0;
}
