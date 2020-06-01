#include <set>
#include "Layer.h"

constexpr const char* cividis_colormap[256] = {"0.0 0.135112 0.304751", "0.0 0.138068 0.311105", "0.0 0.141013 0.317579", "0.0 0.143951 0.323982", "0.0 0.146877 0.330479", "0.0 0.149791 0.337065", "0.0 0.152673 0.343704", "0.0 0.155377 0.3505", "0.0 0.157932 0.357521", "0.0 0.160495 0.364534", "0.0 0.163058 0.371608", "0.0 0.165621 0.378769", "0.0 0.168204 0.385902", "0.0 0.1708 0.3931", "0.0 0.17342 0.400353", "0.0 0.176082 0.407577", "0.0 0.178802 0.414764", "0.0 0.18161 0.421859", "0.0 0.18455 0.428802", "0.0 0.186915 0.435532", "0.0 0.188769 0.439563", "0.0 0.19095 0.441085", "0.0 0.193366 0.441561", "0.003602 0.195911 0.441564", "0.017852 0.198528 0.441248", "0.03211 0.201199 0.440785", "0.046205 0.203903 0.440196", "0.058378 0.206629 0.439531", "0.068968 0.209372 0.438863", "0.078624 0.212122 0.438105", "0.087465 0.214879 0.437342", "0.095645 0.217643 0.436593", "0.103401 0.220406 0.43579", "0.110658 0.22317 0.435067", "0.117612 0.225935 0.434308", "0.124291 0.228697 0.433547", "0.130669 0.231458 0.43284", "0.13683 0.234216 0.432148", "0.142852 0.236972 0.431404", "0.148638 0.239724 0.430752", "0.154261 0.242475 0.43012", "0.159733 0.245221 0.429528", "0.165113 0.247965 0.428908", "0.170362 0.250707 0.428325", "0.17549 0.253444 0.42779", "0.180503 0.25618 0.427299", "0.185453 0.258914 0.426788", "0.190303 0.261644 0.426329", "0.195057 0.264372 0.425924", "0.199764 0.267099 0.425497", "0.204385 0.269823 0.425126", "0.208926 0.272546 0.424809", "0.213431 0.275266 0.42448", "0.217863 0.277985 0.424206", "0.222264 0.280702 0.423914", "0.226598 0.283419 0.423678", "0.230871 0.286134 0.423498", "0.23512 0.288848 0.423304", "0.239312 0.291562 0.423167", "0.243485 0.294274 0.423014", "0.247605 0.296986 0.422917", "0.251675 0.299698 0.422873", "0.255731 0.302409 0.422814", "0.25974 0.30512 0.42281", "0.263738 0.307831 0.422789", "0.267693 0.310542 0.422821", "0.271639 0.313253 0.422837", "0.275513 0.315965 0.422979", "0.279411 0.318677 0.423031", "0.28324 0.32139 0.423211", "0.287065 0.324103 0.423373", "0.290884 0.326816 0.423517", "0.294669 0.329531 0.423716", "0.298421 0.332247 0.423973", "0.302169 0.334963 0.424213", "0.305886 0.337681 0.424512", "0.309601 0.340399 0.42479", "0.313287 0.34312 0.42512", "0.316941 0.345842 0.425512", "0.320595 0.348565 0.425889", "0.32425 0.351289 0.42625", "0.327875 0.354016 0.42667", "0.331474 0.356744 0.427144", "0.335073 0.359474 0.427605", "0.338673 0.362206 0.428053", "0.342246 0.364939 0.428559", "0.345793 0.367676 0.429127", "0.349341 0.370414 0.429685", "0.352892 0.373153 0.430226", "0.356418 0.375896 0.430823", "0.359916 0.378641 0.431501", "0.363446 0.381388 0.432075", "0.366923 0.384139 0.432796", "0.37043 0.38689 0.433428", "0.373884 0.389646 0.434209", "0.377371 0.392404 0.43489", "0.38083 0.395164 0.435653", "0.384268 0.397928 0.436475", "0.387705 0.400694 0.437305", "0.391151 0.403464 0.438096", "0.394568 0.406236 0.438986", "0.397991 0.409011 0.439848", "0.401418 0.41179 0.440708", "0.40482 0.414572 0.441642", "0.408226 0.417357 0.44257", "0.411607 0.420145 0.443577", "0.414992 0.422937 0.444578", "0.418383 0.425733 0.44556", "0.421748 0.428531 0.44664", "0.42512 0.431334 0.447692", "0.428462 0.43414 0.448864", "0.431817 0.43695 0.449982", "0.435168 0.439763 0.451134", "0.438504 0.44258 0.452341", "0.44181 0.445402 0.453659", "0.445148 0.448226 0.454885", "0.448447 0.451053 0.456264", "0.451759 0.453887 0.457582", "0.455072 0.456718 0.458976", "0.458366 0.459552 0.460457", "0.461616 0.462405 0.461969", "0.464947 0.465241 0.463395", "0.468254 0.468083 0.464908", "0.471501 0.47096 0.466357", "0.474812 0.473832 0.467681", "0.478186 0.476699 0.468845", "0.481622 0.479573 0.469767", "0.485141 0.482451 0.470384", "0.488697 0.485318 0.471008", "0.492278 0.488198 0.471453", "0.495913 0.491076 0.471751", "0.499552 0.49396 0.472032", "0.503185 0.496851 0.472305", "0.506866 0.499743 0.472432", "0.51054 0.502643 0.47255", "0.514226 0.505546 0.47264", "0.51792 0.508454 0.472707", "0.521643 0.511367 0.472639", "0.525348 0.514285 0.47266", "0.529086 0.517207 0.472543", "0.532829 0.520135 0.472401", "0.536553 0.523067 0.472352", "0.540307 0.526005 0.472163", "0.544069 0.528948 0.471947", "0.54784 0.531895 0.471704", "0.551612 0.534849 0.471439", "0.555393 0.537807 0.471147", "0.559181 0.540771 0.470829", "0.562972 0.543741 0.470488", "0.566802 0.546715 0.469988", "0.570607 0.549695 0.469593", "0.574417 0.552682 0.469172", "0.578236 0.555673 0.468724", "0.582087 0.55867 0.468118", "0.585916 0.561674 0.467618", "0.589753 0.564682 0.46709", "0.593622 0.567697 0.466401", "0.597469 0.570718 0.465821", "0.601354 0.573743 0.465074", "0.605211 0.576777 0.464441", "0.609105 0.579816 0.463638", "0.612977 0.582861 0.46295", "0.616852 0.585913 0.462237", "0.620765 0.58897 0.461351", "0.624654 0.592034 0.460583", "0.628576 0.595104 0.459641", "0.632506 0.59818 0.458668", "0.636412 0.601264 0.457818", "0.640352 0.604354 0.456791", "0.64427 0.60745 0.455886", "0.648222 0.610553 0.454801", "0.652178 0.613664 0.453689", "0.656114 0.61678 0.452702", "0.660082 0.619904 0.451534", "0.664055 0.623034 0.450338", "0.668008 0.626171 0.44927", "0.671991 0.629316 0.448018", "0.675981 0.632468 0.446736", "0.679979 0.635626 0.445424", "0.68395 0.638793 0.444251", "0.687957 0.641966 0.442886", "0.691971 0.645145 0.441491", "0.695985 0.648334 0.440072", "0.700008 0.651529 0.438624", "0.704037 0.654731 0.437147", "0.708067 0.657942 0.435647", "0.712105 0.66116 0.434117", "0.716177 0.664384 0.432386", "0.720222 0.667618 0.430805", "0.724274 0.670859 0.429194", "0.728334 0.674107 0.427554", "0.732422 0.677364 0.425717", "0.736488 0.680629 0.424028", "0.740589 0.6839 0.422131", "0.744664 0.687181 0.420393", "0.748772 0.69047 0.418448", "0.752886 0.693766 0.416472", "0.756975 0.697071 0.414659", "0.761096 0.700384 0.412638", "0.765223 0.703705 0.410587", "0.769353 0.707035 0.408516", "0.773486 0.710373 0.406422", "0.777651 0.713719 0.404112", "0.781795 0.717074 0.401966", "0.785965 0.720438 0.399613", "0.790116 0.72381 0.397423", "0.794298 0.72719 0.395016", "0.79848 0.73058 0.392597", "0.802667 0.733978 0.390153", "0.806859 0.737385 0.387684", "0.811054 0.740801 0.385198", "0.815274 0.744226 0.382504", "0.819499 0.747659 0.379785", "0.823729 0.751101 0.377043", "0.827959 0.754553 0.374292", "0.832192 0.758014 0.371529", "0.836429 0.761483 0.368747", "0.840693 0.764962 0.365746", "0.844957 0.76845 0.362741", "0.849223 0.771947 0.359729", "0.853515 0.775454 0.3565", "0.857809 0.778969 0.353259", "0.862105 0.782494 0.350011", "0.866421 0.786028 0.346571", "0.870717 0.789572 0.343333", "0.875057 0.793125 0.339685", "0.879378 0.796687 0.336241", "0.88372 0.800258 0.332599", "0.888081 0.803839 0.32877", "0.89244 0.80743 0.324968", "0.896818 0.81103 0.320982", "0.901195 0.814639 0.317021", "0.905589 0.818257 0.312889", "0.91 0.821885 0.308594", "0.914407 0.825522 0.304348", "0.918828 0.829168 0.29996", "0.923279 0.832822 0.295244", "0.927724 0.836486 0.290611", "0.93218 0.840159 0.28588", "0.93666 0.843841 0.280876", "0.941147 0.84753 0.275815", "0.945654 0.851228 0.270532", "0.950178 0.854933 0.265085", "0.954725 0.858646 0.259365", "0.959284 0.862365 0.253563", "0.963872 0.866089 0.247445", "0.968469 0.869819 0.24131", "0.973114 0.87355 0.234677", "0.97778 0.877281 0.227954", "0.982497 0.881008 0.220878", "0.987293 0.884718 0.213336", "0.992218 0.888385 0.205468", "0.994847 0.892954 0.203445", "0.995249 0.898384 0.207561", "0.995503 0.903866 0.21237", "0.995737 0.909344 0.217772"};

