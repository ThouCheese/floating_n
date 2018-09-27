template<size_t N>
BitArray<N> BitArray<N>::operator-(BitArray<N> const other) const
{
    return BitArray<N>{*this} -= other;
}
