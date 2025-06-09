#include <iostream>

template <typename T>
struct Identity_
{
    using type = T;
};

template <typename TCont, typename... TValue>
struct PushBack_;

template <template <typename...> typename TCont, typename... TParams, typename... TValue>
struct PushBack_<TCont<TParams...>, TValue...>
{
    using type = TCont<TParams..., TValue...>;
};

template<typename TCont, typename... TValue>
using PushBack = typename PushBack_<TCont, TValue...>::type;

template <typename...T> struct ClauseSeq;

template <typename TLayerName, template<typename> class TLayer>
struct Sublayer
{
    using LayerName = TLayerName;

    template <typename TInputs>
    using LayerType = TLayer<TInputs>;
};

template <typename TOutLayerName, typename TOutPort, typename TInLayerName, typename TInPort>
struct InternalConnect
{
    using OutLayer = TOutLayerName;
    using OutPort = TOutPort;
    using InLayer = TInLayerName;
    using InPort = TInPort;
};

template <typename TInPort, typename TInLayerName, typename TInLayerPort>
struct InConnect
{
    using InPort = TInPort;
    using InLayerName = TInLayerName;
    using InLayerPort = TInLayerPort;
};

template <typename TOutLayerName, typename TOutLayerPort, typename TOutPort>
struct OutConnect
{
    using OutLayerName = TOutLayerName;
    using OutLayerPort = TOutLayerPort;
    using OutPort = TOutPort;
};


template <typename...TClauses>
struct SeparateClauses_
{
    template<typename TS, typename TI, typename TO, 
             typename TIt, typename... T>
    struct imp
    {
        static_assert(sizeof...(T) == 0);

        using SublayerRes = TS;
        using InConnectRes = TI;
        using OutConnectRes = TO;
        using InterConnectRes = TIt;
    };
    
    template <typename TS, typename TI, typename TO, typename TIt, 
              typename TLayerName, template<typename> class TLayer, typename...T>
    struct imp<TS, TI, TO, TIt, Sublayer<TLayerName, TLayer>, T...>
            : imp<PushBack<TS, Sublayer<TLayerName, TLayer>>,
                  TI, TO, TIt,
                  T...>
    {};
    
    template <typename TS, typename TI, typename TO, typename TIt,
              typename TInPort, typename TInLayerName, typename TInLayerPort,
              typename...T>
    struct imp<TS, TI, TO, TIt, InConnect<TInPort, TInLayerName, TInLayerPort>, T...>
            : imp<TS,
                  PushBack<TI, InConnect<TInPort, TInLayerName, TInLayerPort>>,
                  TO, TIt,
                  T...>
    {};
    
    template <typename TS, typename TI, typename TO, typename TIt,
              typename TOutLayerName, typename TOutLayerPort, typename TOutPort,
              typename...T>
    struct imp<TS, TI, TO, TIt, OutConnect<TOutLayerName, TOutLayerPort, TOutPort>, T...>
            : imp<TS, TI,
                  PushBack<TO, OutConnect<TOutLayerName, TOutLayerPort, TOutPort>>,
                  TIt,
                  T...>
    {};
    
    template <typename TS, typename TI, typename TO, typename TIt, 
              typename TOutLayerName, typename TOutPort, typename TInLayerName, typename TInPort,
              typename...T>
    struct imp<TS, TI, TO, TIt, 
               InternalConnect<TOutLayerName, TOutPort, TInLayerName, TInPort>, T...>
            : imp<TS, TI, TO,
                  PushBack<TIt, InternalConnect<TOutLayerName, TOutPort, TInLayerName, TInPort>>,
                  T...>
    { };
    
    using tmp = imp<ClauseSeq<>, ClauseSeq<>, ClauseSeq<>, ClauseSeq<>, TClauses...>;

    using SublayerRes = typename tmp::SublayerRes;
    using InterConnectRes = typename tmp::InterConnectRes;
    using InConnectRes = typename tmp::InConnectRes;
    using OutConnectRes = typename tmp::OutConnectRes;
};

template <typename...TComposeClauses>
struct Topology
{
    using Sublayers = typename SeparateClauses_<TComposeClauses...>::SublayerRes;
    using InterConnects = typename SeparateClauses_<TComposeClauses...>::InterConnectRes;
    using InputConnects = typename SeparateClauses_<TComposeClauses...>::InConnectRes;
    using OutputConnects = typename SeparateClauses_<TComposeClauses...>::OutConnectRes;
};

template <typename TK, typename TV>
struct KVBinder
{
    using KeyType = TK;
    using ValueType = TV;
    static TV apply(TK*);
};

