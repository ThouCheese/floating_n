template<size_t M, size_t E>
std::ostream &operator<<(std::ostream &stream, F<M, E> number)
{
    // we need a logarithm function to generate a stream of digits, which is a
    // long way down the road, so for now we rely on the builtins. The largest
    // one is the long double, which may still overflow. This means that very
    // large numbers are not printable using this implementation

    if (number == F<M, E>::ZERO)
        std::cout << '0';
    else if (number == F<M, E>::NEG_ZERO)
        std::cout << "-0";
    else 
    {
        if (not number.get_sign())
            stream << '-';
        long double constexpr leading_one = pow(2L, M);
        stream << (leading_one + number.get_man().to_ld()) /
                   leading_one * pow(2L, number.get_exp().to_ld()
                       - F<M, E>::s_exponent_bias.to_ld());
    }
    return stream;
}