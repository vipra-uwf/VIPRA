#pragma once

#include <cassert>
#include <optional>
#include <stdexcept>
#include <vector>

#include "vipra/concepts/string_view.hpp"

#include "vipra/modules/module.hpp"

#include "vipra/modules/util.hpp"
#include "vipra/util/crtp.hpp"

// TODO(rolland): remember to add in documentation that inputs should hold off on loading until their load() method is called

namespace VIPRA::Modules {

template <typename module_t>
class Input : public Util::CRTP<Input<module_t>> {
  using Util::CRTP<Input<module_t>>::self;

 public:
  FORWARD_REGISTER_PARAMS;

  void load()
  {
    // TODO(rolland) maybe have load_impl return a bool and handle if it failed?
    if ( _loaded ) return;
    _loaded = true;
    self().load_impl();
  }

  /**
   * @brief Uses the Input implementation to get a value at {keys}
   * 
   * @tparam data_t 
   * @tparam keys_t 
   * @param keys 
   * @return std::optional<data_t> 
   */
  template <typename data_t>
  auto get(std::vector<std::string> const& keys) const -> std::optional<data_t>
  {
    assert(_loaded);

    try {
      return self().template get_impl<data_t>(keys);
    }
    catch ( std::exception& ex ) {
      // TODO(rolland): if the input module has an issue should we just let it throw or just return nullopt like this?
      // If the module has an issue getting the value return nullopt
      return std::nullopt;
    }
  }

  void set_loaded(bool loaded) { _loaded = loaded; }

 private:
  bool _loaded = false;
};

}  // namespace VIPRA::Modules
