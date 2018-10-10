template<size_t M, size_t E>
constexpr F<M, E> F<M, E>::operator+=(F<M, E> const other)
{
    if (*this == ZERO or *this == NEG_ZERO)
        return *this = other;
    if (other == ZERO or other == NEG_ZERO)
        return *this;
    if (this->get_sign() != other.get_sign())
    {
        if (this->get_sign())
            // this is positive, other is negative
            return *this -= -other;
        else 
            // this is negative, other is positive, compute -this - -other
            return *this = other - -*this;
    }
    // it might seem silly to restrict the exponent difference to a long, but
    // before this becomes a limiting factor, you need to assign a mantissa with
    // length of LONG_MAX_VALUE / 2, which takes 1/8 GB for the mantissa alone.
    long exponent_difference = 0;
    if (this->get_exp() > other.get_exp())
        exponent_difference = (this->get_exp() - other.get_exp()).d_data[0];
    else
        exponent_difference = -static_cast<long>(
            (other.get_exp() - this->get_exp()).d_data[0]
        );
    // we shift the value of the mantissa's one to the right
    BitArray<s_bits> this_man  = (this->get_man() + s_two_to_the_M);
    BitArray<s_bits> other_man = (other.get_man() + s_two_to_the_M);
    BitArray<s_bits> exp;
    if (exponent_difference > 0)
    {
        other_man >>= exponent_difference;
        this_man += other_man;
        exp = this->get_exp();
    }
    else if (exponent_difference < 0)
    {
        this_man >>= -exponent_difference;
        this_man += other_man;
        exp = other.get_exp();
    }
    else
    {
        this_man += other_man;
        this_man >>= 1;
        exp = this->get_exp() + BitArray<s_bits>{1ul};
    };
    size_t overflow = E - this_man.leading_zeros();
    this_man >>= overflow;
    exp += BitArray<s_bits>{overflow};
    this_man -= s_two_to_the_M;
    BitArray<s_bits> sign{
        static_cast<WORD>(this->get_sign() ? 1 : 0)
    };
    sign <<= E + M;
    // copy the sign of other (which is the same as that of other)
    this->d_data = sign | (exp << M) | this_man;
    return *this;
}
