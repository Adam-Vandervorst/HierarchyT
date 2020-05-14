//
// Created by adamv on 5/7/20.
//

#ifndef HIERARCHY_BENCHMARKS_H
#define HIERARCHY_BENCHMARKS_H

#include "Layer.h"

namespace benchmarks {
    // Implementation
    //          nodes[K] time[ms] total[MiB] heap[MiB]
    // HashMap     1000      878        171       171
    //              100       79         22
    //               10        7          2
    // Pointers    1000     1184        818       229
    //              100      133         84
    //               10       11          8
    void hylo_performance();
}

#endif //HIERARCHY_BENCHMARKS_H
