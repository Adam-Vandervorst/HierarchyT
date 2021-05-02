#include <cassert>
#include <set>
#include "../src/Layer.h"
#include "layer.h"

template <class T, template <class> class C1, template <class> class C2=std::initializer_list>
bool set_equal(C1<T> v1, C2<T> v2) {
    if (v1.size() != v2.size()) return false;
    for (T e1 : v1) if (std::find(v2.begin(), v2.end(), e1) == v2.end()) return false;
    for (T e2 : v2) if (std::find(v1.begin(), v1.end(), e2) == v1.end()) return false;
    return true;
}

void add_stack_layers() {
    auto l = new Layer();
    auto [x, y, p] = l->wrap("x", "y", "p");
    l->connect(x, p, y);

    auto g = l->add_layer("g");
    auto [z] = g->wrap("z");
    g->connect(x, p, z);

    auto h = l->add_layer("h");
    auto [q] = h->wrap("q");
    h->connect(y, q, x);

    if (!set_equal(l->get_objects(x, p), {y}) or
        !set_equal(l->get_objects(y, q), {}))
        throw std::logic_error("nodes scoped incorrectly");

    if (!set_equal(g->get_objects(x, p), {y, z}) or
        !set_equal(h->get_objects(y, q), {x}))
        throw std::logic_error("nodes added incorrectly");

    l->free_tree();
    delete l;
}

void remove_stack_layers() {
    auto l = new Layer();
    auto [x, y, z, p, q] = l->wrap("x", "y", "z", "p", "q");
    l->connect(x, p, y);
    l->connect(x, p, z);
    l->connect(y, q, x);

    auto g = l->add_layer("g");
    g->remove_node(z);

    auto h = l->add_layer("h");
    h->remove_node(q);

    if (!set_equal(l->get_objects(x, p), {y, z}) or
        !set_equal(l->get_objects(y, q), {x}))
        throw std::logic_error("nodes scoped incorrectly");

    if (!set_equal(g->get_objects(x, p), {y}) or
        !set_equal(h->get_objects(y, q), {}))
        throw std::logic_error("nodes removed incorrectly");

    l->free_tree();
    delete l;
}