Layer::Layer(Layer* parent, std::string name) : parent(parent), name(std::move(name)) {
    level = parent->level + 1;
}

Layer* Layer::add_layer(std::string new_name) {
    auto new_layer = new Layer(this, new_name.empty() ? name + "_" + std::to_string(children.size()) : new_name);
    children.push_back(new_layer);
    return new_layer;
}

CType Layer::operator[](Address addr) const {
    auto [index, upstream] = addr;
    if (level < upstream) throw std::out_of_range("Parent has no knowledge of child data.");
    unsigned int to_raise = level - upstream;
    auto layer = this;
    while (layer->parent and to_raise--) layer = layer->parent;
    return layer->data[index];
}

std::optional<Address> Layer::find(CType item) const {
    auto it = std::find(data.begin(), data.end(), item);
    if (it == data.end()) return {};
    auto a = Address(std::distance(data.begin(), it), level);
    if (removed_nodes.contains(a)) return {};
    return a;
}

std::vector<Address> Layer::find_all(std::function<bool (Address)> cond) const {
    std::vector<Address> cum; Address a;
    if (parent) cum = parent->find_all(cond);
    for (int i = 0; i < data.size(); ++i)
        if (cond(a = Address(i, level)))
            cum.push_back(a);
    return cum;
}

Layer::EdgeMap::iterator Layer::connect(Address s, Address p, Address o) {
    removed_edges.erase(Triple(s, p, o));
    //auto nit = conn.emplace(s, std::make_pair(p, o));
    auto [begin, end] = conn.equal_range(s);
    bool found_property = false;
    for (auto it = begin; it != end; ++it) {
        if (it->second.first == p) {
            found_property = true;
            if (it->second.second == o) return it;
        } else if (found_property) break;
    }
    return conn.emplace(s, std::make_pair(p, o));
}

