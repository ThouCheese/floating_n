/// returns a `BitArray<N>` with all ones between the indexes, counting from
/// right to left.
template<size_t N>
constexpr BitArray<N> BitArray<N>::with_ones(size_t const start,
                                             size_t const end)
{
    // todo: make this compil
    // static_assert(start < end);
    // this function is slow, but it is only used in compile time. If 
    // compilation times become excessive, look here
    BitArray<N> result;
    for (size_t index = start; index != end; ++index)
    {
        size_t word_index = index % WORD_LENGTH;
        size_t word_num = index / WORD_LENGTH;
        result.d_data[word_num] |= static_cast<WORD>(1) << word_index;
    }
    return result;
}
