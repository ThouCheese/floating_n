template<size_t M, size_t E>
F<M, E> F<M, E>::abs() const
{
    return F<M, E>{*this}.set_sign(true);
}
