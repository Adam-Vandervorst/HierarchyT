#include <vector>
#include <chrono>
#include <iomanip>

#include "tests/grow.h"
#include "tests/shrink.h"
#include "tests/layer.h"
#include "tests/morph.h"
#include "tests/compute.h"

struct format {
    std::string str;
#ifdef __unix__
    static constexpr const char* succ = "\033[32m[✔]\033[0m";
    static constexpr const char* fail = "\033[31m[✗]\033[0m";
#else
    static constexpr const char* succ = "[v]";
    static constexpr const char* fail = "[x]";
#endif
    friend std::ostream& operator<<(std::ostream& os, const format& res) {
        os << res.str;
        return os;
    }

    static format res(double value, double threshold = 20.) {
        std::string mark; std::stringstream ss; ss << std::showpos << std::fixed << std::setprecision(1);
#ifdef __unix__
        if (value > threshold) mark = "\033[31m"; if (value < -threshold) mark = "\033[32m";
        ss << mark << value << "\033[0m";
#else
        ss << value;
#endif
        return format{ss.str()};
    }

    static format summary(std::string section, int s, int f, double t) {
        std::stringstream ss;
        ss << (f ? fail : succ) << " " << section << " success/failed: " << s << "/" << f;
        if (s) ss << ", average time: " << res(t/(double)s) << "%";
        ss << std::endl;
        return format{ss.str()};
    }
};

using namespace std::chrono;

const std::vector<std::pair<std::string, std::vector<std::pair<std::string, std::function<void ()>>>>> all_tests = {
        {"node-basic", {
            {"add-single-node", add_single_node},
            {"add-nodes-wrap", add_nodes_wrap},
            {"get-node-data", get_node_data},
            {"find-node-with-data", find_node_with_data},
            {"remove-single-node", remove_single_node},
            {"remove-re-add-node", remove_re_add_node},
            {"remove-nodes-with-data", remove_nodes_with_data},
        }},
        {"edge-basic", {
            {"add-single-edge", add_single_edge},
            {"add-edges", add_edges},
            {"get-node-targets", get_node_targets},
            {"remove-single-edge-address", remove_single_edge_address},
            {"remove-single-edge-iterator", remove_single_edge_iterator},
            {"remove-re-add-edge", remove_re_add_edge},
        }},
        {"layer-basic", {
            {"add-stack-layers", add_stack_layers},
            {"remove-stack-layers", remove_stack_layers},
        }},
        {"cross-implementation-benchmark", {
            {"hylo-tree-avg", hylo_tree_avg},
            {"random-graph", random_graph},
            {"factorial-tree-count", factorial_tree_count},
        }},
        {"synthetic-benchmark", {
            {"add-many-nodes", add_many_nodes},
            {"add-many-edges", add_many_edges},
            {"remove-source-heavy-node", remove_source_heavy_node},
            {"remove-target-heavy-node", remove_target_heavy_node},
            {"remove-property-heavy-node", remove_property_heavy_node},
            {"remove-many-edges", remove_many_edges},
        }},
};

const std::vector<long> expected_test_times = {
    97, 162, 104, 214, 175, 167, 270,
    225, 439, 763, 415, 387, 542,
    946, 1025,
    6451387, 6239687, 8809388,
    147746, 36873137, 352906065, 4727372, 5074571, 86119561,
};


void test_all(unsigned int runs = 10) {
    std::vector<long> new_test_times; unsigned int test_i = 0;
    int group_s, group_f, total_s = 0, total_f = 0;
    double group_t, total_t = 0.;

    for (auto [group_name, tests] : all_tests) {
        group_s = 0; group_f = 0; group_t = 0.;
        for (auto [name, test] : tests) {
            auto start = high_resolution_clock::now();
            try {
                for (unsigned int i = 0; i < runs; ++i) test();
            } catch (std::exception& e) {
                std::cout << format::fail << " " << name << " : " << e.what() << std::endl;
                ++group_f; ++test_i;
                continue;
            }
            auto stop = high_resolution_clock::now();
            long avg_time = duration_cast<nanoseconds>(stop - start).count()/runs;
            double extra_time_percentage = (double)(avg_time - expected_test_times[test_i])/(.01*expected_test_times[test_i]);

            std::cout << format::succ << " " << name << " " << format::res(extra_time_percentage) << "%" << std::endl;
            ++group_s; ++test_i; group_t += extra_time_percentage;
        }
        std::cout << "\t" << format::summary(group_name, group_s, group_f, group_t);
        total_s += group_s; total_f += group_f; total_t += group_t;
    }
    std::cout << "\t\t"  << format::summary("total", total_s, total_f, total_t);
}


void print_test_times(unsigned int runs = 100) {
    std::cout << "{" << std::endl;
    for (auto [_, tests] : all_tests) {
        for (auto [_, test] : tests) {
            auto start = high_resolution_clock::now();
            for (unsigned int i = 0; i < runs; ++i) test();
            auto stop = high_resolution_clock::now();
            std::cout << duration_cast<nanoseconds>(stop - start).count()/runs << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << "};";
}
