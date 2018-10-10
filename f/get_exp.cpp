template<size_t M, size_t E>
constexpr BitArray<F<M, E>::s_bits> F<M, E>::get_exp() const
{
    return (d_data & s_exp_mask) >> M;
}
