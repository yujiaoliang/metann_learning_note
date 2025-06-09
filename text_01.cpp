#include <iostream>

// 1、无副作用：相同输入-》相同输出
// 2、在编译期调用和执行：

// 不是元函数
int func1(int a) {
    return a+1;
}

// 元函数
constexpr int func(int a) {
    return a+1;
}


// 即使带上了关键字，也不是
// static int c = 1;
// constexpr int func2(int a) {
//     return  a+c;
// }

int main() {
    func2(1);
}