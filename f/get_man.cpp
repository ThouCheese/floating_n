template<size_t M, size_t E>
constexpr BitArray<F<M, E>::s_bits> F<M, E>::get_man() const
{
    return d_data & s_man_mask;
}