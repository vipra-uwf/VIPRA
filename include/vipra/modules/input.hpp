#pragma once

#include <cassert>
#include <optional>
#include <vector>

#include "vipra/concepts/string_view.hpp"

#include "vipra/modules/module.hpp"

#include "vipra/util/crtp.hpp"

// TODO(rolland): remember to add in documentation that inputs should hold off on loading until their load() method is called

namespace VIPRA::Modules {

template <typename module_t>
class Input : Util::CRTP<Input<module_t>> {
 public:
  void load() {
    if ( _loaded ) return;
    this->self().load_impl();
  }

  template <typename data_t, Concepts::StringView... keys_t>
  auto get(keys_t&&... keys) const -> std::optional<data_t> {
    assert(_loaded);

    return this->self().get(std::string_view(std::forward<keys_t>(keys))...);
  }

  template <typename data_t, Concepts::StringView... keys_t>
  auto get_vector(keys_t&&... keys) const -> std::optional<std::vector<data_t>> {
    assert(_loaded);

    return this->self().get_vector(std::string_view(std::forward<keys_t>(keys))...);
  }

  void set_loaded(bool loaded) { _loaded = loaded; }

 private:
  bool _loaded = false;
};

}  // namespace VIPRA::Modules
