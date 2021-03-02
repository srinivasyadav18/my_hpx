#include <iostream>

template<typename T>
T max(const T &a, const T &b)
{
    return a > b ? a : b;
}

int main()
{
    std::cout << max(2, 5);
}