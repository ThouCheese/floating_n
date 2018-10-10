#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#include "../bitarray/bit_array.h"

// N is the total length of the internal bit array, P is the place of the period
template<size_t N, size_t P>
class FixedPoint : public BitArray<N>
{

public:
    /*explicit*/ constexpr FixedPoint(BitArray<N> const &&source);
    FixedPoint<N, P> constexpr operator*=(FixedPoint<N, P> const other);
    FixedPoint<N, P> constexpr operator*(FixedPoint<N, P> const other) const;
};

// template<size_t N, size_t P>
// static_assert(N > P); // maybe allow gte?

#include "fixed_point1.cpp"

#include "mul_assign.cpp"
#include "mul.cpp"

// #include "ostream_shift.cpp"

#endif // FIXED_POINT_H
