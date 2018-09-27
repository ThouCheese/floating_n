template<size_t N>
template<class... T>
constexpr BitArray<N>::BitArray(T... numbers) : d_data{numbers...} {}
