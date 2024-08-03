
#pragma once

#include <iostream>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/ranges.h>

namespace VIPRA {

class Log {
 public:
  enum class Level {
    DEBUG,
    INFO,
    WARN,
    ERROR,
  };

  static void set_level(Level lvl) { level = lvl; }

  /**
 * @brief Calls the provided Logger with Level WARN
 *
 * @tparam params - Variadic Parameters
 * @param message - Message to pass to logger
 * @param ps - Any additional parameters for the logger
 */
  template <typename... param_ts>
  inline static void warn(fmt::format_string<param_ts...> message, param_ts&&... params)
  {
    if ( level <= Level::WARN ) {
      std::cout << "[WARN] " << fmt::format(message, std::forward<param_ts>(params)...) << '\n';
    }
  }

  /**
 * @brief Calls the provided Logger with Level INFO
 *
 * @tparam params - Variadic Parameters
 * @param message - Message to pass to logger
 * @param ps - Any additional parameters for the logger
 */
  template <typename... param_ts>
  inline static void info(fmt::format_string<param_ts...> message, param_ts&&... params)
  {
    if ( level <= Level::INFO ) {
      std::cout << "[INFO] " << fmt::format(message, std::forward<param_ts>(params)...) << '\n';
    }
  }

  /**
 * @brief Calls the provided Logger with Level DEBUG
 *
 * @tparam params - Variadic Parameters
 * @param message - Message to pass to logger
 * @param ps - Any additional parameters for the logger
 */
  template <typename... param_ts>
  inline static void debug(fmt::format_string<param_ts...> message, param_ts&&... params)
  {
    if ( level <= Level::DEBUG ) {
      std::cout << "[DEBUG] " << fmt::format(message, std::forward<param_ts>(params)...) << '\n';
    }
  }

  /**
 * @brief Calls the provided Logger with Level DEBUG
 *
 * @tparam params - Variadic Parameters
 * @param message - Message to pass to logger
 * @param ps - Any additional parameters for the logger
 */
  template <typename... param_ts>
  inline static void error(fmt::format_string<param_ts...> message, param_ts&&... params)
  {
    std::cout << "[ERROR] " << fmt::format(message, std::forward<param_ts>(params)...) << '\n';
  }

 private:
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
  static Level level;
};
}  // namespace VIPRA
