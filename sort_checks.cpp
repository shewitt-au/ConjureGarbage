#include "sort_checks.hpp"

// Hack
#define OS_WINDOWS

#if defined(OS_WINDOWS)
    // __debugbreak does not seem reliable. It sometimes silently terminates
    // when debugging. The debugger? MingW64? I'm not sure who's to blame
    // but the Win32 function 'DebugBreak' seems to work reliably.
    //#define DEBUGBREAK() __debugbreak()

    //#include <windows.h>
    #include <iostream>
    void sortPredicateError(const char *pMsg)
    {
        //DebugBreak();
        //__debugbreak();
        std::cout << pMsg << std::endl;
    }

    //#include <iostream>
    //#define DEBUGBREAK() std::cout << "DB" << std::endl

    //#define DEBUGBREAK() int crap=0
#else
    #include <csignal>
    void sortPredicateError(const char *pMsg)
    {
        raise(SIGTRAP);
    }
    #define DEBUGBREAK() 
#endif
