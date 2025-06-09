// 例题：计算一个无符号整数的对应二进制中1的数量
#include <iostream>

int func(int input) {
    int one_count = 0;
    while (input > 0) {
        one_count += (input%2);
        input /=2;
    }
    return one_count;
}

int one_count2 = 0;
int func2(int input) {
    if (input == 0) {
        return 0;
    }
   return (one_count2 + input%2 + func2(input/2));
}


template<int input>
constexpr int one_count3 = (input%2) + one_count3<(input/2)>;

template<>
constexpr int one_count3<0> = 0;

constexpr int res = one_count3<7>;





int main() {
    // 111

    std::cout<<res<<std::endl;

}
