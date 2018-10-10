template<size_t M, size_t E>
constexpr F<M, E>::F(double const number)
{
    // might be more complicated than necessary
    if (number == 0)
    {   
        *this = ZERO;
        return;
    }
    double pos_num = std::abs(number);
    double exponent_double = __builtin_log(pos_num / std::log(2.0));
    BitArray<s_bits> exp;
    BitArray<s_bits> man;
    if (exponent_double > 0.0)
    {
        // the exponent is the log2 of the number rounded down
        WORD exp_word = static_cast<WORD>(std::log(pos_num) / std::log(2.0));
        // add the bias so we can store the value
        exp = s_exponent_bias + BitArray<s_bits>{exp_word};

        WORD man_word = static_cast<WORD>(
            pos_num / std::pow(2, exp_word) * std::pow(2, M - 1)
        );
        // if the mantissa word is precisely zero, we have a special case and we
        // cannot subtract the implicit leading one. Instead, we leave the 
        // mantissa as-is and we need to add 1 to the exponent
        if (man_word)
            man = (BitArray<s_bits>{man_word} << 1) - s_two_to_the_M;
        else
            exp += BitArray<s_bits>{1ul};
    }
    else
    {
        WORD exp_word = static_cast<WORD>(-std::log(pos_num) / std::log(2.0)+1);
        exp = s_exponent_bias - BitArray<s_bits>{exp_word};
        WORD man_word = static_cast<WORD>(
            pos_num / std::pow(2, -static_cast<int64_t>(exp_word))
                * std::pow(2, M - 1)
        );
        if (man_word)
            man = (BitArray<s_bits>{man_word} << 1) - s_two_to_the_M;
        else
            exp += BitArray<s_bits>{1ul};
    }
    BitArray<s_bits> sign{static_cast<WORD>(number > 0.0 ? 1 : 0)};
    this->d_data = (sign << M + E) | (exp << M) | man;
}
