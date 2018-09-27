#ifndef def_included_f_h
#define def_included_f_h

#include "bit_array.h"

#define LIKELY(condition) __builtin_expect(static_cast<bool>(condition), 1)
#define UNLIKELY(condition) __builtin_expect(static_cast<bool>(condition), 0)

// here M denotes the matissa length, and E the exponent length
template<size_t M, size_t E>
class F
{
    // important constants used interally
    size_t static constexpr s_bits = M + E + 1;
    BitArray<s_bits> static constexpr
            s_two_to_the_M = BitArray<s_bits>::with_ones(M, M + 1),
            s_two_to_the_E = BitArray<s_bits>::with_ones(E, E + 1),
            s_exponent_bias = BitArray<s_bits>::with_ones(E - 1, E);
public:

    // masks for extracting the specific numbers
    BitArray<s_bits> static constexpr
        s_sign_mask = BitArray<s_bits>::with_ones(M + E, M + E + 1),
        s_exp_mask = BitArray<s_bits>::with_ones(M, M + E),
        s_man_mask = BitArray<s_bits>::with_ones(0, M);

    // constant numbers
    F static constexpr
        ZERO{BitArray<s_bits>::with_ones(M + E, M + E + 1)},
        NEG_ZERO{BitArray<s_bits>()},
        _NAN{BitArray<s_bits>::with_ones(M, M + E + 1)},
        INF{BitArray<s_bits>::with_ones(0, M + E + 1)},
        NEG_INF{BitArray<s_bits>::with_ones(0, M + E)},
        ONE{BitArray<s_bits>::with_ones(M + E - 1, M + E + 1)};
    // todo: PI, EULER, EPSILON, will be hard

    BitArray<s_bits> d_data;

    constexpr F<M, E>() = default;
    F<M, E>(double const number);

private:
    constexpr F<M, E>(BitArray<s_bits> const bit_array);
    // these functions are for internal use only, as they don't treat the
    // number so much as a number, but rather as three separate numbers. This 
    // is counterintuitive for the user.
    F<M, E> set_man(BitArray<s_bits> const matissa);
    F<M, E> set_exp(BitArray<s_bits> const exponent);
    F<M, E> set_sign(bool const sign);
    BitArray<s_bits> get_man() const;
    BitArray<s_bits> get_exp() const;
    bool get_sign() const;
public:
    // operators
    bool operator==(F<M, E> const other) const;
    bool operator>(F<M, E> const other) const;
    bool operator>=(F<M, E> const other) const;
    bool operator<(F<M, E> const other) const;
    bool operator<=(F<M, E> const other) const;

    F<M, E> operator-() const;
    F<M, E> operator+=(F<M, E> const other);
    F<M, E> operator+(F<M, E> const other) const;
    F<M, E> operator-=(F<M, E> const other);
    F<M, E> operator-(F<M, E> const other) const;
    F<M, E> operator*=(F<M, E> const other);
    F<M, E> operator*(F<M, E> const other) const;
    F<M, E> operator~() const;

    // mathematical functions
    F<M, E> abs() const;
    F<M, E> sin() const;
    F<M, E> cos() const;
    F<M, E> tan() const;
    F<M, E> sqrt() const;

    template<size_t _M, size_t _E>
    friend std::ostream &operator<<(std::ostream &stream, F<_M, _E> number);
};

template<size_t M, size_t E>
BitArray<F<M, E>::s_bits> constexpr F<M, E>::s_two_to_the_M;

template<size_t M, size_t E>
BitArray<F<M, E>::s_bits> constexpr F<M, E>::s_two_to_the_E;

template<size_t M, size_t E>
BitArray<F<M, E>::s_bits> constexpr F<M, E>::s_exponent_bias;

template<size_t M, size_t E>
F<M, E> constexpr F<M, E>::ZERO;

template<size_t M, size_t E>
F<M, E> constexpr F<M, E>::NEG_ZERO;

template<size_t M, size_t E>
F<M, E> constexpr F<M, E>::INF;

