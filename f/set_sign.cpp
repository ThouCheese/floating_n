template<size_t M, size_t E>
F<M, E> F<M, E>::set_sign(bool const sign)
{
    BitArray<s_bits> constexpr mask = BitArray<s_bits>::with_ones(0, s_bits);
    if (sign)
        this->d_data |= !mask;
    else
        this->d_data &= mask;
    return *this;
}
