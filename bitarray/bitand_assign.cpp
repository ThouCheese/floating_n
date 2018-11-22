template<size_t N>
constexpr BitArray<N> BitArray<N>::operator&=(BitArray<N> const other)
{
    for (size_t index = 0; index != s_length; ++index)
        d_data[index] = d_data[index] & other.d_data[index];
    return *this;
}