Layer::EdgeMap::iterator Layer::disconnect(Layer::EdgeMap::iterator it) {
    removed_edges.emplace(it->first, it->second.first, it->second.second);
    return conn.erase(it);
}

Layer::EdgeMap::iterator Layer::disconnect(Address s, Address p, Address o) {
    removed_edges.emplace(s, p ,o);

    auto [begin, end] = conn.equal_range(s);
    for (auto it = begin; it != end; ++it)
        if (it->second.first == p and it->second.second == o)
            return conn.erase(it);

    return conn.end();
}

std::vector<Address> Layer::get_objects(Address s, Address p) {
    if (removed_nodes.contains(s) or removed_nodes.contains(p)) return {};
    std::vector<Address> objects; Address o;
    const Layer* l = this;
    while (l) {
        auto [begin, end] = l->conn.equal_range(s);
        for (auto it = begin; it != end; ++it)
            if (it->second.first == p and not removed_nodes.contains(o = it->second.second))
                objects.push_back(o);
        l = l->parent;
    }
   return objects;
}

Layer* Layer::operator<<=(Layer* other)  {
    /// Addresses used in other are not valid in this, see `lift` above
    unsigned int shift = this->data.size();
    data.reserve(data.size() + other->data.size());
    data.insert(data.end(), other->data.begin(), other->data.end());
    for (auto [s, po] : other->conn) {
        auto [p, o] = po;
        Address s_, p_, o_;
        s_ = (s.second == other->level) ? Address(s.first + shift, level) : s;
        p_ = (p.second == other->level) ? Address(p.first + shift, level) : p;
        o_ = (o.second == other->level) ? Address(o.first + shift, level) : o;
        conn.insert({s_, std::make_pair(p_, o_)});
    }
    return this;
}

