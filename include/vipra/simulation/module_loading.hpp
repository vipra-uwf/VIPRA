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
struct LoadedModule {
  std::unique_ptr<std::remove_cvref_t<module_t>>                  module;
  std::function<void(void*, Parameters&, VIPRA::Random::Engine&)> config_module;
};

template <typename module_t>
inline auto load_module(std::string const& name, std::string const& installDir,
                        Modules::Type type) -> LoadedModule<module_t>
{
  using mod_t = std::remove_cvref_t<module_t>;
  using make_module_t = mod_t* (*)();
  using config_module_t = void (*)(void*, VIPRA::Parameters&, VIPRA::Random::Engine&);
  // using config_module_t = void (*)(mod_t*, VIPRA::Parameters&, VIPRA::Random::Engine&);

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
    throw std::runtime_error("Unable to load module");
  }

  // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
  auto func = reinterpret_cast<make_module_t>(dlsym(module, "create_module"));
  auto configFunc = reinterpret_cast<config_module_t>(dlsym(module, "setup_module"));
  // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

  if ( func == nullptr ) {
    std::cerr << "create_module not found in: " << path << '\n';
    throw std::runtime_error("Module Missing VIPRA_REGISTER_MODULE");
  }

  if ( configFunc == nullptr ) {
    std::cerr << "setup_module not found in: " << path << '\n';
    throw std::runtime_error("Module Missing VIPRA_REGISTER_MODULE");
  }

  VIPRA::Log::debug("Creating Module");

  std::unique_ptr<mod_t> mod;
  try {
    mod.reset(func());
  }
  catch ( ... ) {
    std::cerr << "Module not created, Error thrown in Module Construction\n";
    throw std::runtime_error("Unable to create Module");
  }

  if ( mod.get() == nullptr ) {
    std::cerr << "Module not created\n";
    throw std::runtime_error("Unable to create Module");
  }

  VIPRA::Log::debug("Returning Module");

  return LoadedModule<module_t>{
      std::move(mod),
      std::function<void(void*, Parameters&, VIPRA::Random::Engine&)>(configFunc)};
      // wrap the module-specific setup function (which expects mod_t*) into the
      // generic std::function that callers use (void*), performing the proper cast
      // back to mod_t* before invoking the module setup.
      // std::function<void(void*, Parameters&, VIPRA::Random::Engine&)>(
      //     [configFunc](void* m, Parameters& p, VIPRA::Random::Engine& e) {
      //       configFunc(static_cast<mod_t*>(m), p, e);
      //     })};
}

}  // namespace VIPRA