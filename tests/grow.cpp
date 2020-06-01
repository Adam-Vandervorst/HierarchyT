#include "../src/Layer.h"
#include "grow.h"

void add_single_node() {
    auto l = new Layer();
    auto a = l->add_node(1);
    if (a != Address(0, 0)) throw std::logic_error("wrong address in empty layer");
    delete l;
}

void add_nodes_wrap() {
    auto l = new Layer();
    auto a = l->add_node(1);
    auto b = l->add_node(2);
    auto [x, y] = l->wrap("x", "y");
    if (a == b) throw std::logic_error("nodes share address");
    if (b != Address(1, 0)) throw std::logic_error("second node has wrong address");
    delete l;
}

void get_node_data() {
    auto l = new Layer();
    auto [a, b] = l->wrap(1, "a");
    if (std::get<int>((*l)[a]) != 1) throw std::logic_error("node stores wrong data");
    if (std::get<std::string>((*l)[b]) != "a") throw std::logic_error("node stores wrong data");
    delete l;
}

void find_node_with_data() {
    auto l = new Layer();
    auto [a, b] = l->wrap(1, "a");
    if (!l->find(CType(1)).has_value()) throw std::logic_error("node data not found");
    if (l->find(CType("a")).value() != b) throw std::logic_error("wrong address found");
    delete l;
}

void add_many_nodes() {
    auto l = new Layer();
    //l->add_nodes(std::vector<int>(10000)); ~40% speedup
    for (int i = 0; i < 10000; ++i) l->add_node(i);
    delete l;
}

void add_single_edge() {
    auto l = new Layer();
    auto [a, b, p] = l->wrap("a", "b", "p");
    auto e = l->connect(a, p, b);
    if (e->first != a) throw std::logic_error("wrong edge source");
    if (e->second.first != p) throw std::logic_error("wrong edge property");
    if (e->second.second != b) throw std::logic_error("wrong edge target");
    delete l;
}

void add_edges() {
    auto l = new Layer();
    auto [a, b, c, p1, p2] = l->wrap("a", "b", "c", "p1", "p2");
    auto e1 = l->connect(a, p1, b);
    auto e2 = l->connect(a, p2, b);
    auto e3 = l->connect(a, p1, c);
    auto e2_ = l->connect(a, p2, b);
    if (e2 != e2_) throw std::logic_error("edge is not unique");
    if (e1->second.first != e3->second.first) throw std::logic_error("edges do not share a property");
    delete l;
}

template<class C1>
bool same_nodes(C1 c1, std::initializer_list<Address> c2) {
    return std::is_permutation(c1.begin(), c1.end(), c2.begin());
}

template<class C>
bool contains(C c, Address n) {
    return std::find(c.begin(), c.end(), n) != c.end();
}

void get_node_targets() {
    auto l = new Layer();
    auto [a, b, c, p1, p2] = l->wrap("a", "b", "c", "p1", "p2");
    auto e1 = l->connect(a, p1, b);
    auto e2 = l->connect(a, p2, b);
    auto e3 = l->connect(a, p1, c);
    auto e1_ = l->connect(a, p1, c);
    if (!same_nodes(l->get_objects(a, p1), {b, c})) throw std::logic_error("wrong edge targets");
    if (!contains(l->get_objects(a, p1), b) or !contains(l->get_objects(a, p2), b)) throw std::logic_error("wrong edge target");
    delete l;
}

void add_many_edges() {
    auto l = new Layer();
    std::vector<int> xi(10), pi(100), yi(10);
    auto xs = l->add_nodes(xi), ys = l->add_nodes(yi), ps = l->add_nodes(pi);
    for (auto x : xs) for (auto p : ps) for (auto y : ys) l->connect(x, p, y);
    delete l;
}
