#pragma once

#include <filesystem>

#include "vipra/types/state.hpp"
#include "vipra/types/time.hpp"

namespace VIPRA::Modules {
/**
 * @brief Base Output module
 * 
 */
class Output {
 public:
  virtual void write(std::filesystem::path const& outputDir) = 0;
  virtual void timestep_update(VIPRA::timestep, VIPRA::delta_t, VIPRA::State const&) = 0;
  virtual void reset_module() = 0;

  virtual ~Output() = default;
  Output() = default;
  Output(const Output&) = default;
  Output(Output&&) = delete;
  auto operator=(const Output&) -> Output& = default;
  auto operator=(Output&&) -> Output& = delete;
};

}  // namespace VIPRA::Modules
