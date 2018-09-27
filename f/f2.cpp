template<size_t M, size_t E>
constexpr F<M, E>::F(BitArray<s_bits> const bit_array) 
    : 
        d_data{bit_array}  
{}