#ifndef GLIB_VARIANT_HPP
#define GLIB_VARIANT_HPP
//-------------------------------------------------------------------
#include <stdexcept>
//-------------------------------------------------------------------
namespace glib
{
//-------------------------------------------------------------------
struct optional_exception : public std::logic_error
{
  optional_exception() : logic_error("bad optional access") { }

  explicit optional_exception(const char* arg) : logic_error(arg) { }

  virtual ~optional_exception() noexcept = default;
};
//-------------------------------------------------------------------
template<typename T>
class optional
{
public:
  constexpr optional(const T & val)
    :
    val_(val),
    valid_(true)
  {}

  constexpr optional(T && val)
    :
    val_(std::move(val)),
    valid_(true)
  {}

  constexpr optional()
    :
    valid_(false)
  {}

  ~optional()
  {
    if (std::is_trivially_destructible<T>::value)
      ;
  }

  constexpr operator bool() const
  {
    return valid_;
  }

  constexpr bool is_valid() const
  {
    return valid_;
  }

  constexpr T & value()
  {
    if (!valid_)
      throw optional_exception("No valid value was be get.");

    return val_;
  }

  constexpr const T & value() const
  {
    return value();
  }

  template<typename U>
  constexpr T value_or(U && other) const &
  {
    return bool(*this) ? val_ : static_cast<T>(std::forward<U>(other));
  }

  template<typename TpOther>
  constexpr T value_or(TpOther && other) &&
  {
    return bool(*this) ? std::move(val_) : static_cast<T>(std::forward<TpOther>(other));
  }

  constexpr const T * operator ->() const
  {
    return &this->val_;
  }

  constexpr T * operator ->()
  {
    return &this->val_;
  }

  constexpr const T & operator *() const &
  {
    return this->val_;
  }

  constexpr T & operator *() &
  {
    return this->val_;
  }

  constexpr const T && operator *() const &&
  {
    return std::move(**this);
  }

  constexpr T && operator *() &&
  {
    return std::move(**this);
  }

private:
  T val_;
  bool valid_;
};
//-------------------------------------------------------------------
template<typename Tp, typename ... Args>
optional<Tp> make_optional(Args && ... args)
{
  return optional<Tp>(std::forward<Args>(args)...);
}
//-------------------------------------------------------------------
} // glib
//-------------------------------------------------------------------
#endif // GLIB_VARIANT_HPP
