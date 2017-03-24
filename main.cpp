#include <iostream>
#include "optional.hpp"

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
}