template <auto N>
struct Int_
{
    constexpr static auto value = N;
};

template <typename TState, template <typename, typename> typename F,
          typename... TRemain>
struct imp_
{
    using type = TState;
};

template <typename TState, template <typename, typename> typename F, typename T0>
struct imp_<TState, F, T0>
{
    using type = F<TState, T0>;
};

template <typename TState, template <typename, typename> typename F,
          typename T0, typename T1>
struct imp_<TState, F, T0, T1>
{
    using type = F<F<TState, T0>, T1>;
};

template <typename TState, template <typename, typename> typename F,
          typename T0, typename T1, typename T2>
struct imp_<TState, F, T0, T1, T2>
{
    using type = F<F<F<TState, T0>, T1>, T2>;
};

template <typename TState, template <typename, typename> typename F,
          typename T0, typename T1, typename T2, typename T3>
struct imp_<TState, F, T0, T1, T2, T3>
{
    using type = F<F<F<F<TState, T0>, T1>, T2>, T3>;
};

template <typename TState, template <typename, typename> typename F,
          typename T0, typename T1, typename T2, typename T3, typename T4>
struct imp_<TState, F, T0, T1, T2, T3, T4>
{
    using type = F<F<F<F<F<TState, T0>, T1>, T2>, T3>, T4>;
};

template <typename TState, template <typename, typename> typename F,
          typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
struct imp_<TState, F, T0, T1, T2, T3, T4, T5>
{
    using type = F<F<F<F<F<F<TState, T0>, T1>, T2>, T3>, T4>, T5>;
};

template <typename TState, template <typename, typename> typename F,
          typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename... TRemain>
struct imp_<TState, F, T0, T1, T2, T3, T4, T5, T6, TRemain...>
{
    using type = typename imp_<F<F<F<F<F<F<F<TState, T0>, T1>, T2>, T3>, T4>, T5>, T6>,
                               F, TRemain...>::type;
};


template <typename TCon>
struct map_;

template <template <typename... > typename TCon, typename...TItem>
struct map_<TCon<TItem...>> : KVBinder<TItem, Int_<true>>...
{
    using KVBinder<TItem, Int_<true>>::apply ...;
    static Int_<false> apply(...);
};

template <typename TCon, typename TKey>
struct HasKey_
{
    constexpr static bool value = decltype(map_<TCon>::apply((TKey*)nullptr))::value;
    
};

template <typename TCon, typename TKey>
constexpr bool HasKey = HasKey_<TCon, TKey>::value;


// template <typename T>
// struct PrintType {
//     constexpr PrintType() {
//         // 这里不能使用 std::cout，因为它是运行时的
//         // 可以使用编译时断言输出信息
//         static_assert(std::is_same_v<T, T>, "Printing type: " __PRETTY_FUNCTION__);
//     }
// };

template <typename T>
struct PrintType {
    constexpr PrintType() {
        #pragma message("Printing type: " __PRETTY_FUNCTION__)
    }
};

// 用于打印布尔值信息的辅助模板
// template <bool B>
// struct PrintBool {
  
//     PrintBool() {
//         std::cout << "inInter: " << (B ? "true" : "false") << std::endl;
//     }
// };

#include <cstring> 
template<typename T>
struct TypeName {
    static std::string get() {
        const char* name = __PRETTY_FUNCTION__;
        // 提取类型名称（GCC/Clang 格式）
        const char* start = std::strstr(name, "T = ") + 4;
        const char* end = std::strrchr(start, ']');
        return std::string(start, end);
    }
};

// 2. 类型列表打印工具
template<typename TList>
struct TypeListPrinter;

template<>
struct TypeListPrinter<ClauseSeq<>> {
    static void print() { std::cout << std::endl; }
};

template<typename T, typename... Ts>
struct TypeListPrinter<ClauseSeq<T, Ts...>> {
    static void print() {
        std::cout << TypeName<T>::get() << ", ";
        TypeListPrinter<ClauseSeq<Ts...>>::print();
    }
};

template <typename TInterLayers, typename TSublayerOrdered, typename TSubLayerUnordered, typename...T>
struct SublayerPreprocess_
{
    using Ordered = TSublayerOrdered;
    using Unordered = TSubLayerUnordered;
        static void printOrderedTypes() {
        std::cout << "Ordered types: ";
        TypeListPrinter<Ordered>::print();
    }
    
    static void printUnorderedTypes() {
        std::cout << "Unordered types: ";
        TypeListPrinter<Unordered>::print();
    }
};

