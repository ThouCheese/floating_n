size_t constexpr fact(size_t const n)
{
    return n == 0 ? 1 : n * fact(n - 1);
}

template<size_t M, size_t E>
F<M, E> constexpr F<M, E>::get_pi() noexcept
{
    F<M, E> _2 = ONE + ONE;      // initialized to 2
    F<M, E> denom = _2;          // initialized to 2
    F<M, E> pi = _2 + ONE;       // initialzied to 3
    F<M, E> _4 = pi + ONE;       // initialized to 4
    F<M, E> pi_previous = ZERO;  // initialized to 0
    for (size_t iter = 0; iter < 30; ++iter)
    {
        pi_previous = pi;
        pi += _4 / (denom * (denom + ONE) * (denom + _2));
        denom += _2;
        pi -= _4 / (denom * (denom + ONE) * (denom + _2));
        denom += _2;
    }
    return pi;
}
