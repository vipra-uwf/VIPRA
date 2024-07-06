#pragma once

namespace VIPRA::Util {

template <typename class_t>
class CRTP;

template <template <class> class mixin_t, class derived_t>
struct CRTP<mixin_t<derived_t>> {
 protected:
  consteval auto self() -> derived_t& { return static_cast<derived_t&>(*this); }
  consteval auto self() const -> derived_t const& { return static_cast<derived_t const&>(*this); }
};

}  // namespace VIPRA::Util