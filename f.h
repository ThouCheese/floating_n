#ifndef def_included_f_h
#define def_included_f_h

#include "bit_array.h"

// here M denotes the matissa length, and E the exponent length
template<size_t M, size_t E>
class F
{
public:
    size_t static constexpr s_bits = M + E + 1;

    // the implicit leading one in the mantissa, used for addition
    BitArray<s_bits> static constexpr s_two_to_the_M = 
        BitArray<s_bits>::with_ones(M, M + 1);
    // since the exponent is stored in biased form, to obtain it's true value,
    // we need to subtract this number.
    BitArray<s_bits> static constexpr s_two_to_the_E = 
        BitArray<s_bits>::with_ones(E, E + 1);

    // masks for extracting the specific numbers
    BitArray<s_bits> static constexpr s_sign_mask = 
        BitArray<s_bits>::with_ones(M + E, M + E + 1);
    BitArray<s_bits> static constexpr s_man_mask;// = 
        // BitArray<s_bits>::with_ones(M, M + E);
    BitArray<s_bits> static constexpr s_exp_mask = 
        BitArray<s_bits>::with_ones(0, M);

    // constant numbers
    F static constexpr ZERO = F<M, E>(
        BitArray<s_bits>::with_ones(M + E, M + E + 1));
    F static constexpr NEG_ZERO = F<M, E>(
        BitArray<s_bits>());
    // A var named NAN won't compile with a very confusing error message
    F static constexpr _NAN = F<M, E>( 
        BitArray<s_bits>::with_ones(M, M + E + 1));
    F static constexpr INF = F<M, E>(
        BitArray<s_bits>::with_ones(0, M + E + 1));
    F static constexpr NEG_INF = F<M, E>(
        BitArray<s_bits>::with_ones(0, M + E));
    F static constexpr ONE = F<M, E>(
        BitArray<s_bits>::with_ones(M + E - 1, M + E + 1));
    // todo: PI, EULER, EPSILON

    BitArray<s_bits> d_data;

    constexpr F<M, E>() = default;
    F<M, E>(double const number);

private:
    // these functions are for internal use only, as they don't treat the
    // number so much as a number, but rather as three separate numbers. This 
    // is counterintuitive for the user.
    constexpr F<M, E>(BitArray<s_bits> const bit_array);
    F<M, E> set_man(BitArray<s_bits> const matissa);
    F<M, E> set_exp(BitArray<s_bits> const exponent);
    F<M, E> set_sign(bool const sign);
    BitArray<s_bits> get_man();
    BitArray<s_bits> get_exp();
    bool get_sign();

    // operators
    bool operator==(F<M, E> const other);
    bool operator>(F<M, E> const other);
    bool operator>=(F<M, E> const other);
    bool operator<(F<M, E> const other);
    bool operator<=(F<M, E> const other);
    F<M, E> operator*=(F<M, E> const other);
    F<M, E> operator*(F<M, E> const other);

    // mathematical functions
    F<M, E> abs();

    template<size_t _M, size_t _E>
    friend std::ostream &operator<<(std::ostream &stream, F<_M, _E> number);
};

template<size_t M, size_t E>
BitArray<F<M, E>::s_bits> constexpr F<M, E>::s_man_mask = 
        BitArray<F<M, E>::s_bits>::with_ones(M, M + E);

template<size_t M, size_t E>
std::ostream &operator<<(std::ostream &stream, F<M, E> number);

template<size_t M, size_t E>
F<M, E>::F(double const number)
{
    if (number == 0.)
    {   
        *this = ZERO;
        return;
    }
    double pos_num = std::abs(number);
    WORD exponent_word = static_cast<WORD>(std::log(pos_num)/std::log(2.0));
    BitArray<s_bits> exp = BitArray<s_bits>{exponent_word};
    // casting to WORD means 64 bits precision while initializing if WORD is
    // 64 bits. This is more than the standard double, so it's fine. For 
    // proper support for 32 bit machines, this needs to be altered at some 
    // point
    BitArray<s_bits> man = BitArray<s_bits>{
        static_cast<WORD>(pos_num / std::pow(2, exponent_word) * 
            std::pow(2, M)) 
    };
    man -= BitArray<s_bits>::with_ones(M, M + 1);
    BitArray<s_bits> sign = BitArray<s_bits>{
        static_cast<WORD>(number > 0.0 ? 1 : 0)
    };
    std::cout << "man is " << pos_num / std::pow(2, exponent_word) * 
            std::pow(2, M) << '\n';

    this->d_data = man | (exp << M) | (sign << M + E);
    
}

template<size_t M, size_t E>
constexpr F<M, E>::F(BitArray<s_bits> const bit_array)
    :
        d_data{bit_array}
{}

template<size_t M, size_t E>
F<M, E> F<M, E>::set_man(BitArray<s_bits> const mantissa)
{
    d_data &= !s_man_mask; // set the mantissa bits to zero
    d_data |= mantissa; // insert the correct bits
    return *this;
}

template<size_t M, size_t E>
F<M, E> F<M, E>::set_exp(BitArray<s_bits> const exponent)
{
    d_data &= !s_exp_mask; // set the exponent bits to zero
    d_data |= exponent << M; // insert the correct bits
    return *this;
}

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
F<M, E> F<M, E>::operator*=(F<M, E> const other)
{
    if (*this == NAN or other == NAN)
        return NAN;
    else if (this->abs() == ZERO or other.abs() == ZERO)
        return this->sign() == other.sign() ? ZERO : NEG_ZERO;
    else if (this->abs() == INF or other.abs() == INF)
        return this->sign() == other.sign() ? INF : NEG_INF;

    // we compute the product of the two mantissa's
    BitArray<s_bits> mantissa = 
        this->man().floating_point_mul_internal(other.man()) +
        this->man() + other.man() + s_two_to_the_M;
    // we need to shift the result (left or right) until the leading 1 is at 
    // position M. 
    int64_t overflow = static_cast<int64_t>(E) - mantissa.leading_zeros();
    mantissa >>= overflow;
    // since the leading bit is implicit, we remove it
    mantissa -= s_two_to_the_M;
    BitArray<s_bits> exponent = this->exp() + other.exp() + overflow;
    // TODO
}

template<size_t M, size_t E>
F<M, E> F<M, E>::operator*(F<M, E> const other)
{
    return F<M, E>{*this} *= other;
}

template<size_t M, size_t E>
F<M, E> F<M, E>::abs()
{
    return F<M, E>{*this}.set_sign(true);
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
