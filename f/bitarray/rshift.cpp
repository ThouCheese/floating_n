template<size_t N>
constexpr BitArray<N> BitArray<N>::operator>>(int const n) const
{
    return BitArray<N>{*this} >>= n;
}
