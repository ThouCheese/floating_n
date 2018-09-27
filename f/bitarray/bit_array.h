#ifndef def_included_bit_array_h
#define def_included_bit_array_h

#include <iostream>
#include <iomanip>
#include <cmath>

typedef uint64_t WORD;
/**
* Here N is the number of bits that the array has to store
*/
template<size_t N>
class BitArray
{
public:
    size_t static constexpr WORD_LENGTH = 64;
    size_t static constexpr d_length = (N + WORD_LENGTH - 1) / WORD_LENGTH;
    WORD d_data[d_length];

    constexpr BitArray();
    template<class... T>
    constexpr BitArray(T... ts);
    constexpr BitArray(BitArray const &other) = default;

    constexpr BitArray<N> operator>>=(int const n);
    constexpr BitArray<N> operator>>(int const n) const;
    constexpr BitArray<N> operator<<=(int const n);
    constexpr BitArray<N> operator<<(int const n) const;
    bool operator>(BitArray<N> const other) const;
    bool operator<(BitArray<N> const other) const;
    bool operator>=(BitArray<N> const other) const;
    bool operator<=(BitArray<N> const other) const;
    bool operator==(BitArray<N> const other) const;
    BitArray<N> operator&=(BitArray<N> const other);
    BitArray<N> operator&(BitArray<N> const other) const;
    BitArray<N> operator|=(BitArray<N> const other);
    BitArray<N> operator|(BitArray<N> const other) const;
    BitArray<N> operator^=(BitArray<N> const other);
    BitArray<N> operator^(BitArray<N> const other) const;
    BitArray<N> operator!() const;
    BitArray<N> operator+=(BitArray<N> const other);
    BitArray<N> operator+(BitArray<N> const other) const;
    BitArray<N> operator-=(BitArray<N> const other);
    BitArray<N> operator-(BitArray<N> const other) const;
    BitArray<N> operator*(BitArray<N> const other) const;
    // BitArray<2*N> safe_mul(BitArray<N> const other) const;

    template<size_t PERIOD>
    BitArray<N> fixed_point_mul(BitArray<N> const other) const;
    size_t leading_zeros() const;

    long double to_ld() const;

    static constexpr BitArray<N> with_ones(size_t start, size_t end);
};

#include "bit_array0.cpp"

#include "bit_array1.cpp"

#include "rshift_assign.cpp"

#include "rshift.cpp"

#include "lshift_assign.cpp"

#include "lshift.cpp"

#include "gt.cpp"

#include "lt.cpp"

#include "gte.cpp"

#include "lte.cpp"

#include "equals.cpp"

#include "bitand_assign.cpp"

#include "bitand.cpp"

#include "bitor_assign.cpp"

#include "bitor.cpp"

#include "bitxor_assign.cpp"

#include "bitxor.cpp"

#include "not.cpp"

#include "add_assign.cpp"

#include "add.cpp"

#include "sub_assign.cpp"

#include "sub.cpp"

#include "mul.cpp"

#include "fixed_point_mul.cpp"

#include "leading_zeros.cpp"

#include "to_ld.cpp"

#include "with_ones.cpp"

#include "ostream_shift.cpp"

#endif