int Layer::free_tree()  {
    int total_freed = children.size();
    for (Layer* child : children) {
        total_freed += child->free_tree();
        delete child;
    }
    children.clear();
    return total_freed;
}

#define show_p(x) std::to_string(reinterpret_cast<uintptr_t>(x))
#define show_a(addr) "I" + std::to_string(addr.first) + "L" + std::to_string(addr.second)

void Layer::draw() const {
    std::vector<const Layer*> path = {this};
    const Layer* up = this;
    while ((up = up->parent)) path.push_back(up);

    for (int i = path.size() - 1; i >= 0; --i) {
        const Layer* graph = path[i];
        unsigned int c_index = 256*((double)graph->level / (double)path.size());
        std::cout << "node [penwidth=2, color=\"" << cividis_colormap[c_index] << "\"];\n";
        std::cout << "edge [penwidth=1.5, color=\"" << cividis_colormap[c_index] << "\"];\n";
        for (int index = 0; index < graph->data.size(); ++index)
            std::cout <<  show_a(Address(index, graph->level)) << " [label=\"" << graph->data[index] << "\"];\n";
        for (auto [s, po] : graph->conn)
            std::cout << show_a(s) << " -> " << show_a(po.second) << " [label=\"" << (*this)[po.first] << "\"];\n";
    }
}

void Layer::draw_hierarchy() const {
    /*  draw hierarchy
    std::cout << "digraph G {\n"
                 "nodesep=.05;\n"
                 "rankdir=LR;\n"
                 "node [shape=record,width=.1,height=.1];\n";
    r.draw_hierarchy();
    std::cout << "}\n";
     */
    std::cout << show_p(this) << " [label = \"{" << this->name << "|" << data.size() << "|" << conn.size() << "}\"];\n";
    for (Layer* child : children) {
        std::cout << show_p(this) << " -> " << show_p(child) << ";\n";
        child->draw_hierarchy();
    }
}

Address Layer::hylo(Address seed, F pre, F post) {
    this->data[seed.first] = pre(this, seed);
    auto [begin, end] = conn.equal_range(seed);
    std::vector<Address> temp(std::distance(begin, end)); auto it = begin;
    std::transform(begin, end, temp.begin(), [](EdgeMap::value_type& s_po) {return s_po.second.second;});
    std::for_each(temp.begin(), temp.end(), [&](Address& o) {(it++)->second.second = hylo(o, pre, post);});
    this->data[seed.first] = post(this, seed);
    return seed;
}

Layer* Layer::rewrite(Layer* l, Layer* r) {
    auto rewrite_layer = new Layer(this, "Rewrite" + this->name + "From" + l->name + "To" + r->name);
    std::unordered_map<Address, Address, hash_pair> phiV;
    std::unordered_map<Triple , Triple, hash_triple> phiE;
    // this G   match_replace P   (part of G that matches P) M   (G\M) C  (edges M->C ⋃ uncaptured P) J
    // ...
    return r;
}
