template<size_t N>
constexpr BitArray<N> BitArray<N>::operator>>=(int const n)
{
    // use the left shift operator if we're shifting right by a negative amount
    if (n < 0)
        return *this << -n;
    // skip denotes the number of indexes in the array that we're shifting 
    // *this
    size_t const skip = n / 64;
    // shift denotes the size of the shift after we've selected the right 
    // index. For example, if we're shifting right a BitArray with 64-bit words
    // by 77, we only need to shift each wor=d by 13.
    size_t const shift = n % 64;
    // work from right to left starting at skip to make sure that reading and
    // writing don't interfere with eachother. Note that index may overshoot
    // d_length, so it is ill-advised to use `!=` instead of `<`.
    for (size_t index = skip; index < d_length; ++index)
    {
        size_t const target = index - skip;
        d_data[target] = (d_data[index] >> shift);
        // if we're not in the leftmost word, we need to include carry from the
        // word to the left
        if (index != d_length - 1 and shift != 0)
            // if `shift` is zero, `WORD_LENGTH - shift` is too large for the 
            // shift operator, and it becomes UB, instead of all zeros, so we
            // check for this
            d_data[target] |= (d_data[index + 1] << (WORD_LENGTH - shift));
    }
    // now we need to to clear leftnost n / WORD_LENGTH WORDS
    for (size_t index = d_length; index-- != d_length - n / WORD_LENGTH;)
        d_data[index] = 0;

    return *this;
}
