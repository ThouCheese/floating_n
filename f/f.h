#ifndef def_included_f_h
#define def_included_f_h

#include "bitarray/bit_array.h"

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

#include "f1.cpp"

#include "f2.cpp"

#include "set_man.cpp"

#include "set_exp.cpp"

#include "set_sign.cpp"

#include "get_man.cpp"

#include "get_exp.cpp"

#include "get_sign.cpp"

#include "equals.cpp"

#include "gt.cpp"

#include "lt.cpp"

#include "gte.cpp"

#include "lte.cpp"

#include "neg.cpp"

#include "add_assign.cpp"

#include "add.cpp"

#include "sub_assign.cpp"

#include "sub.cpp"

#include "mul_assign.cpp"

#include "mul.cpp"

// template<size_t M, size_t E>
// F<M, E> F<M, E>::operator~() const
// {
//     return F<M, E>{this->d_data ^ s_exp_mask};
// }

#include "abs.cpp"

#include "ostream_shift.cpp"

#endif
