template<size_t WORD_LENGTH>
constexpr std::pair<WORD, WORD> mul_with_carry(WORD const a, WORD const b)
{
    auto lo = [&](WORD const number) 
        { return number & (static_cast<WORD>(-1) >> WORD_LENGTH / 2); };
    auto hi = [&](WORD const number) 
        { return number >> WORD_LENGTH / 2; };
    // use the wrapping behaviour of the normal overflow of unsigned numbers
    // to compute the result without too much involvement
    WORD result = a * b;
    // complex mathematics to compute the carry, I have a visual proof, email
    // me or don't, i'm a comment, not the police
    WORD carry = hi(a) * hi(b) + hi(hi(a) * lo(b) + lo(a) * hi(b));
    return std::pair<WORD, WORD>(carry, result);
}

template<size_t N>
constexpr BitArray<N> BitArray<N>::operator*(BitArray<N> const other) const
{
    // todo: as it stands this function prevents overflow by refusing to read
    // and write bits that are to significant and that may thus cause an 
    // overflow. It will produce incorrect results in the case of an overflow
    // by truncating the answer's most significant bits.
    // todo 2: the loop uses a temporary BitArray to insert the computed values,
    // so that it automatically handles carrying. This ~might~ be slow.

    // to perform this function we need to compute the carry of the 
    // multiplication of two 64-bit words. This is covered by the mul_with_carry
    // function
    auto result = BitArray<N>();

    // O(n^c), (1 < c < 2) which is pretty bad :(
    for (size_t self_index = 0; self_index != s_length; ++self_index)
        for (size_t other_index = 0; other_index != s_length; ++other_index)
        {
            if (self_index + other_index >= s_length)
                break;
            auto pair = mul_with_carry<WORD_LENGTH>(
                this->d_data[self_index], other.d_data[other_index]
            );
            BitArray<N> temp = BitArray<N>();
            temp.d_data[self_index + other_index] = pair.second;
            // prevent a segfault when writing the carry
            if (self_index + other_index + 1 < s_length)
                temp.d_data[self_index + other_index + 1] = pair.first;
            result = result + temp;
        }
    return result;
}
