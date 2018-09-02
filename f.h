#ifndef def_included_f_h
#define def_included_f_h

#include "bit_array.h"

// here M denotes the matissa length, and E the exponent length
template<size_t M, size_t E>
class F
{
public:
    size_t static constexpr s_bits = M + E + 1;

    BitArray<s_bits> static constexpr s_sign_mask = 
        BitArray<s_bits>::with_ones(M + E, M + E + 1);
    BitArray<s_bits> static constexpr s_man_mask = 
        BitArray<s_bits>::with_ones(M + E, M);
    BitArray<s_bits> static constexpr s_exp_mask = 
        BitArray<s_bits>::with_ones(0, M);

    F static constexpr ZERO = F<M, E>(
        BitArray<s_bits>(((WORD) 1) << (BitArray<s_bits>::WORD_LENGTH - 1)));
    F static constexpr NEG_ZERO = F<M, E>(
        BitArray<s_bits>());

    BitArray<s_bits> d_data;

    constexpr F<M, E>() = default;
    F<M, E>(double const number);

private:
    // these functions are for internal use only, as they don't treat the
    // number so much as a number, but rather as three separate numbers. This 
    // is counterintuitive for the user.
    constexpr F<M, E>(BitArray<s_bits> const bit_array);
    void set_man(BitArray<s_bits> const matissa);
    void set_exp(BitArray<s_bits> const exponent);
    void set_sign(bool const sign);
    BitArray<s_bits> get_man();
    BitArray<s_bits> get_exp();
    bool get_sign();

    bool operator==(F<M, E> const other);
    bool operator>(F<M, E> const other);
    bool operator>=(F<M, E> const other);
    bool operator<(F<M, E> const other);
    bool operator<=(F<M, E> const other);

    template<size_t _M, size_t _E>
    friend std::ostream &operator<<(std::ostream &stream, F<_M, _E> number);
};

template<size_t M, size_t E>
F<M, E>::F(double const number)
{
    // todo: convert normal double to F<M, E>
}

template<size_t M, size_t E>
constexpr F<M, E>::F(BitArray<s_bits> const bit_array)
    :
        d_data{bit_array}
{}

template<size_t M, size_t E>
void F<M, E>::set_man(BitArray<s_bits> const mantissa)
{
    d_data &= !s_man_mask; // set the mantissa bits to zero
    d_data |= mantissa; // insert the correct bits
}

template<size_t M, size_t E>
void F<M, E>::set_exp(BitArray<s_bits> const exponent)
{
    d_data &= !s_exp_mask; // set the exponent bits to zero
    d_data |= exponent << M; // insert the correct bits
}

template<size_t M, size_t E>
void F<M, E>::set_sign(bool const sign)
{
    BitArray constexpr mask = BitArray<s_bits>::with_ones(0, s_bits);
    if (sign)
        *this |= !mask;
    else
        *this &= mask;
}

template<size_t M, size_t E>
BitArray<F<M, E>::s_bits> F<M, E>::get_man()
{
    return d_data & s_man_mask;
}

template<size_t M, size_t E>
BitArray<F<M, E>::s_bits> F<M, E>::get_exp()
{
    return (d_data & s_exp_mask) >> M;
}

template<size_t M, size_t E>
bool F<M, E>::get_sign()
{
    return (d_data >> (M + E)).d_data[0] == 1;
}

template<size_t M, size_t E>
bool F<M, E>::operator==(F<M, E> const other)
{
    return this->d_data == other.d_data;
}

template<size_t M, size_t E>
bool F<M, E>::operator>(F<M, E> const other)
{   
    if (this->get_sign() != other.get_sign())
        return this->get_sign();

    if (this->get_exp() > other.get_exp())
        return true;
    else if (this->get_exp() < other.get_exp())
        return false;

    if (this->get_man() > other.get_man())
        return true;
    return false;
}

template<size_t M, size_t E>
bool F<M, E>::operator<(F<M, E> const other)
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

template<size_t M, size_t E>
bool F<M, E>::operator>=(F<M, E> const other)
{
    return not (*this < other);
}

template<size_t M, size_t E>
bool F<M, E>::operator<=(F<M, E> const other)
{
    return not (*this > other);
}

template<size_t M, size_t E>
std::ostream &operator<<(std::ostream &stream, F<M, E> number)
{
    // we need a logarithm function to generate a stream of digits, which is a
    // long way down the road, so for now we rely on the builtins. The largest
    // one is the long double, which may still overflow. This means that very
    // large numbers are not printable using this implementation

    // todo: use of the `<F, M>::operator== causes an undefined refernce to 
    // `BitArray<F + M + 1>::operator==`. Not sure how to fix this.
    // if (number == F<M, E>::ZERO)
    //     std::cout << '0';
    // else if (number == F<M, E>::NEG_ZERO)
    //     std::cout << "-0";
    // else 
    {
        if (not number.get_sign())
            stream << '-';
        long double constexpr leading_one = pow(2L, M);
        stream << (leading_one + number.get_man().to_ld()) / leading_one * 
               pow(2L, number.get_exp().to_ld());

    }
    return stream;
}

#endif
