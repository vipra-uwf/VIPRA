#pragma once

#include <cstdint>
#include <map>
#include <set>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <vector>
#include "vipra/concepts/numeric.hpp"

namespace VIPRA {
enum ArgType : uint8_t {
  OPTIONAL = 0x00,
  REQUIRED = 0x01,
  VALUE_REQUIRED = 0x02,
  BOTH = 0x03
};

inline constexpr auto operator|(ArgType Lhs, ArgType Rhs) -> ArgType
{
  return static_cast<ArgType>(static_cast<std::underlying_type_t<ArgType>>(Lhs) |
                              static_cast<std::underlying_type_t<ArgType>>(Rhs));
}

inline constexpr auto operator&(ArgType Lhs, ArgType Rhs) -> ArgType
{
  return static_cast<ArgType>(static_cast<std::underlying_type_t<ArgType>>(Lhs) &
                              static_cast<std::underlying_type_t<ArgType>>(Rhs));
}

inline constexpr auto operator==(ArgType Lhs, ArgType Rhs) -> bool
{
  return static_cast<std::underlying_type_t<ArgType>>(Lhs) ==
         static_cast<std::underlying_type_t<ArgType>>(Rhs);
}

inline constexpr auto operator!=(ArgType Lhs, ArgType Rhs) -> bool
{
  return static_cast<std::underlying_type_t<ArgType>>(Lhs) !=
         static_cast<std::underlying_type_t<ArgType>>(Rhs);
}

class Args {
 private:
  using ArgsMap = std::map<std::string, std::string, std::less<>>;
  using ArgsSet = std::set<std::string, std::less<>>;

 public:
  /**
   * @brief Parses out the arguments passed into the program
   * 
   * @param argv 
   */
  static void parse(int argc, const char* const* argv)
  {
    get_args().clear();
    auto args = format(argc, argv);

    for ( auto arg = std::next(args.begin()); arg != args.end(); ++arg ) {
      if ( arg->length() < 2 ) throw std::runtime_error("Unknown Flag: " + *arg);
      if ( (*arg)[0] != '-' ) throw std::runtime_error("Unknown Flag: " + *arg);

      auto [flag, val] = split_arg(*arg);

      auto [fullValid, fullError] = validate_flag(flag, val);
      if ( fullValid ) {
        get_args()[flag] = val;
        continue;
      }

      auto [compositeValid, compositeError] = validate_composite_flag(flag);
      if ( compositeValid ) {
        continue;
      }

      throw std::runtime_error(fullError + ": " += flag);
    }

    check_required();
  }
  /**
   * @brief Adds an argument flag that can be accepted
   * 
   * @param key : argument key to add
   */
  static void register_arg(std::string const& key)
  {
    get_arg_map()[key] = ArgType::OPTIONAL;
  }

  /**
   * @brief Adds an argument flag that can be accepted, and sets its type
   * 
   * @param key : argument key to add
   * @param type : argument type
   */
  static void register_arg(std::string const& key, ArgType type)
  {
    if ( type & ArgType::REQUIRED ) get_req_set().insert(key);
    get_arg_map()[key] = type;
  }

  /**
   * @brief Adds an argument flag that can be accepted, sets its type and provides a default value
   * 
   * @param key : argument key to add
   * @param type : argument type
   */
  static void register_arg(std::string const& key, std::string const& defaultValue,
                           ArgType type)
  {
    if ( type & ArgType::REQUIRED ) get_req_set().insert(key);
    get_arg_map()[key] = type;
    get_defaults()[key] = defaultValue;
  }

  /**
   * @brief Returns true if the argument is present
   * 
   * @param key : argument key
   * @return true 
   * @return false 
   */
  static auto has(std::string_view key) -> bool
  {
    if ( get_defaults().find(key) != get_defaults().end() ) {
      return true;
    }

    return get_args().find(key) != get_args().end();
  }

