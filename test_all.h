#include <vector>
#include <map>

#include "tests/grow.h"
#include "tests/shrink.h"
#include "tests/layer.h"
#include "tests/morph.h"
#include "tests/compute.h"


const std::map<std::string, std::map<std::string, std::function<void ()>>> all_tests = {
        {"cross-implementation-benchmark", {
            {"hylo-tree-avg", hylo_performance}
        }}
};
const std::vector<double> test_times = {7000000};


void test_all() {
}
