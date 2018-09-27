template<size_t N>
long double BitArray<N>::to_ld() const
{
    long double result = 0;
    for (size_t index = 0; index != d_length; ++index)
        result += static_cast<long double>(d_data[index]) * pow(2L, 64 * index);
    return result;
}