  /**
   * @brief Returns the argument with the given key
   * 
   * @param key : argument key
   * @return std::string const& : argument value
   */
  template <typename val_t = std::string>
  [[nodiscard]] static auto get(std::string_view key) -> val_t
  {
    auto iter = get_args().find(key);
    if ( iter == get_args().end() ) {
      iter = get_defaults().find(key);
      if ( iter == get_defaults().end() )
        throw std::out_of_range("Attempt To Access Missing Argument:" + std::string(key));
    }

    if constexpr ( Concepts::Numeric<val_t> ) {
      return static_cast<val_t>(std::stof(iter->second));
    }
    else {
      return iter->second;
    }
  }

  /**
   * @brief Returns the number of arguments
   * 
   * @return size_t : argument count
   */
  static auto count() -> size_t { return get_args().size(); }

  /**
   * @brief Clears the registered arguments
   * 
   */
  static void reset()
  {
    get_arg_map().clear();
    get_req_set().clear();
  }

 private:
  /**
   * @brief Splits an argument into its key/value
   * 
   * @param arg : argument to split
   * @return std::pair<std::string, std::string> : arguments key/value
   */
  static auto split_arg(std::string_view arg) -> std::pair<std::string, std::string>
  {
    auto loc = arg.find('=');
    if ( loc == std::string::npos )
      return std::make_pair(std::string(arg.begin() + 1), "");

    return std::make_pair(std::string(arg.begin() + 1, loc - 1),
                          std::string(arg.begin() + loc + 1, arg.length() - (loc + 1)));
  }

  static auto split_single_letter_args(std::string_view arg) -> std::vector<std::string>
  {
    std::vector<std::string> args;
    for ( auto chr : arg ) {
      args.emplace_back(1, chr);
    }
    return args;
  }

  /**
   * @brief Checks if a flag is a composite flag
   * 
   * @param arg 
   * @return std::pair<bool, std::string> 
   */
  static auto validate_composite_flag(std::string const& arg)
      -> std::pair<bool, std::string>
  {
    auto flags = split_single_letter_args(arg);
    for ( const auto& flag : flags ) {
      auto valid = validate_flag(flag, "");
      if ( ! valid.first ) return valid;
      get_args()[flag] = "";
    }

    return {true, ""};
  }

  /**
   * @brief Checks if a flag is valid
   * 
   * @param flag : flag to check
   */
  static auto validate_flag(std::string_view flag,
                            std::string_view value) -> std::pair<bool, std::string>
  {
    auto& argSet = get_arg_map();
    auto  iter = argSet.find(flag);
    if ( iter == argSet.end() ) return {false, "Unknown Flag"};

    auto type = iter->second;

    if ( (type & ArgType::VALUE_REQUIRED) == ArgType::VALUE_REQUIRED ) {
      if ( value.empty() ) return {false, "Required Value Missing"};
    }

    return {true, ""};
  }

  /**
   * @brief Checks that all required flags are set
   * 
   * @param args : arguments map
   */
  static void check_required()
  {
    for ( const auto& arg : get_req_set() ) {
      if ( ! has(arg) ) {
        throw std::runtime_error("Missing Flag: " + arg);
      }
    }
  }

  /**
   * @brief Converts argc/argv into a vector of strings
   * 
   * @param argc : command line argument count
   * @param argv : command line arguemnts
   * @return std::vector<std::string> 
   */
  static auto format(int argc, const char* const* argv) -> std::vector<std::string>
  {
    return std::vector<std::string>{argv,
                                    std::next(argv, static_cast<std::ptrdiff_t>(argc))};
  }

  static inline auto get_arg_map() -> std::map<std::string, ArgType, std::less<>>&
  {
    static std::map<std::string, ArgType, std::less<>> argSet;
    return argSet;
  }

  static inline auto get_req_set() -> ArgsSet&
  {
    static ArgsSet reqSet;
    return reqSet;
  }

  static inline auto get_args() -> ArgsMap&
  {
    static ArgsMap args;
    return args;
  }

  static inline auto get_defaults() -> ArgsMap&
  {
    static ArgsMap defaults{};
    return defaults;
  }

 public:
  Args() = delete;
};
}  // namespace VIPRA