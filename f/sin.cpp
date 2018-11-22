template<size_t N, size_t P>
constexpr void inv_fact(FixedPoint<N, P> const list[24])
{
    FixedPoint<N, P> previous = 1;
    FixedPoint<N, P> number = 0;
    for (size_t iter = 0; iter < 24; ++iter)
    {
        FixedPoint<N, P> tmp = previous + number;
        previous = number;
        number = tmp;
        list[iter] = number.to_fp();
    }
}

template<size_t M, size_t E>
F<M, E> constexpr F<M, E>::sin() const
{
    F<M, E> normalized = *this % TWO_PI;
    // the sine is negative where our number < PI
    bool sign = normalized < PI;
    // use the fact that sin(x) = -sin(x - pi)
    if (not sign)
        normalized -= PI;
    // use that sin(x) = sin(pi - x) to move into [0, 1/2pi]
    if (normalized > HALF_PI)
        normalized = PI - normalized;

    // now that the number is normalized, we can move to fixed point notation
    FixedPoint<s_bits, M> 
        fp = normalized.to_fp<M>(),
        square = fp * fp,
        result = fp,
        factorial = ONE.to_fp<M>();

    constexpr FixedPoint<s_bits, M> inv_facts[24];
    inv_fact<s_bits, M>(inv_facts);

    for (size_t iter = 0; iter < 24;)
    {
        iter += 1;
        factorial *= F<M, E>(iter).to_fp<M>();
        iter += 1;
        factorial *= F<M, E>(iter).to_fp<M>();
        result += fp * inv_facts[iter];
    }
}