template <typename TInterLayers, typename TSublayerOrdered, typename TSubLayerUnordered, typename TCur, typename...T>
struct SublayerPreprocess_<TInterLayers, 
                            TSublayerOrdered,
                            TSubLayerUnordered,
                            TCur, T...>
{
    static constexpr bool inInter = HasKey<TInterLayers, typename TCur::LayerName>;
    // static constexpr PrintType<TCur> print_tcur();
    // 打印 inInter 布尔值信息
    // static constexpr PrintBool<inInter> print_inInter{};
    
    using NewOrdered = typename std::conditional_t<inInter,
                                                    Identity_<TSublayerOrdered>,
                                                    PushBack_<TSublayerOrdered, typename TCur::LayerName>>::type;
    using NewUnordered = typename std::conditional_t<inInter,
                                                        PushBack_<TSubLayerUnordered, typename TCur::LayerName>,
                                                        Identity_<TSubLayerUnordered>>::type;
                                                        
    using Ordered = typename SublayerPreprocess_<TInterLayers, NewOrdered, NewUnordered, T...>::Ordered;
    using Unordered = typename SublayerPreprocess_<TInterLayers, NewOrdered, NewUnordered, T...>::Unordered;
    static void printOrderedTypes() {
        std::cout << "Ordered types: ";
        TypeListPrinter<Ordered>::print();
    }
    
    static void printUnorderedTypes() {
        std::cout << "Unordered types: ";
        TypeListPrinter<Unordered>::print();
    }


};

template <typename TInitState, typename TInputCont, template <typename, typename> typename F>
struct Fold_;

template <typename TInitState, template<typename...> typename TCont, typename... TParams,
          template <typename, typename> typename F>
struct Fold_<TInitState, TCont<TParams...>, F>
{
    template <typename S, typename I>
    using FF = typename F<S, I>::type;
    
    using type = typename imp_<TInitState, FF, TParams...>::type;
};

template <typename TInitState, typename TInputCont, template <typename, typename> typename F>
using Fold = typename Fold_<TInitState, TInputCont, F>::type;

template <typename TCon, typename TKey, bool bMute = false>
struct Insert_ : PushBack_<TCon, TKey>
{
    static_assert(!HasKey<TCon, TKey>);
};

template <typename TCon, typename TKey>
struct Insert_<TCon, TKey, true>
{
    using type = typename std::conditional_t<HasKey<TCon, TKey>,
                                                Identity_<TCon>,
                                                PushBack_<TCon, TKey>>::type;
};

template <typename TCont1, typename TCont2>
struct Cascade_;

template <template <typename...> typename TCont, typename... TParams1, typename... TParams2>
struct Cascade_<TCont<TParams1...>, TCont<TParams2...>>
{
    using type = TCont<TParams1..., TParams2...>;
};

template <typename TCont1, typename TCont2>
using Cascade = typename Cascade_<TCont1, TCont2>::type;

template <typename TCon, typename TKey, bool bMute = false>
using Insert = typename Insert_<TCon, TKey, bMute>::type;

/// InternalLayerSet
template <typename TState, typename TInput>
using InternalInLayerSet_ = Insert_<TState, typename TInput::InLayer, true>;

template <typename TInternalClauses>
using InternalInLayerSet = Fold<ClauseSeq<>, TInternalClauses,
                                                         InternalInLayerSet_>;

template <typename TState, typename TInput>
using InternalOutLayerSet_ = Insert_<TState, typename TInput::OutLayer, true>;

template <typename TInternalClauses>
using InternalOutLayerSet = Fold<ClauseSeq<>, TInternalClauses,
                                                          InternalOutLayerSet_>;

template <typename TInternalClauses>
using InternalLayerSet = Fold<InternalInLayerSet<TInternalClauses>, TInternalClauses,
                                                       InternalOutLayerSet_>;


template <typename TArray>
struct Size_;

template <template <typename...> class Cont, typename...T>
struct Size_<Cont<T...>>
{
    constexpr static size_t value = sizeof...(T);
};

template <typename T>
using RemConstRef = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename TArray>
constexpr static size_t Size = Size_<RemConstRef<TArray>>::value;


template <typename TCheckInterSet, typename TRemainInters, typename TPostTags, typename...T>
struct InternalLayerPrune
{
    using PostTags = TPostTags;
    using RemainIters = TRemainInters;
};

