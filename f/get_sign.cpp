template<size_t M, size_t E>
bool F<M, E>::get_sign() const
{
    return (d_data >> (M + E)).d_data[0] == 1;
}