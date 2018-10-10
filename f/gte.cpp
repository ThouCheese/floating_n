template<size_t M, size_t E>
bool constexpr F<M, E>::operator>=(F<M, E> const other) const
{
    return not (*this < other);
}