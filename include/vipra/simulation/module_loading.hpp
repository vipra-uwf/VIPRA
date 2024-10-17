#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>

#include <dlfcn.h>

#include "vipra/logging/logging.hpp"
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

  typedef mod_t* (*make_module)();
  typedef void (*config_module)(void*, VIPRA::Parameters&,
                                VIPRA::Random::Engine&);

  std::string path;

  if ( type == Modules::Type::PedInput || type == Modules::Type::MapInput )
    path = installDir + "/input/lib" + name + ".so";
  else
    path = installDir + '/' + Modules::to_string(type) + "/lib" + name + ".so";

  if ( ! std::filesystem::exists(path) ) {
    VIPRA::Log::error("No Module {} at {}", name, path);
    throw std::runtime_error("Module Not Found");
  }

  VIPRA::Log::debug("Loading Module: {} at {}", name, path);

  void* module = dlopen(path.c_str(), RTLD_LAZY);

  if ( module == nullptr ) {
    std::cerr << "module not found at: " << path << '\n';
    VIPRA::Log::error("dlopen failed: {}\n", dlerror());
    throw std::runtime_error("Module Not Found");
  }

  // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
  auto func = reinterpret_cast<make_module>(dlsym(module, "create_module"));
  auto configFunc =
      reinterpret_cast<config_module>(dlsym(module, "setup_module"));
  // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

  if ( func == nullptr ) {
    std::cerr << "create_module not found in: " << path << '\n';
    VIPRA::Log::error("dlopen failed: {}\n", dlerror());
    throw std::runtime_error("Module Missing VIPRA_REGISTER_MODULE");
  }

  if ( configFunc == nullptr ) {
    std::cerr << "setup_module not found in: " << path << '\n';
    VIPRA::Log::error("dlopen failed: {}\n", dlerror());
    throw std::runtime_error("Module Missing VIPRA_REGISTER_MODULE");
  }

  VIPRA::Log::debug("Creating Module");

  std::unique_ptr<mod_t> mod(func());

  if ( mod.get() == nullptr ) {
    std::cerr << "Module not created\n";
    throw std::runtime_error("Unable to reate Module");
  }

  VIPRA::Log::debug("Returning Module");

  return {std::move(mod),
          std::function<void(void*, Parameters&, VIPRA::Random::Engine&)>(
              configFunc)};
}

}  // namespace VIPRA