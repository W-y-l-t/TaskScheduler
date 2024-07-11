#include <iostream>

template <typename Head, typename... Tail>
class MyTuple {
 public:
    explicit MyTuple(const Head& head,
                     const Tail&... tail) : head_(head), tail_(tail...) {}

    [[nodiscard]] constexpr int size() const {
        return tail_.size() + 1;
    }

    Head head_;
    MyTuple<Tail...> tail_;
};

template <typename Head>
class MyTuple<Head> {
 public:
    explicit MyTuple(const Head& head) : head_(head) {}

    [[nodiscard]] constexpr int size() const {
        return 1;
    }

    Head head_;
};

template <size_t index, typename Head, typename... Tail>
struct NthType : NthType<index - 1, Tail...> {
    static_assert(index <= sizeof...(Tail), "Tuple's index out of bounds");
};

template <typename Head, typename... Tail>
class NthType<0, Head, Tail...> {
 public:
    Head head_;
};

template <size_t index>
class getter {
 public:
    template <typename... Tail>
    static decltype(NthType<index, Tail...>::head_)& get(MyTuple<Tail...>& tuple) {
        return getter<index - 1>::get(tuple.tail_);
    }
};

template <>
class getter<0> {
 public:
    template <typename Head, typename... Tail>
    static Head& get(MyTuple<Head, Tail...>& tuple) {
        return tuple.head_;
    }
};

template <size_t index, typename... Tail>
decltype(NthType<index, Tail...>::head_)& get(MyTuple<Tail...>& tuple) {
    return getter<index>::get(tuple);
}

template <typename Func, typename... Args, size_t... Indices>
auto apply_helper(Func&& func, MyTuple<Args...>& tuple, std::index_sequence<Indices...>) {
    return func(get<Indices>(tuple)...);
}

template <typename Func, typename... Args>
auto apply(Func&& func, MyTuple<Args...>& tuple) {
    return apply_helper(std::forward<Func>(func), tuple, std::make_index_sequence<sizeof...(Args)>{});
}
