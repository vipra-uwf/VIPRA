#pragma once

#ifdef VIPRA_PERF_TESTING

#define VIPRA_PERF_FUNC __attribute__((noinline))

#define VIPRA_INLINE

#else

#define VIPRA_PERF_FUNC

#define VIPRA_INLINE __attribute__((always_inline))

#endif