#ifndef HIERARCHY_LAYER_H
#define HIERARCHY_LAYER_H
#include <vector>
#include <array>
#include <numeric>
#include <string>
#include <tuple>
#include <unordered_map>
#include <iostream>
#include <functional>
#include <variant>
#include <unordered_set>
#include <map>

class Layer;
using CType = std::variant<std::string, int, double>;
using Address = std::pair<unsigned int, unsigned int>;
using Triple = std::tuple<Address, Address, Address>;
using F = std::function<CType (Layer*, Address)>;

static std::ostream& operator<<(std::ostream& s, CType const& v) {
    std::visit([&](auto&& arg){s << arg;}, v);
    return s;
}

struct hash_pair {
    static size_t hash(unsigned int x) {
        x = ((x >> 16u) ^ x) * 0x45d9f3b;
        x = ((x >> 16u) ^ x) * 0x45d9f3b;
        x = (x >> 16u) ^ x;
        return static_cast<size_t>(x);
    }

    size_t operator()(const Address& value) const {
        return value.first ^ ((2u << 31u) - value.second);
        //return hash(value.first) ^ ((2u << 31u) - hash(value.second));
    }
};

struct hash_triple {
    size_t operator()(const Triple& value) const {
        unsigned int p1 = 143483, p2 = 60169;
        auto [s, p, o] = value;
        return hash_pair{}(s) ^ p1*(hash_pair{}(p) ^ p2*hash_pair{}(o));
    }
};


class Layer {
    using EdgeMap = std::unordered_multimap<Address, std::pair<Address, Address>, hash_pair>;

    std::vector<CType> data;
    std::unordered_set<Address, hash_pair> removed_nodes;
    std::unordered_set<Triple, hash_triple> removed_edges;

    Layer* parent = nullptr;
    std::vector<Layer*> children;
public:
    EdgeMap conn;

    unsigned int level = 0;
    std::string name = "root";

    Layer() = default;
    explicit Layer(Layer* parent, std::string name);
    Layer* add_layer(std::string new_name = "");

    template <typename DataType>
    Address add_node(DataType item) {
        data.push_back(item);
        return Address(data.size() - 1, level);
    }

    void remove_node(Address n) {
        removed_nodes.insert(n);
    }

    template <typename DataType>
    std::vector<Address> add_nodes(std::vector<DataType> items) {
        int s = items.size(), ds = data.size();
        std::vector<Address> node_addresses(s);
        data.reserve(ds + s);
        for (int i = 0; i < s; ++i) {
            data.push_back(items[i]);
            node_addresses[i] = Address(ds + i, level);
        }
        return node_addresses;
    };

    template <typename ...Args>
    std::array<Address, sizeof...(Args)> wrap(Args... items) {
        return {add_node(items)...};
    };

    const CType& operator[](Address addr) const;
    std::optional<Address> find(CType item) const;
    std::vector<Address> find_all(std::function<bool (Address)> cond) const;
    EdgeMap::iterator connect(Address s, Address p, Address o);
    EdgeMap::iterator disconnect(EdgeMap::iterator it);
    EdgeMap::iterator disconnect(Address s, Address p, Address o);
    std::vector<Address> get_objects(Address s, Address p);

    Layer* operator<<=(Layer* other);

    unsigned int free_tree();

    void draw() const;
    void draw_hierarchy() const;

    static constexpr auto Fid = [](Layer* l_, Address n){return (*l_)[n];};
    Address hylo(Address seed, F pre = Fid, F post  = Fid);
    Layer* rewrite(Layer* l, Layer* r);
};

#endif //HIERARCHY_LAYER_H
