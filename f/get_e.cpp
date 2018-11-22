#include <iterator>

template<size_t M, size_t E>
F<M, E> constexpr F<M, E>::get_e() noexcept
{
    F<M, E> result = ZERO;
    F<M, E> factorial = ONE;
    // for (size_t iter = 0; iter != 30; ++iter)
    // {
    //     if (iter != 0)
    //         factorial *= iter;
    //     result += ONE / factorial;
    // }
    F<M, E> constexpr thirty = 30;
    for (F<M, E> iter = ZERO; iter != thirty; iter += ONE)
    {
        if (iter != ZERO)
            factorial *= iter;
        result += ONE / factorial;
    }
    return result;
}
    