#include <cmath>
#include <iostream>
#include <iomanip>
#include <bitset>

int main()
{
    double inf = 3;
    uint64_t num = *reinterpret_cast<uint64_t *>(&inf);
    std::cout << std::bitset<64>(num) << '\n';
}