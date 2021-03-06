template<size_t N>
bool constexpr BitArray<N>::operator<(BitArray<N> const other) const
{
    for (size_t index = s_length; index-- != 0;)
        if (this->d_data[index] < other.d_data[index])
            return true;
        else if (this->d_data[index] > other.d_data[index])
            return false;
    return false;
}
