template<size_t M, size_t E>
F<M, E> F<M, E>::operator*=(F<M, E> const other)
{
    if (UNLIKELY(*this == NAN or other == NAN))
        return NAN;
    else if (this->abs() == ZERO or other.abs() == ZERO)
        return this->get_sign() == other.get_sign() ? ZERO : NEG_ZERO;
    else if (this->abs() == INF or other.abs() == INF)
        return this->get_sign() == other.get_sign() ? INF : NEG_INF;
    // we compute the product of the two mantissa's
    BitArray<s_bits> man =
        floating_point_mul_internal<s_bits, M>(this->get_man(), other.get_man())
            + this->get_man() + other.get_man() + s_two_to_the_M;
    // we need to shift the result (left or right) until the leading 1 is at
    // position M.
    WORD overflow = E - man.leading_zeros();
    man >>= overflow;
    // since the leading bit is implicit, we remove it
    man -= s_two_to_the_M;
    // the new exponent is the sum of the other two
    BitArray<s_bits> exp = this->get_exp() + other.get_exp() - s_exponent_bias;
    // now we adjust the exponent according to how much the mantissa overflowed
    if (overflow > 0)
        exp += BitArray<s_bits>(static_cast<WORD>(overflow));
    else
        exp -= BitArray<s_bits>(static_cast<WORD>(-overflow));
    BitArray<s_bits> sign{
        static_cast<WORD>(this->get_sign() ^ other.get_sign() ? 0 : 1)
    };
    return this->d_data = (sign << M + E) | (exp << M) | man;
}
