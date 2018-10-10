/// Performs floating point multiplication with the period to the left of
/// `P`, but assums that everything to the left of `P` is set to 
/// zero. This allows for some optimizations, and is used by `fixed_point_mul`
template<size_t N, size_t P>
BitArray<N> constexpr floating_point_mul_internal(BitArray<N> const a,
                                                  BitArray<N> const b)
{
    auto hi = [](BitArray<N> num)
        { return num >> (P / 2); };
    auto lo = [](BitArray<N> num)
        { return num & BitArray<N>::with_ones(0, P / 2); };
    auto result = hi(a) * hi(b) + hi(hi(a) * lo(b) + lo(a) * hi(a));
    return result;
}

/// Performs floating point multiplication with the period to the left of 
/// `P`. 
// template<size_t N>
// template<size_t P>
// BitArray<N> BitArray<N>::fixed_point_mul(BitArray<N> const other) const
// {
//     static_assert(N > P);
//     // if we assume that the comma is between `P` and `P + 1`, we can
//     // get the part before the comma by shifting right by `P` and the part
//     // after the comma by performing bitwise and by `P` ones and then all 
//     // zeros.
    
// }

template<size_t N, size_t P>
FixedPoint<N, P> constexpr FixedPoint<N, P>::operator*=(FixedPoint<N, P> const other)
{
    BitArray<N> this_int = *this >> P;
    BitArray<N> this_flp = *this & BitArray<N>::with_ones(0, P);
    BitArray<N> other_int = other >> P;
    BitArray<N> other_flp = other & BitArray<N>::with_ones(0, P);

    return FixedPoint<N, P>((this_int * other_int << P) + this_int * other_flp + 
        this_flp * other_int + 
        floating_point_mul_internal<N, P>(this_flp, other_flp));
}