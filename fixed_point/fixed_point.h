#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#include "../bitarray/bit_array.h"

// N is the total length of the internal bit array, P is the place of the period
template<size_t N, size_t P>
class FixedPoint : public BitArray<N>
{
    static_assert(N > P);

    FixedPoint<N, P> static constexpr 
        ONE = BitArray<N>::with_ones(P, P + 1);

public:
    constexpr FixedPoint();
    /*explicit*/ constexpr FixedPoint(BitArray<N> const &&source);
    FixedPoint<N, P> constexpr operator*=(FixedPoint<N, P> const other);
    FixedPoint<N, P> constexpr operator*(FixedPoint<N, P> const other) const;
    
    template<size_t _N, size_t _P>
    friend std::ostream &operator<<(std::ostream &os, FixedPoint<_N, _P> fp);
};

template<size_t N, size_t P>
std::ostream &operator<<(std::ostream &os, FixedPoint<N, P> const fp);

#include "fixed_point1.cpp"
#include "fixed_point2.cpp"

#include "mul_assign.cpp"
#include "mul.cpp"

#include "ostream_shift.cpp"

#endif // FIXED_POINT_H
