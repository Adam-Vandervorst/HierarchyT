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
        if (std::holds_alternative<int>(data)) return CType((double)(std::get<int>(data)));
        else {
            auto nl = g->get_objects(n, left)[0], nr = g->get_objects(n, right)[0];
            auto ld = (*g)[nl], rd = (*g)[nr];
            return CType((std::get<double>(ld) + std::get<double>(rd))/2.);
        }
    };
}


void hylo_tree_avg() {
    auto expr = new Layer();
    auto [l, r, seed] = expr->wrap("Left", "Right", 10000);

    auto res_addr = expr->hylo(seed, rand_btree(l, r), btree_avg(l, r));

    double res = std::get<double>((*expr)[res_addr]);
    expr->free_tree();
    delete expr;
    if (not (4 < res and res < 7)) throw std::logic_error("expression average should be 5.5");
}

void random_graph() {
    int nodes = 1000, properties = 100, edges = 20000;
    auto l = new Layer();

    auto ns = l->add_nodes(std::vector<int>(nodes));
    auto ps = l->add_nodes(std::vector<int>(properties));

    while (l->conn.size() != edges)
        l->connect(ns[randint(0, nodes - 1)], ps[randint(0, properties - 1)], ns[randint(0, nodes - 1)]);

    delete l;
}

F fact(int to_gen, Address prop) {
    return [to_gen, prop](Layer* g, Address seed) {
        auto i = std::get<int>((*g)[seed]);
        if (i < to_gen)
            for (int k = i; k >= 0; --k)
                g->connect(seed, prop, g->add_node(i + 1));
        return CType(i);
    };
}

constexpr auto factorial(unsigned int n) {
    if (n == 1 or n == 0) return 1u;
    return n*factorial(n - 1);
}

void factorial_tree_count() {
    int n = 8;
    auto l = new Layer();

    auto seed = l->add_node(1);
    auto prop = l->add_node("");
    l->hylo(seed, fact(n, prop));
    if (factorial(n) != l->find_all([l, n](Address a){return (*l)[a] == CType(n);}).size())
        throw std::logic_error("factorial tree incorrect");
    delete l;
}
