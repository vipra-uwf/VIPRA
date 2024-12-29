
#include "vipra/parameter_sweep/parameter_sweep.hpp"
#include "vipra/util/timing.hpp"

namespace VIPRA {
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables) Bug in clang-tidy
#ifdef VIPRA_USE_MPI
MPI_Comm ParameterSweep::comm{};
#endif

int                             ParameterSweep::rank{};
int                             ParameterSweep::size{};
ParameterSweep::DeferedFinalize ParameterSweep::_finalize{};
Util::Timings                   ParameterSweep::_timings{"sim_run"};
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
}  // namespace VIPRA