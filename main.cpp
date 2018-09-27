#include "f/f.h"

int main()
{
    F<64, 64> number2 = 1;
    for (size_t i : {1, 2, 3, 4, 5, 6, 7, 8, 9})
    {
        F<64, 64> number1 = i;
        F<64, 64> number3 = number2 + number1;
        std::cout << number1 << " + " << number2 << " = " << number3 << '\n';
    }
}
