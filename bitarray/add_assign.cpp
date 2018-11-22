template<size_t N>
constexpr BitArray<N> BitArray<N>::operator+=(BitArray<N> const other)
{
    size_t carry = 0;
    for (size_t index = 0; index != s_length; ++index)
    {
        // first overwrite the carry by true or false, so 1 or 0, depending on
        // whether `d_data[index] + carry` overflows. Store the result at
        // `result.d_data[index]`.
        carry = __builtin_add_overflow(d_data[index], carry, d_data + index);
        carry += __builtin_add_overflow(d_data[index], other.d_data[index],
                                        d_data + index);


    }
    return *this;
}
