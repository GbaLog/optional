#include <iostream>
#include "optional.hpp"

struct OptionalTest
{
  OptionalTest() : n(0) {}
  OptionalTest(int n) : n(n) {}
  OptionalTest(const OptionalTest & opt) : n(opt.n) {}
  OptionalTest(OptionalTest && opt) : n(opt.n) { opt.n = 0; }
  OptionalTest & operator =(const OptionalTest & opt) { n = opt.n; return *this; }
  OptionalTest & operator =(OptionalTest && opt) { n = opt.n; opt.n = 0; return *this; }
  ~OptionalTest() { n = 0; }

  int n;
};

int main(int argc, char *argv[])
{
  std::cout << "Hello World!\n";

  glib::optional<int> opt(321);

  std::cout << std::move(*opt) << "\n";

  auto opt2 = glib::make_optional<std::string>("kek");

  std::cout << opt2->size() << "\n";

  try
  {
    auto opt = glib::make_optional<int>();

    std::cout << opt.value() << "\n";
  }
  catch(glib::optional_exception & ex)
  {
    std::cerr << ex.what() << "\n";
  }

  auto opt3 = glib::make_optional<int>();

  opt3.swap(opt);

  //std::cout << opt.value() << "\n";

  auto optTest = glib::make_optional<OptionalTest>();
  auto optTest2 = glib::make_optional<OptionalTest>(2);

  optTest.swap(optTest2);
}
