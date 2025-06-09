#include "sigmoid.h"
#include <stack>

template <typename TInputType>
struct InternalDataTypeCalculator
{
    using OutputType = decltype(Sigmoid(std::declval<TInputType>()));
    using type = std::stack<OutputType>;
};

template <typename TInputs>
class SigmoidLayer
{
public:
    // todo check input valid
    constexpr static bool IsFeedbackOutput = true;
    SigmoidLayer(std::string name)
        : m_name(std::move(name))
    {}

    auto FeedForward(TInputs&& p_in)
    {
        auto res = Sigmoid(p_in);

        if constexpr (IsFeedbackOutput)
        {
            m_data.push(res);
        }
        return res;
    }

    template <typename TGrad>
    auto FeedBackward(TGrad&& p_grad)
    {
        if constexpr (!IsFeedbackOutput)
        {
            // fake code
            return TInputs{};
        } else {
            auto input = m_data.top();
            auto res = SigmoidGrad(std::move(p_grad), std::move(input));            
            m_data.pop();
            return res;
        }
    }
    InternalDataTypeCalculator<TInputs> m_data;
    private:
     std::string m_name;
};