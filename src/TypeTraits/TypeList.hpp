#pragma once

#include <type_traits>

namespace sb {
    template<class... Args>
    struct TypeList {};
  
  // BelongTo
    template<class T, class U>
    struct BelongTo;

    template<class T>
    struct BelongTo<T, TypeList<>> {
        static constexpr bool value = false;
        static constexpr size_t number = 0;
    };

    template<class T, class... Args>
    struct BelongTo<T, TypeList<T, Args ...>> {
        static constexpr bool value = true;
        static constexpr size_t number = 0;
    };

    template<class T, class U, class... Args>
    struct BelongTo<T, TypeList<U, Args ...>> {
        static constexpr bool value = BelongTo<T, TypeList<Args ...>>::value;
        static constexpr size_t number = BelongTo<T, TypeList<Args ...>>::number + 1;
    };
  // C_BelongTo
    template<class T, class TL>
    concept C_BelongTo = BelongTo<T, TL>::value;
  // Size
    template<class T>
    struct Size;

    template<>
    struct Size<TypeList<>> {
        static constexpr size_t value = 0;
    };

    template<class T, class ...Args>
    struct Size<TypeList<T, Args ...>> {
        static constexpr size_t value = Size<TypeList<Args ...>>::value + 1;
    };
  // Get
    template<size_t I, class T>
    struct Get;

    template<size_t I, class T>
    using GetT = typename Get<I, T>::type;

    template<class T, class... Args>
    struct Get<0, TypeList<T, Args ...>> {
        using type = T;
    };

    template<size_t I, class T, class... Args>
    struct Get<I, TypeList<T, Args ...>> {
        using type = GetT<I - 1, TypeList<Args ...>>;
    };
  // Concat
    template<class ...Args>
    struct Concat {};

    template<class ...Args>
    using Concat_t = typename Concat<Args ...>::type;

    template<class ...Args1, class ...Args2, class ...Args3>
    struct Concat<TypeList<Args1 ...>, TypeList<Args2 ...>, Args3 ...> {
        using type = Concat_t<TypeList<Args1 ..., Args2 ...>, Args3...>;
    };

    template<class ...Args>
    struct Concat<TypeList<Args ...>> {
        using type = TypeList<Args ...>;
    };
  // Merge
    template<class TL1, class TL2, template<class, class> class Less>
    struct Merge {};

    template<class TL1, class TL2, template<class, class> class Less>
    using Merge_t = typename Merge<TL1, TL2, Less>::type;

    template<template<class, class> class Less, class ... Args>
    struct Merge<TypeList<>, TypeList<Args ...>, Less> {
        using type = TypeList<Args ...>;
    };

    template<template<class, class> class Less, class ... Args>
    struct Merge<TypeList<Args ...>, TypeList<>, Less> {
        using type = TypeList<Args ...>;
    };

    template<template<class, class> class Less, class T, class U, class ... Args1, class ... Args2>
    struct Merge<TypeList<T, Args1 ...>, TypeList<U, Args2 ...>, Less> {
        static constexpr bool less = Less<T, U>::value;
        using MinType = std::conditional_t<less, T, U>;
        using TL1 = std::conditional_t<less, TypeList<Args1 ...>, TypeList<T, Args1 ...>>;
        using TL2 = std::conditional_t<less, TypeList<U, Args2 ...>, TypeList<Args2 ...>>;
        using type = Concat_t<TypeList<MinType>, Merge_t<TL1, TL2, Less>>;
    };
  // Split
    template<size_t Count, class TL>
    struct Split {};

    template<size_t Count, class TL>
    using Split_l = typename Split<Count, TL>::left;

    template<size_t Count, class TL>
    using Split_r = typename Split<Count, TL>::right;

    template<size_t Count, class A, class ... Args>
    struct Split<Count, TypeList<A, Args ...>> {
        using left = Concat_t<TypeList<A>, Split_l<Count - 1, TypeList<Args...>>>;
        using right = Split_r<Count - 1, TypeList<Args...>>;
    };

    template<class A, class ... Args>
    struct Split<0, TypeList<A, Args ...>> {
        using left = TypeList<>;
        using right = TypeList<A, Args ...>;
    };

    template<>
    struct Split<0,  TypeList<>> {
        using left = TypeList<>;
        using right = TypeList<>;
    };
  // Sort
    template<class T, template<class, class> class Less>
    struct Sort {};

    template<class U, template<class, class> class Less>
    using Sort_t = typename Sort<U, Less>::type;

    template<template<class, class> class Less>
    struct Sort<TypeList<>, Less> {
        using type = TypeList<>;
    };

    template<class U, template<class, class> class Less>
    struct Sort<TypeList<U>, Less> {
        using type = TypeList<U>;
    };

    template<class T, class U, class ...Args, template<class, class> class Less>
    struct Sort<TypeList<T, U, Args ...>, Less> {
        static constexpr size_t Size = ::sb::Size<TypeList<T, U, Args ...>>::value;
        using split = Split<Size / 2, TypeList<T, U, Args ...>>;
        using type = Merge_t<Sort_t<typename split::left, Less>, Sort_t<typename split::right, Less>, Less>;
    };
  // Compress
    template<class T, template<class, class> class Compressor>
    struct Compress {};

    template<class T, template<class, class> class Compressor>
    using Compress_t = typename Compress<T, Compressor>::type;

    template<template<class, class> class Compressor>
    struct Compress<TypeList<>, Compressor> {
        using type = TypeList<>;
    };
    
    template<class T, template<class, class> class Compressor>
    struct Compress<TypeList<T>, Compressor> {
        using type = TypeList<T>;
    };

    template<class T, class U, class ...Args, template<class, class> class Compressor>
    struct Compress<TypeList<T, U, Args ...>, Compressor> {
        static const bool canCompress = Compressor<T, U>::value;
        using TL = std::conditional_t<canCompress, TypeList<>, TypeList<T>>;
        using Next = std::conditional_t<canCompress, typename Compressor<T, U>::type, U>;
        using type = Concat_t<TL, Compress_t<TypeList<Next, Args ...>, Compressor>>;
    };
  // Transform
    template<class TL, template<class> class Transformer>
    struct Transform {};

    template<class T, template<class> class Transformer>
    using Transform_t = typename Transform<T, Transformer>::type;

    template<template<class> class Transformer>
    struct Transform<TypeList<>, Transformer> {
        using type = TypeList<>;
    };

    template<class T, class ...Args, template<class> class Transformer>
    struct Transform<TypeList<T, Args ...>, Transformer> {
        using type = Concat_t<TypeList<typename Transformer<T>::type>, Transform_t<TypeList<Args...>, Transformer>>;
    };
  // DeleteIf
    template<class TL, template<class> class Conditionerer>
    struct DeleteIf {};

    template<class T, template<class> class Conditioner>
    using DeleteIf_t = typename DeleteIf<T, Conditioner>::type;

    template<template<class> class Conditioner>
    struct DeleteIf<TypeList<>, Conditioner> {
        using type = TypeList<>;
    };

    template<class T, class ...Args, template<class> class Conditioner>
    struct DeleteIf<TypeList<T, Args ...>, Conditioner> {
        using TL = std::conditional_t<Conditioner<T>::value, TypeList<>, TypeList<T>>;
        using type = Concat_t<TL, DeleteIf_t<TypeList<Args ...>, Conditioner>>;
    };
} // namespace sb
