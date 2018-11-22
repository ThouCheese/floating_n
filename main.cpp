#include "main.ih"

int main()
{
    std::cout << F<64, 64>::EULER << '\n';
    std::cout << F<64, 64>::PI << '\n';
    std::cout << F<64, 64>::PI.sin() << '\n';
}
