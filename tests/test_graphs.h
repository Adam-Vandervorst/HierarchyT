#ifndef HIERARCHYT_TEST_GRAPHS_H
#define HIERARCHYT_TEST_GRAPHS_H

#include "../src/Layer.h"


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
        auto l0 = l->add_layer("second layer"); l->add_layer(); l->add_layer();
        auto l00 = l0->add_layer("final layer");
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
}


#endif //HIERARCHYT_TEST_GRAPHS_H
