#include <cmath>

enum consts : uint64_t
{
    zero =     0x0000000000000000,
    neg_zero = 0x8000000000000000,
    inf =      0x7ff0000000000000,
    neg_inf =  0xfff0000000000000,
    _nan =     0x7ff8000000000000,
    exp_bias = 0x00000000000003ff,
};

uint64_t constexpr _2pow52 = 0x10000000000000;

uint64_t constexpr f64_to_u64(double dbl)
{
    if (dbl == 0.0)
        return consts::zero;
    if (dbl == INFINITY)
        return consts::inf;
    if (dbl == -INFINITY)
        return consts::neg_inf;
    if (dbl != dbl)
        return consts::_nan;

    uint64_t sign = dbl < 0.0;
    double abs = sign ? -dbl : dbl;
    int64_t exp_signed = 52;

    while (abs >= _2pow52)
    {
        ++exp_signed;
        abs /= 2;
    }
    while (abs < _2pow52)
    {
        --exp_signed;
        abs *= 2;
    }
    uint64_t exp = exp_signed + exp_bias;
    uint64_t mantissa = static_cast<uint64_t>(abs);
    mantissa -= 0x10000000000000;
    return sign << 63 | exp << 52 | mantissa;
}