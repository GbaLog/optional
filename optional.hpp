#ifndef GLIB_VARIANT_HPP
#define GLIB_VARIANT_HPP
//-------------------------------------------------------------------
#include <stdexcept>
#include <utility>
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

  template<typename U>
  constexpr optional(const U & val)
    :
    val_(static_cast<T>(val)),
    valid_(true)
  {}

  template<typename U>
  constexpr optional(U && val)
    :
    val_(std::move(static_cast<T>(val))),
    valid_(true)
  {}

  template<typename U>
  optional<T> & operator =(const optional<U> & other)
  {
    if (other)
    {
      valid_ = true;
      val_ = other.value();
    }

    return *this;
  }

  template<typename U>
  optional<T> & operator =(optional<U> && other)
  {
    if (other)
    {
      valid_ = true;
      val_ = std::move(*other);
      other.valid_ = false;
    }

    return *this;
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

  //TODO
  template<typename U>
  void swap(optional<U> & other) noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_constructible<U>::value)
  {
    if (!*this && !other)
      return;

    if (*this && !other)
    {
      *other = std::move(**this);
      valid_ = false;
      return;
    }

    if (!*this && other)
    {
      *this = std::move(other);
      other.valid_ = false;
      return;
    }

    using std::swap;

    swap(**this, *other);
  }

  void reset() noexcept
  {
    if (valid_)
    {
      valid_ = false;
      value().T::~T();
    }
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
