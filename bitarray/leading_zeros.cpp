template<size_t N>
constexpr size_t BitArray<N>::leading_zeros() const
{
    for (size_t index = s_length; index-- != 0;)
        if (d_data[index] != 0)
            for (size_t num_index = WORD_LENGTH; num_index--;)
                if ((d_data[index] & (1ul << num_index)))
                    return N - WORD_LENGTH * index - (num_index + 1);
    return N;
}
