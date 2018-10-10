template<size_t N>
constexpr BitArray<N> BitArray<N>::operator<<=(int const n)
{
    // undocumented stuff is the same as in `operator>>`
    if (n < 0)
        return *this >> -n;

    size_t const skip = n / 64;
    size_t const shift = n % 64;

    // each word in `this` is written from the position `index` to `index + 
    // skip`. Therefore, the first word that is actually written to somewhere 
    // the the word at then index `d_length - skip`, so we start our get_loop
    // there. We then work left to right all the way to zero.
    for (size_t index = d_length - skip; index-- != 0;)
    {
        size_t const target = index + skip;
        d_data[target] = (d_data[index] << shift);
        // we have to include the carry from the word to the right
        if (index != 0 and shift != 0)
            d_data[target] |= (d_data[index - 1] >> 
                (WORD_LENGTH - shift));
    }

    // now we need to to clear rightmost n / WORD_LENGTH WORDS
    for (size_t index = 0; index != n / WORD_LENGTH; ++index)
        d_data[index] = 0;

    // we now delete the leftmost bits that are not supposed to be remembered.
    // (eg a BitArray<100> will use two 64-bit words, but should keep all zeros
    // in the leftmost 28 bits.)
    WORD constexpr mask = 0xFFFFFFFFFFFFFFFF >> (d_length * WORD_LENGTH - N);
    d_data[d_length - 1] &= mask;
    return *this;
}
