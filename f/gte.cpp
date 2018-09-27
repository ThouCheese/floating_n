template<size_t M, size_t E>
bool F<M, E>::operator>=(F<M, E> const other) const
{
    return not (*this < other);
}