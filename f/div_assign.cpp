template<size_t M, size_t E>
constexpr F<M, E> F<M, E>::operator/=(F<M, E> const other)
{
    size_t constexpr REPETITIONS = std::ceil(std::log((M + 1) / std::log(17)));

    if (other == _NAN or other == ZERO or other == NEG_ZERO)
        return *this = _NAN;
    if (*this == ZERO or *this == NEG_ZERO or *this == _NAN)
        return *this;

    // en.wikipedia.org/wiki/Division_algorithm#Newton%E2%80%93Raphson_division
    FixedPoint<s_bits, M> other_fp = (other.get_man() + s_two_to_the_M) >> 1;
    FixedPoint<s_bits, M> constexpr _48over17 = 
        (F<M, E>{48.0 / 17.0}.get_man() + s_two_to_the_M) << 1;
    FixedPoint<s_bits, M> constexpr _32over17 = 
        (F<M, E>{32.0 / 17.0}.get_man() + s_two_to_the_M);
    FixedPoint<s_bits, M> result = _48over17 - _32over17 * other_fp;
    for (size_t iter = 0; iter < REPETITIONS * 2; ++iter)
        result += result - other_fp * result * result;

    BitArray<s_bits> man = result - s_two_to_the_M;
    // making the exponent negative is weird with biased notation
    BitArray<s_bits> exp = ~other.get_exp() & (s_exp_mask >> M);
    // division by something with mantissa precisely 1 (so 000...000) in bits 
    // creates a reciprocal of precisely 2^(M + 1). We are now left with
    // precisely 2^M after subtracting 2^M. We adjust the mantissa and exponent
    // accordingly
    if (man == s_two_to_the_M)
    {
        man = BitArray<s_bits>{};  // empty
        exp += BitArray<s_bits>{1ul};
    }
    exp <<= M;
    auto sign = BitArray<s_bits>(
        this->get_sign() == other.get_sign() ? 1ul : 0
    ) << M + E;
    F<M, E> reciprocal(sign | exp | man);
    return *this *= reciprocal;
}
