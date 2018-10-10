#include <algorithm>
#include <iterator>

template<size_t N, size_t P>
constexpr FixedPoint<N, P>::FixedPoint(BitArray<N> const &&source)
{
    // this->d_data = std::move(source.d_data); // does not work?
    for (size_t index = 0; index != this->d_length; ++index)
        this->d_data[index] = std::move(source.d_data[index]);
}
