size_t constexpr fact(size_t const n)
{
    return n == 0 ? 1 : n * fact(n - 1);
}

template<size_t M, size_t E>
F<M, E> constexpr F<M, E>::get_pi() noexcept
{
    F<M, E> pi = 3;
    F<M, E> _4 = 4;
    F<M, E> denom = 2;
    for (size_t iter = 1; iter < 1000; ++iter)
    {
        pi += _4 / (denom * (denom + 1) * (denom + 2));
        denom += 2;
        pi -= _4 / (denom * (denom + 1) * (denom + 2));
        denom += 2;
    }
    return pi;
}