template <typename TCheckInterSet, typename TRemainInters, typename TPostTags, typename TCur, typename...T3>
struct InternalLayerPrune<TCheckInterSet,
                            TRemainInters,
                            TPostTags,
                            TCur, T3...>
{
    static constexpr bool inInterIn = HasKey<TCheckInterSet, typename TCur::OutLayer>;
    
    using NewRemainInters = typename std::conditional_t<inInterIn,
                                                        PushBack_<TRemainInters, TCur>,
                                                        Identity_<TRemainInters>>::type;
    using NewTagContainer = typename std::conditional_t<inInterIn,
                                                        Identity_<TPostTags>,
                                                        Insert_<TPostTags, typename TCur::OutLayer, true>>::type;

    using nextStep = InternalLayerPrune<TCheckInterSet, NewRemainInters, NewTagContainer, T3...>;

    using RemainIters = typename nextStep::RemainIters;
    using PostTags = typename nextStep::PostTags;
};


namespace NSErase
{
    template <typename TCon, typename TKey, typename... TItems>
    struct Helper_
    {
        using type = TCon;
    };
    
    template <template <typename...> typename TCon, typename... TParams, typename TKey, typename TCur, typename... TItems>
    struct Helper_<TCon<TParams...>, TKey, TCur, TItems...>
    {
        using type = typename Helper_<TCon<TParams..., TCur>, TKey, TItems...>::type;
    };
    
    template <template <typename...> typename TCon, typename... TParams, typename TKey, typename... TItems>
    struct Helper_<TCon<TParams...>, TKey, TKey, TItems...>
    {
        using type = TCon<TParams..., TItems...>;
    };
}

template <typename TCon, typename TKey>
struct Erase_;

template <template <typename...> typename TCon, typename TKey, typename... TItems>
struct Erase_<TCon<TItems...>, TKey> : NSErase::Helper_<TCon<>, TKey, TItems...>
{};

template <typename TCon, typename TKey>
using Erase = typename Erase_<TCon, TKey>::type;


template <typename TOrderedSublayers, typename TUnorderedSublayers,
            typename TCheckInternals>
struct MainLoop
{
    static_assert(Size<TCheckInternals> == 0);
    using type = Cascade<TOrderedSublayers, TUnorderedSublayers>;
};

template <typename TOrderedSublayers, typename TUnorderedSublayers, typename TIC, typename...TI>
struct MainLoop<TOrderedSublayers,
                TUnorderedSublayers,
                ClauseSeq<TIC, TI...>>
{
    using InternalLayerPruneRes = 
        InternalLayerPrune<InternalInLayerSet<ClauseSeq<TIC, TI...>>,
                            ClauseSeq<>,
                            ClauseSeq<>, TIC, TI...>;

    using NewInter = typename InternalLayerPruneRes::RemainIters;
    using PostTags = typename InternalLayerPruneRes::PostTags;
    static_assert((Size<NewInter> < Size<ClauseSeq<TIC, TI...>>),
                    "Cycle exist in the compose layer");

    using NewOrdered = Cascade<TOrderedSublayers, PostTags>;
    using NewUnordered = Fold<TUnorderedSublayers, PostTags, Erase_>;
    using type = typename MainLoop<NewOrdered, NewUnordered, NewInter>::type;
};

template <typename TSublayerClause, typename TInterClause>
struct TopologicalOrdering_;

template <typename...TSublayers, typename TInterClause>
struct TopologicalOrdering_<ClauseSeq<TSublayers...>, TInterClause>
{
using SublayerPreRes = SublayerPreprocess_<InternalLayerSet<TInterClause>,
                                                      ClauseSeq<>, ClauseSeq<>, TSublayers...>;

// using type = typename MainLoop<typename SublayerPreRes::Ordered,
//                                         typename SublayerPreRes::Unordered,
//                                         TInterClause>::type;

};

template <typename... Ts>
void print_types(const char* label) {
    std::cout << label << ": ";
    ((std::cout << typeid(Ts).name() << " "), ...);
    std::cout << std::endl;
}

template <typename... Ts>
struct ClauseSeq {
    template <typename F>
    static void apply(F&& f) {
        f(Ts{}...);
    }
};

// 打印 Topology 中 Sublayers 的函数
template <typename TTopology>
void print_sublayers() {
    TTopology::Sublayers::apply([](auto... Ts) {
        print_types<decltype(Ts)...>("Sublayers");
    });
}

template <typename TTopology>
void print_interconnectlayers() {
    TTopology::InterConnects::apply([](auto... Ts) {
        print_types<decltype(Ts)...>("interconnectlayers");
    });
}

template <typename T> 
void print_after_process() {
    std::cout << typeid(T).name() << std::endl;
}
