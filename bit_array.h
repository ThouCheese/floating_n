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
    constexpr BitArray<N> operator>>(int const n);
    constexpr BitArray<N> operator<<=(int const n);
    constexpr BitArray<N> operator<<(int const n);
    bool operator>(BitArray<N> const other);
    bool operator<(BitArray<N> const other);
    bool operator>=(BitArray<N> const other);
    bool operator<=(BitArray<N> const other);
    bool operator==(BitArray<N> const other);
    BitArray<N> operator&=(BitArray<N> const other);
    BitArray<N> operator&(BitArray<N> const other);
    BitArray<N> operator|=(BitArray<N> const other);
    BitArray<N> operator|(BitArray<N> const other);
    BitArray<N> operator^=(BitArray<N> const other);
    BitArray<N> operator^(BitArray<N> const other);
    BitArray<N> operator!();
    BitArray<N> operator+=(BitArray<N> const other);
    BitArray<N> operator+(BitArray<N> const other);
    BitArray<N> operator-=(BitArray<N> const other);
    BitArray<N> operator-(BitArray<N> const other);
    BitArray<N> operator*=(BitArray<N> const other);
    BitArray<N> operator*(BitArray<N> const other);
    BitArray<2*N> safe_mul(BitArray<N> const other);

    template<size_t PERIOD>
    BitArray<N> fixed_point_mul(BitArray<N> const other);
    size_t leading_zeros();

    long double to_ld();



    static constexpr BitArray<N> with_ones(size_t start, size_t end);
};

template<size_t N>
constexpr BitArray<N>::BitArray()
    :
        d_data{}
{}

template<size_t N>
template<class... T>
constexpr BitArray<N>::BitArray(T... numbers)
    :
        d_data{numbers...}
{}

template<size_t N>
constexpr BitArray<N> BitArray<N>::operator>>=(int const n)
{
    // use the left shift operator if we're shifting right by a negative amount
    if (n < 0)
        return *this << -n;
    // skip denotes the number of indexes in the array that we're shifting 
    // *this
    size_t const skip = n / 64;
    // shift denotes the size of the shift after we've selected the right 
    // index. For example, if we're shifting right a BitArray with 64-bit words
    // by 77, we only need to shift each wor=d by 13.
    size_t const shift = n % 64;
    // work from right to left starting at skip to make sure that reading and
    // writing don't interfere with eachother
    for (size_t index = skip; index != d_length; ++index)
    {
        size_t const target = index - skip;
        d_data[target] = (d_data[index] >> shift);
        // if we're not in the leftmost word, we need to include carry from the
        // word to the left
        if (index != d_length - 1)
            // if `shift` is zero, `WORD_LENGTH - shift` is too large for the 
            // shift operator, and it becomes UB, instead of all zeros. To 
            // ensure the correct behaviour, we multiply by `shift != 0`, and
            // we get the correct result
            d_data[target] |= (d_data[index + 1] << 
                (WORD_LENGTH - shift)) * (shift != 0);
    }
    return *this;
}

template<size_t N>
constexpr BitArray<N> BitArray<N>::operator>>(int const n)
{
    return BitArray<N>{*this} >>= n;
}

template<size_t N>
constexpr BitArray<N> BitArray<N>::operator<<=(int const n)
{
    // undocumented stuff is the same as in `operator>>`
    if (n < 0)
        return *this >> -n;

    size_t const skip = n / 64;
    size_t const shift = n % 64;

    // each word in `this` is written from the position `index` to `index + 
    // skip`. Therefore, the first word that is actually written to somewhere 
    // the the word at then index `d_length - skip`, so we start our get_loop 
    // there. We then work left to right all the way to zero.
    for (size_t index = d_length - skip; index != 0;)
    {
        // decrement outside the get_loop condition to reach zero
        --index;
        size_t const target = index + skip;
        d_data[target] = (d_data[index] << shift);
        // we have to include the carry from the word to the right
        if (index != 0)
            d_data[target] |= (d_data[index - 1] >> 
                (WORD_LENGTH - shift)) * (shift != 0);
    }

    // we now delete the leftmost bits that are not supposed to be remembered.
    // (eg a BitArray<100> will use two 64-bit words, but should keep all zeros
    // in the leftmost 28 bits.)
    WORD constexpr mask = 0xFFFFFFFFFFFFFFFF >> (d_length * WORD_LENGTH - N);
    d_data[d_length - 1] &= mask;
    return *this;
}

template<size_t N>
constexpr BitArray<N> BitArray<N>::operator<<(int const n)
{
    return BitArray<N>{*this} <<= n;
}

template<size_t N>
bool BitArray<N>::operator>(BitArray const other)
{
    for (size_t index = d_length; index != 0;)
    {
        --index;
        if (this->d_data[index] > other.d_data[index])
            return true;
        else if (this->d_data[index] < other.d_data[index])
            return false;
    }
    return false;
}

