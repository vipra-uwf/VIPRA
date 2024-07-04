#pragma once

#ifdef VIPRA_PERF_TESTING

#include "vipra/performance/performance_testing.hpp"

#define CONCAT_(x, y) x##y
#define CONCAT(x, y) CONCAT_(x, y)
#define uniquename CONCAT(sb_, __COUNTER__)

#define VIPRA_PERF_ADD_CALL(name) VIPRA::Perf::add_call(name);

#define VIPRA_PERF_START(name) VIPRA::Perf::start(name);

#define VIPRA_PERF_STOP(name) VIPRA::Perf::stop(name);

#define VIPRA_PERF_FUNCTION(name) VIPRA::Perf::FunctionTimer uniquename = VIPRA::Perf::FunctionTimer{name};

#define VIPRA_OUTPUT_PERFORMANCE std::cout << VIPRA::Perf::report();

#else

#define VIPRA_PERF_ADD_CALL(name)

#define VIPRA_PERF_START(name)

#define VIPRA_PERF_STOP(name)

#define VIPRA_PERF_FUNCTION(name)

#define VIPRA_OUTPUT_PERFORMANCE

#endif