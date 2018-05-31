#include "jast/statistics.h"
#include <iostream>

namespace jast {

void Statistics::dump() {
    std::cout << "-- Statistics\n";
#define PRINT_COUNTER(C) std::cout << #C << " = " << counters_[CountType::k##C] << "\n";
    COUNTER_TYPE(PRINT_COUNTER)
#undef PRINT_COUNTER

}

}

