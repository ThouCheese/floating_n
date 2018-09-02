#include "f.h"
#include <iostream>
#include <iomanip>

int main()
{
    std::cout << BitArray<20>::with_ones(8, 12) << '\n';
}
