template<size_t M, size_t E>
constexpr F<M, E> F<M, E>::set_man(BitArray<s_bits> const mantissa)
{
    d_data &= !s_man_mask; // set the mantissa bits to zero
    d_data |= mantissa; // insert the correct bits
    return *this;
}