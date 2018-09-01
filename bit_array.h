#ifndef def_included_bit_array_h
#define def_included_bit_array_h

#include <iostream>
#include <iomanip>

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
    constexpr BitArray(WORD const number);
    BitArray(std::initializer_list<WORD> const list);

    constexpr BitArray<N> operator>>(int const n);
    constexpr BitArray<N> operator<<(int const n);
    bool operator>(BitArray<N> const other);
    bool operator<(BitArray<N> const other);
    bool operator>=(BitArray<N> const other);
    bool operator<=(BitArray<N> const other);
    bool operator==(BitArray<N> const other);
    BitArray<N> operator&(BitArray<N> const other);
    BitArray<N> operator|(BitArray<N> const other);
    BitArray<N> operator^(BitArray<N> const other);
    BitArray<N> operator+(BitArray<N> const other);
    BitArray<N> operator-(BitArray<N> const other);
    BitArray<N> operator*(BitArray<N> const other);
    BitArray<2*N> safe_mul(BitArray<N> const other);
};

template<size_t N>
constexpr BitArray<N>::BitArray()
    :
        d_data({})
{}

// sets only the first word
template <size_t N>
constexpr BitArray<N>::BitArray(WORD const number)
    :
        d_data({})
{
    d_data[0] = number;
}

template<size_t N>
BitArray<N>::BitArray(std::initializer_list<WORD> const list)
    :
        d_data(list)
{}

template<size_t N>
constexpr BitArray<N> BitArray<N>::operator>>(int const n)
{
    // use the left shift operator if we're shifting right by a negative amount
    if (n < 0)
        return *this << -n;
    // skip denotes the number of indexes in the array that we're shifting 
    // *this
    size_t const skip = n / 64;
    // shift denotes the size of the shift after we've selected the right 
    // index. For example, if we're shifting right a BitArray with 64-bit words
    // by 77, we only need to shift each word by 13.
    size_t const shift = n % 64;
    BitArray<N> result;
    // work from right to left starting at skip to make sure that reading and
    // writing don't interfere with eachother
    for (size_t index = skip; index != d_length; ++index)
    {
        size_t const target = index - skip;
        result.d_data[target] = (this->d_data[index] >> shift);
        // if we're not in the leftmost word, we need to include carry from the
        // word to the left
        if (index != d_length - 1)
            // if `shift` is zero, `WORD_LENGTH - shift` is too large for the 
            // shift operator, and it becomes UB, instead of all zeros. To 
            // ensure the correct behaviour, we multiply by `shift != 0`, and
            // we get the correct result
            result.d_data[target] |= (this->d_data[index + 1] << 
                (WORD_LENGTH - shift)) * (shift != 0);
    }
    return result;
}

template<size_t N>
constexpr BitArray<N> BitArray<N>::operator<<(int const n)
{
    // undocumented stuff is the same as in `operator>>`
    if (n < 0)
        return *this >> -n;

    size_t const skip = n / 64;
    size_t const shift = n % 64;
    auto result = BitArray<N>();

    // each word in `this` is written from the position `index` to `index + 
    // skip`. Therefore, the first word that is actually written to somewhere 
    // the the word at then index `d_length - skip`, so we start our get_loop 
    // there. We then work left to right all the way to zero.
    for (size_t index = d_length - skip; index != 0;)
    {
        // decrement outside the get_loop condition to reach zero
        --index;
        size_t const target = index + skip;
        result.d_data[target] = (this->d_data[index] << shift);
        // we have to include the carry from the word to the right
        if (index != 0)
            result.d_data[target] |= (this->d_data[index - 1] >> 
                (WORD_LENGTH - shift)) * (shift != 0);
    }

    // we now delete the leftmost bits that are not supposed to be remembered.
    // (eg a BitArray<100> will use two 64-bit words, but should keep all zeros
    // in the leftmost 28 bits.)
    WORD constexpr mask = 0xFFFFFFFFFFFFFFFF >> (d_length * WORD_LENGTH - N);
    result.d_data[d_length - 1] &= mask;
    return result;
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
    for (size_t index = d_length; index != 0;)
    {
        --index;
        if (this->d_data[index] > other.d_data[index])
            return true;
        else if (this->d_data[index] < other.d_data[index])
            return false;
    }
    return true;
}

template<size_t N>
bool BitArray<N>::operator<=(BitArray<N> const other)
{
    for (size_t index = d_length; index != 0;)
    {
        --index;
        if (this->d_data[index] < other.d_data[index])
            return true;
        else if (this->d_data[index] > other.d_data[index])
            return false;
    }
    return true;
}

template<size_t N>
BitArray<N> BitArray<N>::operator&(BitArray<N> const other)
{
    auto result = BitArray<N>();
    for (size_t index = 0; index != d_length; ++index)
        result.d_data[index] = this->d_data[index] & other.d_data[index];
    return result;
}

template<size_t N>
BitArray<N> BitArray<N>::operator|(BitArray<N> const other)
{
    auto result = BitArray<N>();
    for (size_t index = 0; index != d_length; ++index)
        result.d_data[index] = this->d_data[index] | other.d_data[index];
    return result;
}

template<size_t N>
BitArray<N> BitArray<N>::operator^(BitArray<N> const other)
{
    auto result = BitArray<N>();
    for (size_t index = 0; index != d_length; ++index)
        result.d_data[index] = this->d_data[index] ^ other.d_data[index];
    return result;
}

template<size_t N>
BitArray<N> BitArray<N>::operator+(BitArray<N> const other)
{
    bool carry = false;
    auto result = BitArray<N>();
    for (size_t index = 0; index != d_length; ++index)
    {
        result.d_data[index] = 
            this->d_data[index] + other.d_data[index] + carry;

        // might be a slow method, but not sure how to make faster
        WORD constexpr MAX = -1;
        if (MAX - this->d_data[index] < other.d_data[index] or
                MAX - other.d_data[index] < this->d_data[index])
            carry = true;
        else
            carry = false;
    }
    return result;
}

template<size_t N>
BitArray<N> BitArray<N>::operator-(BitArray<N> const other)
{
    bool carry = false;
    auto result = BitArray<N>();
    for (size_t index = 0; index != d_length; ++index)
    {
        result.d_data[index] = 
            this->d_data[index] - other.d_data[index] - carry;
        if (this->d_data[index] < other.d_data[index])
            carry = true;
        else
            carry = false;
    }
    return result;
}

std::pair<WORD, WORD> mul_with_carry(WORD const a, WORD const b)
{
    auto lo = [](WORD const number) { return number & 0x00000000FFFFFFFF; };
    auto hi = [](WORD const number) { return number >> 32; };
    // use the wrapping behaviour of the normal overflow of unsigned numbers
    // to compute the result without too much involvement
    WORD result = a * b;
    // complex mathematics to compute the carry, I have a visual proof, email
    // me (or don't :))
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
            auto pair = mul_with_carry(this->d_data[self_index], 
                                       other.d_data[other_index]);
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
