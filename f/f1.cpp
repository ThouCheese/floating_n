template<size_t M, size_t E>
F<M, E>::F(double const number)
{
    // might be more complicated than necessary
    if (UNLIKELY(number == 0))
    {   
        *this = ZERO;
        return;
    }
    double pos_num = std::abs(number);
    double exponent_double = std::log(pos_num / std::log(2.0));
    BitArray<s_bits> exp; // avoid RAII for performance?
    BitArray<s_bits> man; // same
    if (exponent_double > 0.0)
    {
        WORD exp_word = static_cast<WORD>(std::log(pos_num) / std::log(2.0));
        exp = s_exponent_bias + BitArray<s_bits>{exp_word};
        WORD man_word = static_cast<WORD>(
            pos_num / std::pow(2, exp_word) * std::pow(2, M - 1)
        );
        man = (BitArray<s_bits>{man_word} << 1) - s_two_to_the_M;
    }
    else
    {
        WORD exp_word = static_cast<WORD>(-std::log(pos_num) / std::log(2.0)+1);
        exp = s_exponent_bias - BitArray<s_bits>{exp_word};
        WORD man_word = static_cast<WORD>(
            pos_num / std::pow(2, -static_cast<int64_t>(exp_word))
                * std::pow(2, M - 1)
        );
        man = (BitArray<s_bits>{man_word} << 1) - s_two_to_the_M;
    }
    BitArray<s_bits> sign{static_cast<WORD>(number > 0.0 ? 1 : 0)};
    this->d_data = (sign << M + E) | (exp << M) | man;
}
