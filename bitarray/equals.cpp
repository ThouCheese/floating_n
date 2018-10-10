template<size_t N>
bool constexpr BitArray<N>::operator==(BitArray<N> const other) const
{
    for (size_t index = 0; index != d_length; ++index)
        if (this->d_data[index] != other.d_data[index])
            return false;
    return true;
}
