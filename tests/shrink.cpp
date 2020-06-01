#include "../src/Layer.h"
#include "shrink.h"

void remove_single_node() {
    auto l = new Layer();
    auto a = l->add_node(1);
    l->remove_node(a);
    if (l->find(CType(1)).has_value()) throw std::logic_error("node not deleted");
    delete l;
}

void remove_re_add_node() {
    auto l = new Layer();
    auto a = l->add_node(1);
    l->remove_node(a);
    auto a_ = l->add_node(1);
    if (a == a_) throw std::logic_error("re-add equal");
    delete l;
}

void remove_nodes_with_data() {
    auto l = new Layer();
    auto [a, b, c, d] = l->wrap(1, 1, "c", 1);
    while (auto n = l->find(1)) l->remove_node(n.value());
    if (l->find(1).has_value()) throw std::logic_error("nodes not removed");
    if (not std::get_if<std::string>(&(*l)[c])) throw std::logic_error("removing nodes invalidates address");
    delete l;
}

void remove_source_heavy_node() {
    auto l = new Layer();
    auto x = l->add_node("to delete");
    std::vector<int> yi(100), pi(100);
    auto ys = l->add_nodes(yi), ps = l->add_nodes(pi);
    for (auto y : ys) for (auto p : ps) l->connect(x, p, y);
    l->remove_node(x);
    delete l;
}

void remove_target_heavy_node() {
    auto l = new Layer();
    auto y = l->add_node("to delete");
    std::vector<int> xi(100), pi(100);
    auto xs = l->add_nodes(xi), ps = l->add_nodes(pi);
    for (auto x : xs) for (auto p : ps) l->connect(x, p, y);
    l->remove_node(y);
    delete l;
}

void remove_property_heavy_node() {
    auto l = new Layer();
    auto p = l->add_node("to delete");
    std::vector<int> xi(100), yi(100);
    auto xs = l->add_nodes(xi), ys = l->add_nodes(yi);
    for (auto x : xs) for (auto y : ys) l->connect(x, p, y);
    l->remove_node(p);
    delete l;
}

void remove_single_edge_address() {
    auto l = new Layer();
    auto [a, p, q, b] = l->wrap("a", "p", "q", "b");
    l->connect(a, p, b); l->connect(a, q, b);
    l->disconnect(a, p, b);
    for (auto it = l->conn.find(a); it != l->conn.end(); ++it)
        if (it->second.first == p) throw std::logic_error("edge should be deleted");
    delete l;
}

void remove_single_edge_iterator() {
    auto l = new Layer();
    auto [a, p, q, b] = l->wrap("a", "p", "q", "b");
    auto ep = l->connect(a, p, b), eq = l->connect(a, q, b);
    l->disconnect(eq);
    for (auto it = l->conn.find(a); it != l->conn.end(); ++it)
        if (it->second.first == q) throw std::logic_error("edge should be deleted");
    delete l;
}

void remove_re_add_edge() {
    auto l = new Layer();
    auto [a, p, b] = l->wrap("a", "p", "b");
    l->connect(a, p, b);
    if (l->get_objects(a, p).empty()) throw std::logic_error("edge not connected");
    l->disconnect(a, p, b);
    if (!l->get_objects(a, p).empty()) throw std::logic_error("edge not disconnected");
    l->connect(a, p, b);
    if (l->get_objects(a, p).empty()) throw std::logic_error("edge not reconnected");
    delete l;
}

void remove_many_edges() {
    auto l = new Layer();
    std::vector<int> xi(10), pi(100), yi(10);
    auto xs = l->add_nodes(xi), ys = l->add_nodes(yi), ps = l->add_nodes(pi);
    for (auto x : xs) for (auto p : ps) for (auto y : ys) l->connect(x, p, y);
    for (auto x : xs) for (auto p : ps) for (auto y : ys) l->disconnect(x, p, y);
    delete l;
}
