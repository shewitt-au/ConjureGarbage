#pragma once

#include <intrin.h>

// Hack
#define OS_WINDOWS

#if defined(OS_WINDOWS)
    #define DEBUGBREAK() __debugbreak()
#else
    #include <csignal>
    #define DEBUGBREAK() raise(SIGTRAP)
#endif

// Logical implication
inline bool imp(bool l, bool r) {
    return !(l && !r);
}

template <typename Predicate>
auto checked_pedicate(const Predicate pred) {
    return [=](const auto &l, const auto &r) {
        // Irreflexivity: !(x<x)
        if (pred(l,l))
            DEBUGBREAK();
        if (pred(r,r))
            DEBUGBREAK();

        // Asymmetry: if l<r not r<l
        if (!imp(pred(l,r), !pred(r,l)))
            DEBUGBREAK();
        if (!imp(pred(r,l), !pred(l,r)))
            DEBUGBREAK();

        return pred(l, r);
    };
}

// For all i in [b, e), pred(i, i+1) is true.
template <typename Iter, typename Predicate>
void transitivity(const Iter b, const Iter e, const Predicate pred)
{
    if (e-b < 2)
        return;

    for (Iter l=b; l<e-1; ++l) {
        for (Iter r=l+1; r<e; ++r) {
            if (!pred(*l, *r))
                DEBUGBREAK();
        }
    }
}

// For all i in [b, e), pred(i, i+1) is false and pred(i+1, i) is false.
// Incomparability is perhaps better undersood as equality.
template <typename Iter, typename Predicate>
void transitivity_of_incomparability(const Iter b, const Iter e, const Predicate pred)
{
    if (e-b < 2)
        return;

    for (Iter l=b; l<e-1; ++l) {
        for (Iter r=l+1; r<e; ++r) {
            if (pred(*l, *r) || pred(*r, *l))
                DEBUGBREAK();
        }
    }
}
