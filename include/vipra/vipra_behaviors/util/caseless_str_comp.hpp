#pragma once

#include <cstring>
#include <string>

namespace VIPRA::Behaviors {

/**
 * @brief Struct for caseless look up in a std::map
 * 
 */
struct CaselessStrCompare {
  struct Comp {
    auto operator()(std::string const& str1,
                    std::string const& str2) const -> bool
    {
      const size_t cnt = str1.size();
      if ( cnt != str2.size() ) {
        return false;
      }

      for ( size_t i = 0; i < cnt; ++i ) {
        if ( str1[i] == str2[i] ) {
          continue;
        }

        if ( str1[i] > str2[i] ) {
          if ( str1[i] - SPACE_ASCII != str2[i] ) {
            return false;
          }
        }
        else {
          if ( str2[i] - SPACE_ASCII != str1[i] ) {
            return false;
          }
        }
      }
      return true;
    }
  };
  struct Hash {
    auto operator()(std::string const& str) const -> size_t
    {
      std::string temp{str};
      for ( char& chr : temp ) {
        chr = static_cast<char>(std::tolower(chr));
      }
      return std::hash<std::string>{}(temp);
    }
  };

 private:
  static constexpr char SPACE_ASCII = 32;
};

}  // namespace VIPRA::Behaviors