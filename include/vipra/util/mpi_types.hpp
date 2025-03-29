#pragma once

#ifdef VIPRA_USE_MPI
#include <mpi.h>
namespace VIPRA::Util {

template <typename data_t>
[[nodiscard]] inline auto get_mpi_type() -> MPI_Datatype;

template <>
[[nodiscard]] inline auto get_mpi_type<double>() -> MPI_Datatype
{
  return MPI_DOUBLE;
}

template <>
[[nodiscard]] inline auto get_mpi_type<char>() -> MPI_Datatype
{
  return MPI_CHAR;
}

template <>
[[nodiscard]] inline auto get_mpi_type<int8_t>() -> MPI_Datatype
{
  return MPI_INT8_T;
}

template <>
[[nodiscard]] inline auto get_mpi_type<int16_t>() -> MPI_Datatype
{
  return MPI_INT16_T;
}

template <>
[[nodiscard]] inline auto get_mpi_type<int>() -> MPI_Datatype
{
  return MPI_INT;
}

template <>
[[nodiscard]] inline auto get_mpi_type<int64_t>() -> MPI_Datatype
{
  return MPI_INT64_T;
}

}  // namespace VIPRA::Util
#endif