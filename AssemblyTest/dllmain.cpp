// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

struct ImAPlakStruct
{
    int ky;
    char fak;
    bool plak;
};

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        ImAPlakStruct* s = new ImAPlakStruct();
        break;
    }
    return TRUE;
}

