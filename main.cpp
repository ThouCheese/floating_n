#include "f.h"

int main()
{
    F<64, 64> number{3.0};
    std::cout << number.d_data << '\n';
    std::cout << number << '\n';
}