template<size_t M, size_t E>
F<M, E> constexpr F<M, E>::NEG_INF;

template<size_t M, size_t E>
BitArray<F<M, E>::s_bits> constexpr F<M, E>::s_sign_mask;

template<size_t M, size_t E>
BitArray<F<M, E>::s_bits> constexpr F<M, E>::s_man_mask;

template<size_t M, size_t E>
BitArray<F<M, E>::s_bits> constexpr F<M, E>::s_exp_mask;

template<size_t M, size_t E>
std::ostream &operator<<(std::ostream &stream, F<M, E> number);

template<size_t M, size_t E>
F<M, E>::F(double const number)
{
    // might be more complicated than necessary
    if (UNLIKELY(number == 0))
    {   
        *this = ZERO;
        return;
    }
    double pos_num = std::abs(number);
    double exponent_double = std::log(pos_num / std::log(2.0));
    BitArray<s_bits> exp; // avoid RAII for performance?
    BitArray<s_bits> man; // same
    if (exponent_double > 0.0)
    {
        WORD exp_word = static_cast<WORD>(std::log(pos_num) / std::log(2.0));
        exp = s_exponent_bias + BitArray<s_bits>{exp_word};
        WORD man_word = static_cast<WORD>(
            pos_num / std::pow(2, exp_word) * std::pow(2, M - 1)
        );
        man = (BitArray<s_bits>{man_word} << 1) - s_two_to_the_M;
    }
    else
    {
        WORD exp_word = static_cast<WORD>(-std::log(pos_num) / std::log(2.0)+1);
        exp = s_exponent_bias - BitArray<s_bits>{exp_word};
        WORD man_word = static_cast<WORD>(
            pos_num / std::pow(2, -static_cast<int64_t>(exp_word))
                * std::pow(2, M - 1)
        );
        man = (BitArray<s_bits>{man_word} << 1) - s_two_to_the_M;
    }
    BitArray<s_bits> sign{static_cast<WORD>(number > 0.0 ? 1 : 0)};
    this->d_data = (sign << M + E) | (exp << M) | man;
}

template<size_t M, size_t E>
constexpr F<M, E>::F(BitArray<s_bits> const bit_array) : d_data{bit_array}  {}

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
BitArray<F<M, E>::s_bits> F<M, E>::get_man() const
{
    return d_data & s_man_mask;
}

template<size_t M, size_t E>
BitArray<F<M, E>::s_bits> F<M, E>::get_exp() const
{
    return (d_data & s_exp_mask) >> M;
}

template<size_t M, size_t E>
bool F<M, E>::get_sign() const
{
    return (d_data >> (M + E)).d_data[0] == 1;
}

template<size_t M, size_t E>
bool F<M, E>::operator==(F<M, E> const other) const
{
    return this->d_data == other.d_data;
}

template<size_t M, size_t E>
bool F<M, E>::operator>(F<M, E> const other) const
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
bool F<M, E>::operator<(F<M, E> const other) const
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
bool F<M, E>::operator>=(F<M, E> const other) const
{
    return not (*this < other);
}

template<size_t M, size_t E>
bool F<M, E>::operator<=(F<M, E> const other) const
{
    return not (*this > other);
}

template<size_t M, size_t E>
F<M, E> F<M, E>::operator-() const
{
    return F<M, E>{this->d_data ^ s_sign_mask};
}

