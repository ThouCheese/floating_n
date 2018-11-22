template<size_t M, size_t E>
constexpr F<M, E>::F(double const number)
{
    // assume the following layout for a double:
    // 1 bit that is 0 if +, 1 if -
    // 11 bits that are the biased exponent
    // 52 bits that are the matissa
    WORD num = f64_to_u64(number);
    BitArray<s_bits> sign{num & 0x8000000000000000 ? 0ul : 1};
    // BitArray<s_bits>  exp{(num & 0x7FF0000000000000) >> 52};
    
    BitArray<s_bits> exp = s_exponent_bias + BitArray<s_bits>{1ul};
    if (num & 0x4000000000000000)
        exp += BitArray<s_bits>((num & 0x3FF0000000000000) >> 52);
    else
        exp -= BitArray<s_bits>((0x4000000000000000 - (num & 0x3FF0000000000000)) >> 52);


    BitArray<s_bits>  man{num & 0x000FFFFFFFFFFFFF};
    man <<= (M - 52);

    this->d_data = (sign << M + E) | (exp << M) | man;
}
