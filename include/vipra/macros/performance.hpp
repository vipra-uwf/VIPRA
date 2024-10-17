#pragma once

#ifdef VIPRA_PERF_TESTING

#define VIPRA_INLINE

#else

#define VIPRA_INLINE __attribute__((always_inline))

#endif