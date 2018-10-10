template<size_t N>
constexpr BitArray<N> BitArray<N>::operator~() const
{
    BitArray<N> result;
    for (size_t index = 0; index != d_length; ++index)
        result.d_data[index] = ~this->d_data[index];
    return result;
}
