template<size_t N>
bool BitArray<N>::operator>(BitArray const other) const
{
    for (size_t index = d_length; index-- != 0;)
        if (this->d_data[index] > other.d_data[index])
            return true;
        else if (this->d_data[index] < other.d_data[index])
            return false;
    return false;
}
