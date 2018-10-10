template<size_t M, size_t E>
F<M, E> constexpr F<M, E>::pow(size_t const exp) const
{
    F<M, E> result = 1;
    for (size_t iter = 0; iter < exp; ++iter)
        result *= *this;

    return result;
}