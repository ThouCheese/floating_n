#include "f.h"

int main()
{
    F<64, 64> number1 = 500;
    F<64, 64> number2 = 700;
    F<64, 64> number3 = number1 * number2;
    std::cout << number1 << " * " << number2 << " = " << number3 << '\n';
}
