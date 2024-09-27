#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>

#include <dlfcn.h>

#include "vipra/modules.hpp"
#include "vipra/special_modules/parameters.hpp"

namespace VIPRA {

template <typename module_t>
inline auto load_module(std::string const& name, std::string const& installDir,
                        Modules::Type type)
    -> std::pair<
        std::unique_ptr<std::remove_cvref_t<module_t>>,
        std::function<void(void*, Parameters&, VIPRA::Random::Engine&)>>
{
  using mod_t = std::remove_cvref_t<module_t>;

  typedef std::unique_ptr<mod_t> (*make_module)();
  typedef void (*config_module)(void*, VIPRA::Parameters&,
                                VIPRA::Random::Engine&);

  std::string path = installDir + '/' + Modules::to_string(type) + '/' + name;

  void* module = dlopen(path.c_str(), RTLD_LAZY);

  if ( module == nullptr ) {
    std::cerr << "module not found at: " << path << '\n';
    throw std::runtime_error("Module Not Found");
  }

  auto func = reinterpret_cast<make_module>(dlsym(module, "create_module"));
  auto configFunc =
      reinterpret_cast<config_module>(dlsym(module, "setup_module"));

  if ( func == nullptr ) {
    std::cerr << "create_module not found in: " << path << '\n';
    throw std::runtime_error("Module Not Found");
  }

  if ( configFunc == nullptr ) {
    std::cerr << "setup_module not found in: " << path << '\n';
    throw std::runtime_error("Module Not Found");
  }

  std::unique_ptr<mod_t> mod = func();

  return {std::move(mod),
          std::function<void(void*, Parameters&, VIPRA::Random::Engine&)>(
              configFunc)};
}

}  // namespace VIPRA