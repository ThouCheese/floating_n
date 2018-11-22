template<size_t N, size_t P>
constexpr FixedPoint<N, P>::FixedPoint()
{
    // this->d_data = std::move(source.d_data); // does not work?
    for (size_t index = 0; index != this->s_length; ++index)
        this->d_data[index] = 0;
    
}
