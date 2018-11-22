template<size_t M, size_t E>
constexpr F<M, E> F<M, E>::operator-=(F<M, E> const other)
{
    if (other == ZERO or other == NEG_ZERO)
        return *this;
    if (*this == ZERO or *this == NEG_ZERO)
        return *this = -other;
    if (this->get_sign() != other.get_sign())
    {
        if (this->get_sign())
            return *this += other;
        else
            return *this = -*this + -other;
    }
    F<M, E> const *great = 0, *small = 0;
    if (this->abs() > other.abs())
        great = this, small = &other;
    else
        great = &other, small = this;
    BitArray<s_bits> great_man = great->get_man() + s_two_to_the_M;
    BitArray<s_bits> small_man = small->get_man() + s_two_to_the_M;
    BitArray<s_bits> great_exp = great->get_exp();
    BitArray<s_bits> small_exp = small->get_exp();
    size_t exp_difference = (great_exp - small_exp).d_data[0];
    small_man >>= exp_difference;
    BitArray<s_bits> man = great_man - small_man;
    size_t overflow = man.leading_zeros() - E;
    man <<= overflow;
    man -= s_two_to_the_M;
    BitArray<s_bits> exp = (great_exp - BitArray<s_bits>(overflow)) << M;
    
    auto sign = BitArray<s_bits>(
        great->get_sign() && (this == great) ? 1ul : 0
    ) << M + E;

    this->d_data = sign | exp | man;
    return *this;
}
