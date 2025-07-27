#pragma once

#include <algorithm>

// Hack
#define OS_WINDOWS

#if defined(OS_WINDOWS)
    // __debugbreak is not seem reliable. It sometimes silently terminates
    // when debugging. The debugger? MingW64? I'm not sure who's to blame
    // but the Win32 function 'DebugBreak' seems to work reliably.
    //#define DEBUGBREAK() __debugbreak()

    #include <windows.h>
    #define DEBUGBREAK() DebugBreak()

    //#include <iostream>
    //#define DEBUGBREAK() std::cout << "DB" << std::endl

    //#define DEBUGBREAK() int crap=0
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

// Transitivity:
// For all x,y.z in [b, e)
//  if pred(x,y) and pred(y,z) are true then pred(x,z) is true
//
// All elements in [b, e) should have already been sorted so
// b[n]<b[n+1]. We'll assume this even though with a dogy predicate
// it may not be the case.
template <typename Iter, typename Predicate>
void transitivity(const Iter b, const Iter e, const Predicate pred)
{
    for (Iter l=b; l<e-2; ++l) {
        for (Iter r=l+2; r<e; ++r) {
            if (!pred(*l, *r)) {
                DEBUGBREAK();
                // For all x (at index n) in [b, e-1): 
                //  pred(b[n], b[n+1]) == true
                // pred(*l, *r) returned false however. This is in violation
                // of a strict weak ordering.
            }
        }
    }
}

// For all x,y.z in [b, e)
//  if !pred(x,y) && !pred(y,x) && !pred(y,z) && !pred(z,y)
//  then
//  !pred(x,z) && !pred(z,x)
//
// Incomparability is perhaps better undersood as equality.
template <typename Iter, typename Predicate>
void transitivity_of_incomparability(const Iter b, const Iter e, const Predicate pred)
{
    for (Iter l=b; l<e-2; ++l) {
        for (Iter r=l+2; r<e; ++r) {
            if (!(!pred(*l, *r) && !pred(*r, *l))) {
                DEBUGBREAK();
                // For all x (at index n) in [b, e-1): 
                //  pred(b[n], b[n+1]) == false && pred(b[n+1], b[n]) == false
                // (!pred(*l, *r) && !pred(*r, *l)) returned false however. This is in violation
                // of a strict weak ordering.
            }
        }
    }
}

template <typename Iter, typename Predicate>
void post_sort_check(const Iter b, const Iter e, const Predicate pred) {
    if (b==e)
        return;

    for (Iter l=b; l<e-1;) {
        if (pred(*l, *(l+1))) {
            Iter r = l+1;
            for (; r<e-1 && pred(*r, *(r+1)); ++r) {}
            transitivity(l, r+1, pred);
            l = r;
        } else if (!pred(*(l+1), *l)) {
            Iter r = l+1;
            for (; r<e-1 && !pred(*(r+1), *r) && !pred(*r, *(r+1)); ++r) {}
            transitivity_of_incomparability(l, r+1, pred);
            l = r;
        }
        else {
            // !pred(*l, *(l+1)) && pred(*(l+1), *l)
            // So l>=r && l>r
            DEBUGBREAK(); // NOT sorted!
            ++l;
        }
    }
}

template<typename RandomIt, typename Compare>
void checked_sort(RandomIt first, RandomIt last, Compare comp) {
    std::sort(first, last, checked_pedicate(comp));
    post_sort_check(first, last, comp);
}

template<typename RandomIt, typename Compare>
void checked_stable_sort(RandomIt first, RandomIt last, Compare comp) {
    std::stable_sort(first, last, checked_pedicate(comp));
}

#undef DEBUGBREAK
