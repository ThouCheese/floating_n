template<size_t M, size_t E>
F<M, E> F<M, E>::operator-=(F<M, E> const other)
{
    if (this->get_sign() != other.get_sign())
    {
        if (this->get_sign())
            return *this += other;
        else
        {
            *this = -*this;
            *this += -other;
        }

        F<M, E> const *great, *small;
        if (this->abs() > other.abs())
        {
            great = this;
            small = &other;
        }
        else
        {
            great = &other;
            small = this;
        }
        
    }
}
