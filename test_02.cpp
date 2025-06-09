#include <iostream>
#include <string>

// 1、类型元函数
template <typename T>
struct Func
{
    using type = T;
};

template <>
struct Func<int>
{
    using type = int;
    using ref_type = int&;
    using const_ref_type = const int&;
};

Func<int>::type h = 100;
Func<int>::ref_type j = h;
Func<int>::const_ref_type k = 100; 

// 2、特殊的元函数
template <int a>
constexpr int fun = a+1;

static int c =1;
constexpr int func1(int a) {
    return a+c;
}

auto a =  fun<5>;

int main(){
    std::cout<<h<<std::endl;
    std::cout<<j<<std::endl;
    std::cout<<k<<std::endl;
    std::cout<<a<<std::endl;
}