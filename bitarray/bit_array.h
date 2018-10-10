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
    explicit constexpr BitArray(T... ts);
    constexpr BitArray(BitArray const bitand other) = default;

    constexpr BitArray<N> operator>>=(int const n);
    constexpr BitArray<N> operator>>(int const n) const;
    constexpr BitArray<N> operator<<=(int const n);
    constexpr BitArray<N> operator<<(int const n) const;
    bool constexpr operator>(BitArray<N> const other) const;
    bool constexpr operator<(BitArray<N> const other) const;
    bool constexpr operator>=(BitArray<N> const other) const;
    bool constexpr operator<=(BitArray<N> const other) const;
    bool constexpr operator==(BitArray<N> const other) const;
    constexpr BitArray<N> operator&=(BitArray<N> const other);
    constexpr BitArray<N> operator&(BitArray<N> const other) const;
    constexpr BitArray<N> operator|=(BitArray<N> const other);
    constexpr BitArray<N> operator|(BitArray<N> const other) const;
    constexpr BitArray<N> operator^=(BitArray<N> const other);
    constexpr BitArray<N> operator^(BitArray<N> const other) const;
    constexpr BitArray<N> operator~() const;
    constexpr BitArray<N> operator+=(BitArray<N> const other);
    constexpr BitArray<N> operator+(BitArray<N> const other) const;
    constexpr BitArray<N> operator-=(BitArray<N> const other);
    constexpr BitArray<N> operator-(BitArray<N> const other) const;
    constexpr BitArray<N> operator*(BitArray<N> const other) const;

    constexpr size_t leading_zeros() const;

    constexpr long double to_ld() const;

    static constexpr BitArray<N> with_ones(size_t start, size_t end);
};

#include "bit_array0.cpp"
#include "bit_array1.cpp"

#include "rshift_assign.cpp"
#include "rshift.cpp"

#include "lshift_assign.cpp"
#include "lshift.cpp"

#include "gt.cpp"
#include "gte.cpp"

#include "lt.cpp"
#include "lte.cpp"

#include "equals.cpp"

#include "bitand_assign.cpp"
#include "bitand.cpp"

#include "bitor_assign.cpp"
#include "bitor.cpp"

#include "bitxor_assign.cpp"
#include "bitxor.cpp"

#include "bitnot.cpp"

#include "add_assign.cpp"
#include "add.cpp"

#include "sub_assign.cpp"
#include "sub.cpp"

#include "mul.cpp"

#include "leading_zeros.cpp"

#include "to_ld.cpp"

#include "with_ones.cpp"

#include "ostream_shift.cpp"

#endif