template<size_t N, size_t P>
std::ostream &operator<<(std::ostream &os, FixedPoint<N, P> const fp)
{
    return os << (fp / FixedPoint<N, P>::base);
}
