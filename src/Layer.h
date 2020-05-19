#ifndef HIERARCHY_LAYER_H
#define HIERARCHY_LAYER_H
#include <vector>
#include <array>
#include <string>
#include <tuple>
#include <unordered_map>
#include <iostream>
#include <functional>
#include <variant>

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
        //return value.first ^ value.second;  // what's best, when?
        return hash(value.first) ^ ((2u << 31u) - hash(value.second));
    }
};

struct hash_triple {
    size_t operator()(const Triple& value) const {
        // return hash(str.first ^ str.second);  // what's best, when?
        auto [s, p, o] = value;
        return hash_pair{}(s) ^ hash_pair{}(p) ^ hash_pair{}(o);
    }
};


class Layer {
    using EdgeMap = std::unordered_multimap<Address, std::pair<Address, Address>, hash_pair>;

    std::vector<CType> data;

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
        // could be made lazy
        auto [index, upstream] = n;
        if (level < upstream) throw std::out_of_range("Parent has no knowledge of child data.");
        if (upstream == level) {
            std::erase_if(conn, [n](EdgeMap::value_type s_po){
                return s_po.first == n or s_po.second.first == n or s_po.second.second == n;
            });
            data.erase(data.begin() + index);
        } else throw std::runtime_error("Not implemented yet"); // store parent deleted nodes and transfer on merge?
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

    CType operator[](Address addr) const;
    std::optional<Address> find(CType item) const;
    EdgeMap::iterator connect(Address s, Address p, Address o);
    EdgeMap::iterator disconnect(EdgeMap::iterator it);
    EdgeMap::iterator disconnect(Address s, Address p, Address o);
    std::vector<Address> get_objects(Address s, Address p);
    std::vector<Address> lift(std::vector<Address> addresses, Layer* lifted) const;

    Layer* operator<<=(Layer* other);

    int free_tree();

    void draw() const;
    void draw_hierarchy() const;

    Address hylo(Address seed, F pre, F post);
    Layer* rewrite(Layer* l, Layer* r);
};

#endif //HIERARCHY_LAYER_H
