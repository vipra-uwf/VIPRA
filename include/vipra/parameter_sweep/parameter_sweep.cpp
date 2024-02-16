

#include "parameter_sweep.hpp"

namespace VIPRA {
MPI_Comm ParameterSweep::comm{};
int      ParameterSweep::rank{};
int      ParameterSweep::size{};
}  // namespace VIPRA