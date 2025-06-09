#include "unary.h"

template <typename TP>
auto Sigmoid(TP&& p_m)
{
    using rawM = RemConstRef<TP>;
    using ResType = UnaryOp<UnaryOpTags::Sigmoid, rawM>;
    return ResType(std::forward<TP>(p_m));
}