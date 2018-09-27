template<size_t N>
bool BitArray<N>::operator<=(BitArray<N> const other) const
{
    return not (*this > other);
}
