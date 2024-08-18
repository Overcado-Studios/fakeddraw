// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#include <crtdbg.h>
#endif

#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include "singleton.h"

// Disable specific warnings if necessary
#pragma warning(disable: 4786)

#define NVASSERT(a, b) \
do { \
    static bool bIgnore = false; \
    if (!bIgnore && ((int)(a) == 0)) { \
        std::ostringstream strOut; \
        strOut << b << "\nAt: " << __FILE__ << ", " << __LINE__; \
        OutputDebugString(strOut.str().c_str()); \
        int Ret = MessageBoxEx(NULL, strOut.str().c_str(), _T("NVASSERT"), MB_ABORTRETRYIGNORE, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)); \
        if (Ret == IDIGNORE) { \
            bIgnore = true; \
        } else if (Ret == IDABORT) { \
            __debugbreak(); \
        } \
    } \
} while (0)

void test()
{
    NVASSERT(0, "foo");
}