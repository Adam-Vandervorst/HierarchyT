//
// Created by adamv on 5/7/20.
//

#include "Benchmarks.h"
#include <chrono>
#include <experimental/random>

using namespace std::chrono;
using std::experimental::randint;


F rand_btree(Address left, Address right) {
    return [left, right](Layer* g, Address seed) {
        auto total = std::get<int>((*g)[seed]);
        if (total > 1) {
            int l = randint(1, total - 1), r = total - l;
            auto[nl, nr] = g->wrap(l, r);
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

void benchmarks::hylo_performance() {
    auto expr = new Layer();
    auto[l, r, seed] = expr->wrap("Left", "Right", 100000);

    auto start = high_resolution_clock::now();
    auto res = expr->hylo(seed, rand_btree(l, r), btree_avg(l, r));
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<nanoseconds>(stop - start);
    std::cout << (*expr)[res] << ", " << duration.count() << std::endl;
    expr->free_tree();
    delete expr;
}

