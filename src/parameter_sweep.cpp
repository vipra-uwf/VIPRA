
#include "vipra/parameter_sweep/parameter_sweep.hpp"
#include "vipra/util/timing.hpp"

namespace VIPRA {
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables) Bug in clang-tidy
#ifdef VIPRA_USE_MPI
MPI_Comm ParameterSweep::comm{};
#endif

int                              ParameterSweep::rank{};
int                              ParameterSweep::size{};
ParameterSweep::DeferredFinalize ParameterSweep::_finalize{};
Util::Timings                    ParameterSweep::_timings{"run"};
Util::Timings                    ParameterSweep::_inputTimings{"input_time"};
Util::Timings                    ParameterSweep::_mpiTimings{"mpi_time"};

#ifdef VIPRA_TIME_SIM
std::filesystem::path Util::Timings::timingPath{};
#endif

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
}  // namespace VIPRA