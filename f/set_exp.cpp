template<size_t M, size_t E>
constexpr F<M, E> F<M, E>::set_exp(BitArray<s_bits> const exponent)
{
    d_data &= !s_exp_mask; // set the exponent bits to zero
    d_data |= exponent << M; // insert the correct bits
    return *this;
}