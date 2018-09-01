#include "bit_array.h"

// here M denotes the matissa length, and E the exponent length
template<size_t M, size_t E>
class F
{

    

public:
    size_t static constexpr bits = M + E + 1;
    BitArray<bits> static constexpr sign_mask = BitArray<bits>(1) << (M + E);
    F static constexpr ZERO = 
        F<M, E>(BitArray<bits>(((WORD) 1) << (BitArray<bits>::WORD_LENGTH - 1)));
    

    BitArray<bits> d_bits;
    constexpr F<M, E>() = default;
    F<M, E>(double const number);

private:
    constexpr F<M, E>(BitArray<bits> const bit_array);
    void set_man(BitArray<bits> const matissa);
    void set_exp(BitArray<bits> const exponent);
    void set_sign(bool const sign);
    BitArray<bits> get_man();
    BitArray<bits> get_exp();
    bool get_sign();
};

template<size_t M, size_t E>
F<M, E>::F(double const number)
{

}

template<size_t M, size_t E>
constexpr F<M, E>::F(BitArray<bits> const bit_array)
{
    d_bits = d_bits;
}

template<size_t M, size_t E>
void F<M, E>::set_man(BitArray<bits> const mantissa)
{
    // d_bits = 
}