#pragma once

#include <filesystem>
#include <fstream>

#include "vipra/types/idx.hpp"
#include "vipra/util/crtp.hpp"

namespace VIPRA::Modules {
/**
 * @brief Dummy output for use in other concepts
 * 
 */
template <typename module_t>
class Output : public Util::CRTP<Output<module_t>> {
  using Util::CRTP<Output<module_t>>::self;

 public:
  void write(std::filesystem::path const& outputDir)
  {
    std::filesystem::path filepath = outputDir / self().filename();
    std::ofstream         file(filepath);
    if ( ! file.is_open() ) {
      throw std::runtime_error("Could not open file for writing: " + filepath.string());
    }

    file << self().to_string();

    file.close();
  }

  template <typename data_t>
  void sim_value(std::string const& key, data_t&& value)
  {
    self().set_sim_value(key, std::forward<data_t>(value));
  }

  template <typename data_t>
  void timestep_value(std::string const& key, VIPRA::idx timestep, data_t&& value)
  {
    self().set_timestep_value(key, timestep, std::forward<data_t>(value));
  }

  template <typename data_t>
  void ped_value(VIPRA::idx pedIdx, std::string const& key, data_t&& value)
  {
    self().set_ped_value(pedIdx, key, std::forward<data_t>(value));
  }

  template <typename data_t>
  void ped_timestep_value(VIPRA::idx pedIdx, VIPRA::idx timestep, std::string const& key, data_t&& value)
  {
    self().set_ped_timestep_value(pedIdx, timestep, key, std::forward<data_t>(value));
  }
};

}  // namespace VIPRA::Modules
