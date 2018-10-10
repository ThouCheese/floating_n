template<size_t N, size_t P>
FixedPoint<N, P> constexpr FixedPoint<N, P>::operator*(FixedPoint<N, P> const other) const
{
    return FixedPoint<N, P>{*this} *= other;
}