template<size_t N>
bool BitArray<N>::operator<(BitArray<N> const other)
{
    for (size_t index = d_length; index != 0;)
    {
        --index;
        if (this->d_data[index] < other.d_data[index])
            return true;
        else if (this->d_data[index] > other.d_data[index])
            return false;
    }
    return false;
}

template<size_t N>
bool BitArray<N>::operator>=(BitArray<N> const other)
{
    return not (*this < other);
}

template<size_t N>
bool BitArray<N>::operator<=(BitArray<N> const other)
{
    return not (*this > other);
}

template<size_t N>
BitArray<N> BitArray<N>::operator&=(BitArray<N> const other)
{
    for (size_t index = 0; index != d_length; ++index)
        d_data[index] = d_data[index] & other.d_data[index];
    return *this;
}

template<size_t N>
BitArray<N> BitArray<N>::operator&(BitArray<N> const other)
{
    return BitArray<N>{*this} &= other;
}

template<size_t N>
BitArray<N> BitArray<N>::operator|=(BitArray<N> const other)
{
    for (size_t index = 0; index != d_length; ++index)
        d_data[index] = d_data[index] | other.d_data[index];
    return *this;
}

template<size_t N>
BitArray<N> BitArray<N>::operator|(BitArray<N> const other)
{
    return BitArray<N>{*this} |= other;
}

template<size_t N>
BitArray<N> BitArray<N>::operator^=(BitArray<N> const other)
{
    for (size_t index = 0; index != d_length; ++index)
        d_data[index] = d_data[index] ^ other.d_data[index];
    return *this;
}

template<size_t N>
BitArray<N> BitArray<N>::operator^(BitArray<N> const other)
{
    return BitArray<N>{*this} ^= other;
}

template<size_t N>
BitArray<N> BitArray<N>::operator!()
{
    BitArray<N> result;
    for (size_t index = 0; index != d_length; ++index)
        result.d_data[index] = !this->d_data[index];
    return result;
}

template<size_t N>
BitArray<N> BitArray<N>::operator+=(BitArray<N> const other)
{
    size_t carry = 0;
    for (size_t index = 0; index != d_length; ++index)
    {
        // first overwrite the carry by true or false, so 1 or 0, depending on
        // whether `d_data[index] + carry` overflows. Store the result at
        // `result.d_data[index]`.
        carry = __builtin_add_overflow(d_data[index], carry, d_data + index);
        carry += __builtin_add_overflow(d_data[index], other.d_data[index],
                                        d_data + index);


    }
    return *this;
}

template<size_t N>
BitArray<N> BitArray<N>::operator+(BitArray<N> const other)
{
    return BitArray<N>{*this} += other;
}

template<size_t N>
BitArray<N> BitArray<N>::operator-=(BitArray<N> const other)
{
    bool carry = false;
    for (size_t index = 0; index != d_length; ++index)
    {
        carry = __builtin_sub_overflow(d_data[index], carry, d_data + index);
        carry += __builtin_sub_overflow(d_data[index], other.d_data[index],
                                        d_data + index);
    }
    return *this;
}

template<size_t N>
BitArray<N> BitArray<N>::operator-(BitArray<N> const other)
{
    return BitArray<N>{*this} -= other;
}

template<size_t WORD_LENGTH>
std::pair<WORD, WORD> mul_with_carry(WORD const a, WORD const b)
{
    auto lo = [&](WORD const number) 
        { return number & (static_cast<WORD>(-1) >> WORD_LENGTH / 2); };
    auto hi = [&](WORD const number) 
        { return number >> WORD_LENGTH / 2; };
    // use the wrapping behaviour of the normal overflow of unsigned numbers
    // to compute the result without too much involvement
    WORD result = a * b;
    // complex mathematics to compute the carry, I have a visual proof, email
    // me (or don't :) )
    WORD carry = hi(a) * hi(b) + hi(hi(a) * lo(b) + lo(a) * hi(b));
    return std::pair<WORD, WORD>(carry, result);
}

template<size_t N>
BitArray<N> BitArray<N>::operator*(BitArray<N> const other)
{
    // todo: as it stands this function prevents overflow by refusing to read
    // and write bits that are to significant and that may thus cause an 
    // overflow. It will produce incorrect results in the case of an overflow
    // by truncating the answer's most significant bits.
    // todo2: the loop uses a temporary BitArray to insert the computed values,
    // so that it automatically handles carrying. This ~might~ be slow. 
    // todo3: also implement operator*=

    // to perform this function we need to compute the carry of the 
    // multiplication of two 64-bit words. We do this by splitting t into two
    // seperate 32-bit words which are stored in 64-bit space, but have their
    // leftmost 32 bits empty. 
    auto result = BitArray<N>();

    // O(n^c), (1 < c < 2) which is pretty bad :(
    for (size_t self_index = 0; self_index != d_length; ++self_index)
    {
        for (size_t other_index = 0; other_index != d_length; ++other_index)
        {
            if (self_index + other_index > d_length)
                break;
            auto pair = mul_with_carry<WORD_LENGTH>(
                this->d_data[self_index], other.d_data[other_index]
            );
            BitArray<N> temp = BitArray<N>();
            temp.d_data[self_index + other_index] = pair.second;
            // prevent a segfault when writing the carry
            if (self_index + other_index + 1 < d_length)
                temp.d_data[self_index + other_index + 1] = pair.first;
            result = result + temp;
        }
    }
    return result;
}

