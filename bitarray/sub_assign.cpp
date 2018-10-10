template<size_t N>
constexpr BitArray<N> BitArray<N>::operator-=(BitArray<N> const other)
{
    bool carry = false;
    for (size_t index = 0; index != d_length; ++index)
    {
        carry = __builtin_sub_overflow(d_data[index], carry, d_data + index);
        carry += __builtin_sub_overflow(d_data[index], other.d_data[index],
                                        d_data + index);
    }
    return *this;
}
