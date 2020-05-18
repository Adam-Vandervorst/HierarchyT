#ifndef HIERARCHYT_MORPH_H
#define HIERARCHYT_MORPH_H

#include "../src/Layer.h"


namespace test_graphs {
    auto scene_rewrite() {
        auto universe = new Layer();
        auto match = new Layer(universe, "RewriteMatch");
        auto replace = new Layer(universe, "RewriteReplace");
        auto scene = new Layer(universe, "Scene");
        auto r1 = new Layer(scene, "Rewrite1");
        auto r2 = new Layer(scene, "Rewrite2");
        auto [box, pyr, bal] = universe->wrap("Box", "Pyramid", "Ball");
        auto [l, r, t, b, o] = universe->wrap("Left", "Right", "Top", "Bottom", "Opposite");
        auto [x, y] = match->wrap("x", "y");
        universe->connect(l, o, r);
        universe->connect(r, o, l);
        universe->connect(t, o, b);
        universe->connect(b, o, t);
        match->connect(x, l, y);
        replace->connect(y, r, x);
        scene->connect(box, l, pyr);
        scene->connect(box, t, bal);
        r1->connect(pyr, r, box);
        r2->connect(bal, b, box);
        return std::make_tuple(universe, scene, match, replace, r1, r2);
    }

    auto patch_rewrite() {
        auto G = new Layer();
        auto [x, y, z] = G->wrap("x", "y", "z");
        auto [a, b, c, d, e] = G->wrap("a", "b", "c", "d", "e");
        G->connect(x, b, y); G->connect(x, c, y); G->connect(y, a, y); G->connect(y, d, z); G->connect(z, e, z);
        auto to_match = new Layer(G, "RewriteMatch");
        auto [m] = to_match->wrap("m");
        to_match->connect(m, a, m);
        auto replace = new Layer(to_match, "RewriteReplace");
        auto [m1, m2] = replace->wrap("m1", "m2");
        replace->connect(m1, b, m1);
        return std::make_tuple(G, to_match, replace);
    }
}

#endif //HIERARCHYT_MORPH_H
