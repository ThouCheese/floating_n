template<size_t M, size_t E>
F<M, E> constexpr F<M, E>::operator%=(F<M, E> const other)
{
    return *this -= (*this / other).floor() * other;
}