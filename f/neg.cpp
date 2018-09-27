template<size_t M, size_t E>
F<M, E> F<M, E>::operator-() const
{
    return F<M, E>{this->d_data ^ s_sign_mask};
}