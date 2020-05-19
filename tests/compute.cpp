#include <experimental/random>
#include "../src/Layer.h"
#include "compute.h"

using std::experimental::randint;


F rand_btree(Address left, Address right) {
    return [left, right](Layer* g, Address seed) {
        auto total = std::get<int>((*g)[seed]);
        if (total > 1) {
            int l = randint(1, total - 1), r = total - l;
            auto [nl, nr] = g->wrap(l, r);
            g->connect(seed, left, nl);
            g->connect(seed, right, nr);
            return CType("Mul");
        } else return CType(randint(1, 10));
    };
}

F btree_avg(Address left, Address right) {
    return [left, right](Layer* g, Address n) {
        auto data = (*g)[n];
        if (data.index() != 1) {
            auto nl = g->get_objects(n, left)[0], nr = g->get_objects(n, right)[0];
            auto ld = (*g)[nl], rd = (*g)[nr];
            return CType((std::get<double>(ld) + std::get<double>(rd))/2.);
        } else return CType((double) (std::get<int>(data)));
    };
}


void hylo_tree_avg() {
    auto expr = new Layer();
    auto[l, r, seed] = expr->wrap("Left", "Right", 10000);

    auto res_addr = expr->hylo(seed, rand_btree(l, r), btree_avg(l, r));

    double res = std::get<double>((*expr)[res_addr]);
    expr->free_tree();
    delete expr;
    if (not (4 < res and res < 7)) throw std::logic_error("expression average should be 5.5");
}
