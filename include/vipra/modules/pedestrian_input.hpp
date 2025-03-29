#pragma once

#include <optional>
#include "vipra/geometry/f3d.hpp"
namespace VIPRA::Modules {

class Pedestrians;

/**
 * @brief Base PedestrianInput Module
 * 
 */
class PedestrianInput {
 public:
  virtual void               load(std::string const&) = 0;
  [[nodiscard]] virtual auto get_pedestrians() const -> std::optional<VIPRA::f3dVec> = 0;

  PedestrianInput() = default;
  PedestrianInput(const PedestrianInput&) = default;
  PedestrianInput(PedestrianInput&&) = default;
  auto operator=(const PedestrianInput&) -> PedestrianInput& = default;
  auto operator=(PedestrianInput&&) -> PedestrianInput& = default;
  virtual ~PedestrianInput() = default;
};

}  // namespace VIPRA::Modules