template<size_t N>
BitArray<2*N> BitArray<N>::safe_mul(BitArray<N> const other)
{
    auto result = BitArray<2*N>();
    for (size_t self_index = 0; self_index != d_length; ++self_index)
    {
        for (size_t other_index = 0; other_index != d_length; ++other_index)
        {
            auto pair = mul_with_carry(this->d_data[self_index], 
                                       other.d_data[other_index]);
            BitArray<N> temp = BitArray<N>();
            temp.d_data[self_index + other_index] = pair.second;
            temp.d_data[self_index + other_index + 1] = pair.first;
            result = result + temp;
        }
    }
    return result;
}

/// Performs floating point multiplication with the period to the left of
/// `PERIOD`, but assums that everything to the left of `PERIOD` is set to 
/// zero. This allows for some optimizations, and is used by `fixed_point_mul`
template<size_t N, size_t PERIOD>
BitArray<N> floating_point_mul_internal(BitArray<N> const a,
                                        BitArray<N> const b)
{
    auto hi = [](BitArray<N> num)
        { return num >> (PERIOD / 2); };
    auto lo = [](BitArray<N> num)
        { return num & BitArray<N>::with_ones(0, PERIOD / 2); };
    return hi(a) * hi(b) + hi(hi(a) * lo(b) + lo(a) * hi(a));
}

/// Performs floating point multiplication with the period to the left of 
/// `PERIOD`. 
template<size_t N>
template<size_t PERIOD>
BitArray<N> BitArray<N>::fixed_point_mul(BitArray<N> const other)
{
    static_assert(N > PERIOD);
    // if we assume that the comma is between `PERIOD` and `PERIOD + 1`, we can
    // get the part before the comma by shifting right by `PERIOD` and the part
    // after the comma by performing bitwise and by `PERIOD` ones and then all 
    // zeros.
    BitArray<N> this_int = *this >> PERIOD;
    BitArray<N> this_flp = *this & BitArray<N>::with_ones(0, PERIOD);
    BitArray<N> other_int = other >> PERIOD;
    BitArray<N> other_flp = other & BitArray<N>::with_ones(0, PERIOD);

    return (this_int * other_int << PERIOD) + this_int * other_flp + 
        this_flp * other_int + 
        floating_point_mul_internal<N, PERIOD>(this_flp, other_flp);
}

template<size_t N>
size_t BitArray<N>::leading_zeros()
{
    for (size_t index = d_length; index != 0;)
    {
        --index;
        if (d_data[index] == 0)
            continue;
        for (size_t num_index = WORD_LENGTH; num_index != 0;)
        {
            --num_index;
            if (d_data[index] & (1 << num_index) != 0)
                return N - WORD_LENGTH * index - num_index;
        }
    }
    return N;
}

template<size_t N>
long double BitArray<N>::to_ld()
{
    long double result = 0;
    for (size_t index = 0; index != d_length; ++index)
        result += static_cast<long double>(d_data[index]) * 
            pow(2L, 64 * index);
    return result;
}

/// returns a `BitArray<N>` with all ones between the indexes, counting from
/// right to left.
template<size_t N>
constexpr BitArray<N> BitArray<N>::with_ones(size_t const start,
                                             size_t const end)
{
    // todo: make this compil
    // static_assert(start < end);
    // this function is slow, but it is only used in compile time. If 
    // compilation times become excessive, look here
    BitArray<N> result;
    for (size_t index = start; index != end; ++index)
    {
        size_t word_index = index % WORD_LENGTH;
        size_t word_num = index / WORD_LENGTH;
        result.d_data[word_num] |= static_cast<WORD>(1) << word_index;
    }
    return result;
}

template<size_t N>
std::ostream &operator<<(std::ostream &stream, BitArray<N> bit_array)
{
    std::cout << "0";
    char separator = 'x';
    for (size_t index = BitArray<N>::d_length; index != 0; --index)
    {
        std::cout << separator 
                  << std::setfill('0') 
                  << std::setw(16) 
                  << std::hex 
                  << bit_array.d_data[index-1];
        separator = '_';
    }
    return stream;
}

#endif