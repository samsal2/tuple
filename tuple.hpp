#include <type_traits>

template <std::size_t i, typename T> class tuple_leaf {
public:
  using value_type = T;

  tuple_leaf() = default;
  template <typename U> tuple_leaf(U &&val) : val_(std::forward<U>(val)) {}

  auto get() noexcept -> T & { return val_; }

private:
  T val_;
};

template <std::size_t i, typename... Ts> struct tuple_impl;

template <std::size_t i, typename T, typename... Ts>
struct tuple_impl<i, T, Ts...> : public tuple_leaf<i, T>,
                                 public tuple_impl<i + 1, Ts...> {
  using leaf_type = tuple_leaf<i, T>;
  using impl_type = tuple_impl<i + 1, Ts...>;
  using value_type = typename leaf_type::value_type;

  template <typename U, typename... Us>
  tuple_impl(U &&arg, Us &&...args)
      : leaf_type(std::forward<U>(arg)), impl_type(std::forward<Us>(args)...) {}
};

template <std::size_t i> struct tuple_impl<i> {};

template <typename... Ts> struct tuple : public tuple_impl<0, Ts...> {
  using tuple_impl<0, Ts...>::tuple_impl;
};

template <std::size_t i, typename... Ts> struct element_impl;

template <std::size_t i, typename T, typename... Ts>
struct element_impl<i, T, Ts...> {
  using type = typename element_impl<i - 1, Ts...>::type;
};

template <typename T, typename... Ts> struct element_impl<0, T, Ts...> {
  using type = T;
};

template <std::size_t i, typename... Ts> struct tuple_element;

template <std::size_t i, typename... Ts> struct tuple_element<i, tuple<Ts...>> {
  using type = typename element_impl<i, Ts...>::type;
};

template <std::size_t i, typename Tuple>
auto constexpr get(Tuple &val) noexcept ->
    typename tuple_element<i, Tuple>::type & {
  return static_cast<tuple_leaf<i, typename tuple_element<i, Tuple>::type> &>(
             val)
      .get();
}
