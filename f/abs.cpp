template<size_t M, size_t E>
constexpr F<M, E> F<M, E>::abs() const
{
    return F<M, E>{*this}.set_sign(true);
}
