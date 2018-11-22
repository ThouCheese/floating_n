template<size_t M, size_t E>
F<M, E> constexpr F<M, E>::floor() const
{
    // will do nothing if the last digit of the mantissa has value less than
    // one.
    if (this->abs() < ONE)
        return ZERO;
    BitArray<s_bits> exp = get_exp() - s_exponent_bias;
    // make sure that the exponent is not greater than 2^(2^64)
    assert(exp.leading_zeros() - s_bits < 64);
    size_t exp_word = exp.d_data[0];
    // now shift of the rightmost `exp` bits.
    man >>= exp_word;
    man <<= exp_word;
    return F<M, E>(*this).set_man(man);
}