template<size_t M, size_t E>
F<M, E> F<M, E>::operator*(F<M, E> const other) const
{
    return F<M, E>{*this} *= other;
}
