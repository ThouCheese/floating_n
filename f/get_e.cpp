#include <iterator>

template<size_t M, size_t E>
F<M, E> constexpr F<M, E>::get_e() noexcept
{
    F<M, E> result = 0;
    F<M, E> factorial = 1;

    for (size_t iter = 0; iter != 50; ++iter)
    {
        if (iter != 0)
            factorial *= iter;
        result += ONE / factorial;
    }

    return result;
}
