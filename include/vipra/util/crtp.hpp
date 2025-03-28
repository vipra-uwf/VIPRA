#pragma once

namespace VIPRA::Util {

template <typename class_t>
class CRTP;

template <template <typename> class class_t, class derived_t>
class CRTP<class_t<derived_t>> {
 public:
  constexpr auto self() -> derived_t& { return static_cast<derived_t&>(*this); }
  constexpr auto self() const -> derived_t const&
  {
    return static_cast<derived_t const&>(*this);
  }
};

}  // namespace VIPRA::Util