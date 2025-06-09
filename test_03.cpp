#include <iostream>
namespace john {
    template<bool B, typename T> 
    struct enable_if{};

    template<typename T>
    struct enable_if<true, T> {
        using type = T;
    };

    template<bool B, typename T>
    using enable_if_t = typename enable_if<B, T>::type;
}

template <bool isValid, typename T, john::enable_if_t<isValid, T> * = nullptr>
int func(T &&t) {
    return 1;
}

template <bool isValid, typename T, john::enable_if_t<!isValid, T> * = nullptr>
int func(T &&t) {
    return 0;
}

// 2、借助特化去实现不同的分支





int main() {
    std::cout<<func<false, int>(100)<<std::endl;
}
