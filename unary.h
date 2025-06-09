#include <type_traits>
template <typename T>
using RemConstRef = std::remove_cv_t<std::remove_reference_t<T>>;

struct UnaryOpTags
{
    struct Sigmoid;
    struct Tanh;
    // ...
};


template <typename TOpTag, typename TData>
class UnaryOp
{
    static_assert((std::is_same_v<RemConstRef<TData>, TData>),
                    "TData is not an available types");

public:
    UnaryOp(TData data):m_data(std::move(data)){}

    // todo Eval

private:
    TData m_data;
};