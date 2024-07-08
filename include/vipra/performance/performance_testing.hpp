#pragma once

#include <cassert>
#include <chrono>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "vipra/util/clock.hpp"

namespace VIPRA {

#ifndef VIPRA_PERF_TESTING

class Perf {
 public:
  static void add_call(std::string_view /*unused*/) {}
  static void start(std::string_view /*unused*/) {}
  static void stop(std::string_view /*unused*/) {}
  static auto report() -> std::string { return ""; }

  class FunctionTimer {};
};

#else

class Perf {
 public:
  /**
   * @brief Records a call to the function name
   * 
   * @param name 
   */
  static void add_call(std::string_view name) noexcept
  {
    if ( ! std::is_constant_evaluated() ) {
      if ( get_calls().find(name) == get_calls().end() ) {
        get_calls().emplace(std::string(name), 0);
      }
      (get_calls().find(name))->second++;
    }
  }

  /**
   * @brief Starts a timer for the function name
   * 
   * @param name 
   */
  static void start(std::string_view name) noexcept
  {
    if ( ! std::is_constant_evaluated() ) {
      if ( get_clocks().find(name) == get_clocks().end() ) {
        get_clocks().emplace(std::string(name), Util::Clock<Util::micro>{});
      }

      (get_clocks().find(name))->second.start();
    }
  }

  /**
   * @brief Stops the timer for the function name, and records the time
   * 
   * @param name 
   */
  static void stop(std::string_view name) noexcept
  {
    if ( ! std::is_constant_evaluated() ) {
      auto time = (get_clocks().find(name))->second.stop();
      if ( get_timings().find(name) == get_timings().end() ) {
        get_timings().emplace(std::string(name), Util::micro{0});
      }

      (get_timings().find(name))->second += time;
    }
  }

  /**
   * @brief Returns a report of the function calls and timings
   * 
   * @return std::string 
   */
  static auto report() -> std::string
  {
    std::string report{"\n\nFunction Call Counts:\n"};

    auto sortedCalls = sort_calls();
    for ( auto const& [name, count] : sortedCalls ) {
      report += "\t" + name + ": " + std::to_string(count) + " calls\n";
    }

    report += "\n\nTimings:\n";

    auto sortedTimings = sort_timings();
    for ( auto const& [name, time] : sortedTimings ) {
      auto percall = time / get_calls().at(name);
      auto percallMilli = std::chrono::duration_cast<Util::milli>(percall);
      percall -= percallMilli;
      auto percallMicro = std::chrono::duration_cast<Util::micro>(percall);

      auto copy = time;
      auto seconds = std::chrono::duration_cast<Util::seconds>(copy);
      copy -= seconds;
      auto milli = std::chrono::duration_cast<Util::milli>(copy);
      copy -= milli;
      auto micro = std::chrono::duration_cast<Util::micro>(copy);

      report += "\t" + name + ":\n\t\tTotal Time: " + std::to_string(seconds.count()) + "s " +
                std::to_string(milli.count()) + "ms " + std::to_string(micro.count()) + "us\n";
      report += "\t\tPer Call: " + std::to_string(percallMilli.count()) + "ms " +
                std::to_string(percallMicro.count()) + "us\n";
    }
    return report;
    return "";
  }

  /**
   * @brief A RAII class to time a function
   * 
   */
  class FunctionTimer {
   public:
    FunctionTimer(FunctionTimer const&) = delete;
    FunctionTimer(FunctionTimer&&) noexcept = delete;
    auto operator=(FunctionTimer const&) -> FunctionTimer& = delete;
    auto operator=(FunctionTimer&&) -> FunctionTimer& = delete;
    explicit FunctionTimer(std::string_view name) noexcept : _name(name)
    {
      Perf::add_call(_name);
      Perf::start(_name);
    }

    ~FunctionTimer() noexcept { Perf::stop(_name); }

   private:
    std::string_view _name;
  };

 private:
  [[nodiscard]] static auto get_calls() -> std::map<std::string, size_t, std::less<>>&
  {
    static std::map<std::string, size_t, std::less<>> calls;
    return calls;
  }

  [[nodiscard]] static auto get_timings() -> std::map<std::string, Util::micro, std::less<>>&
  {
    static std::map<std::string, Util::micro, std::less<>> timings;
    return timings;
  }

  [[nodiscard]] static auto get_clocks() -> std::map<std::string, Util::Clock<Util::micro>, std::less<>>&
  {
    static std::map<std::string, Util::Clock<Util::micro>, std::less<>> timings;
    return timings;
  }

  [[nodiscard]] static auto sort_calls() -> std::vector<std::pair<std::string, size_t>>
  {
    const auto cmp = [](auto const& left, auto const& right) -> bool { return left.second > right.second; };

    std::vector<std::pair<std::string, size_t>> retVal;
    for ( auto const& val : get_calls() ) {
      retVal.emplace_back(val);
    }

    std::sort(retVal.begin(), retVal.end(), cmp);

    return retVal;
  }

  [[nodiscard]] static auto sort_timings() -> std::vector<std::pair<std::string, Util::micro>>
  {
    const auto cmp = [](auto const& left, auto const& right) -> bool { return left.second > right.second; };

    std::vector<std::pair<std::string, Util::micro>> retVal;
    for ( auto const& val : get_timings() ) {
      retVal.emplace_back(val);
    }

    std::sort(retVal.begin(), retVal.end(), cmp);

    return retVal;
  }
};

#endif
}  // namespace VIPRA