template<size_t N>
constexpr std::ostream &operator<<(std::ostream &stream, BitArray<N> bit_array)
{
    std::cout << "0";
    char separator = 'x';
    for (size_t index = BitArray<N>::d_length; index-- != 0;)
    {
        std::cout << separator 
                  << std::setfill('0') 
                  << std::setw(16) 
                  << std::hex 
                  << bit_array.d_data[index];
        separator = '_';
    }
    return stream;
}