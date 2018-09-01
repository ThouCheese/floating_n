#include "f.h"
#include <iostream>
#include <iomanip>

int main()
{
    // std::cout << F<20, 10>::sign_mask << '\n';
    auto bit_array = BitArray<101>();
    std::cout << bit_array << '\n';
}
