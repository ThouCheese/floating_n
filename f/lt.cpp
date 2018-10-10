template<size_t M, size_t E>
bool constexpr F<M, E>::operator<(F<M, E> const other) const
{ 
    if (this->get_sign() != other.get_sign())
        return other.get_sign();

    if (this->get_exp() < other.get_exp())
        return true;
    else if (this->get_exp > other.get_exp())
        return false;

    if (this->get_man() < other.get_man())
        return true;
    return false;
}