template<size_t M, size_t E>
F<M, E> F<M, E>::operator+=(F<M, E> const other)
{
    if (this->get_sign() != other.get_sign())
    {
        if (this->get_sign())
            return *this -= -other;
        else {
            *this = -*this;
            return *this -= -other;
        }
    }

    // it might seem silly to restrict the exponent difference to a long, but
    // before this becomes a limiting factor, you need to assign a mantissa with
    // length of LONG_MAX_VALUE / 2, which takes 1/8 GB for the mantissa alone.
    long exponent_difference;
    if (this->get_exp() > other.get_exp())
        exponent_difference = (this->get_exp() - other.get_exp()).d_data[0];
    else
        exponent_difference = -static_cast<long>(
            (other.get_exp() - this->get_exp()).d_data[0]
        );
    std::cout << "exp diff is " << exponent_difference << '\n';

    // we shift the value of the mantissa's one to the right
    BitArray<s_bits> this_man  = (this->get_man() + s_two_to_the_M);
    BitArray<s_bits> other_man = (other.get_man() + s_two_to_the_M);
    BitArray<s_bits> exp;
    size_t overflow;
    if (exponent_difference > 0)
    {
        other_man >>= exponent_difference;
        this_man += other_man;
        exp = this->get_exp();
    }
    else if (exponent_difference < 0)
    {
        this_man >>= -exponent_difference;
        this_man += other_man;
        exp = other.get_exp();
    }
    else
    {
        this_man += other_man;
        this_man >>= 1;
        exp = this->get_exp() + BitArray<s_bits>{1ul};
    };
    overflow = E - this_man.leading_zeros();
    this_man >>= overflow;
    exp += BitArray<s_bits>{overflow};
    this_man -= s_two_to_the_M;
    // copy the sign of other (which is the same as that of other)
    return this->d_data = s_sign_mask | (exp << M) | this_man;
}

template<size_t M, size_t E>
F<M, E> F<M, E>::operator+(F<M, E> const other) const
{
    return F<M, E>{*this} += other;
}

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

        F<M, E> *great, *small;
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

template<size_t M, size_t E>
F<M, E> F<M, E>::operator*=(F<M, E> const other)
{
    if (UNLIKELY(*this == NAN or other == NAN))
        return NAN;
    else if (this->abs() == ZERO or other.abs() == ZERO)
        return this->get_sign() == other.get_sign() ? ZERO : NEG_ZERO;
    else if (this->abs() == INF or other.abs() == INF)
        return this->get_sign() == other.get_sign() ? INF : NEG_INF;
    // we compute the product of the two mantissa's
    BitArray<s_bits> man =
        floating_point_mul_internal<s_bits, M>(this->get_man(), other.get_man())
            + this->get_man() + other.get_man() + s_two_to_the_M;
    // we need to shift the result (left or right) until the leading 1 is at
    // position M.
    WORD overflow = E - man.leading_zeros();
    man >>= overflow;
    // since the leading bit is implicit, we remove it
    man -= s_two_to_the_M;
    // the new exponent is the sum of the other two
    BitArray<s_bits> exp = this->get_exp() + other.get_exp() - s_exponent_bias;
    // now we adjust the exponent according to how much the mantissa overflowed
    if (overflow > 0)
        exp += BitArray<s_bits>(static_cast<WORD>(overflow));
    else
        exp -= BitArray<s_bits>(static_cast<WORD>(-overflow));
    BitArray<s_bits> sign{
        static_cast<WORD>(this->get_sign() ^ other.get_sign() ? 0 : 1)
    };
    return this->d_data = (sign << M + E) | (exp << M) | man;
}

template<size_t M, size_t E>
F<M, E> F<M, E>::operator*(F<M, E> const other) const
{
    return F<M, E>{*this} *= other;
}

template<size_t M, size_t E>
F<M, E> F<M, E>::operator~() const
{
    return F<M, E>{this->d_data ^ s_exp_mask};
}

template<size_t M, size_t E>
F<M, E> F<M, E>::abs() const
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

    if (number == F<M, E>::ZERO)
        std::cout << '0';
    else if (number == F<M, E>::NEG_ZERO)
        std::cout << "-0";
    else 
    {
        if (not number.get_sign())
            stream << '-';
        long double constexpr leading_one = pow(2L, M);
        stream << (leading_one + number.get_man().to_ld()) /
                   leading_one * pow(2L, number.get_exp().to_ld()
                       - F<M, E>::s_exponent_bias.to_ld());
    }
    return stream;
}

#endif
