#include "tuple.hpp"
#include <array>
#include <iostream>

auto main() -> int {
  auto test = tuple<int, bool, std::string_view>(1, false, "hi");

  std::cout << get<2>(test) << '\n';
}
