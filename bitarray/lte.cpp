template<size_t N>
bool constexpr BitArray<N>::operator<=(BitArray<N> const other) const
{
    return not (*this > other);
}
