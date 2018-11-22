template<size_t P>
template<size_t M, size_t E>
FixedPoint<s_bits, P> F<M, E>::to_fp()
{
    BitArray<s_bits> man = get_man();
    BitArray<s_bits> exp = get_exp();
    if (exp > s_exponent_bias)
    {
        exp -= s_exponent_bias;

    }
}