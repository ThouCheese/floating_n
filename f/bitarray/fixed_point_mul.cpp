/// Performs floating point multiplication with the period to the left of
/// `PERIOD`, but assums that everything to the left of `PERIOD` is set to 
/// zero. This allows for some optimizations, and is used by `fixed_point_mul`
template<size_t N, size_t PERIOD>
BitArray<N> floating_point_mul_internal(BitArray<N> const a,
                                        BitArray<N> const b)
{
    auto hi = [](BitArray<N> num)
        { return num >> (PERIOD / 2); };
    auto lo = [](BitArray<N> num)
        { return num & BitArray<N>::with_ones(0, PERIOD / 2); };
    auto result = hi(a) * hi(b) + hi(hi(a) * lo(b) + lo(a) * hi(a));
    return result;
}

/// Performs floating point multiplication with the period to the left of 
/// `PERIOD`. 
template<size_t N>
template<size_t PERIOD>
BitArray<N> BitArray<N>::fixed_point_mul(BitArray<N> const other) const
{
    static_assert(N > PERIOD);
    // if we assume that the comma is between `PERIOD` and `PERIOD + 1`, we can
    // get the part before the comma by shifting right by `PERIOD` and the part
    // after the comma by performing bitwise and by `PERIOD` ones and then all 
    // zeros.
    BitArray<N> this_int = *this >> PERIOD;
    BitArray<N> this_flp = *this & BitArray<N>::with_ones(0, PERIOD);
    BitArray<N> other_int = other >> PERIOD;
    BitArray<N> other_flp = other & BitArray<N>::with_ones(0, PERIOD);

    return (this_int * other_int << PERIOD) + this_int * other_flp + 
        this_flp * other_int + 
        floating_point_mul_internal<N, PERIOD>(this_flp, other_flp);
}
