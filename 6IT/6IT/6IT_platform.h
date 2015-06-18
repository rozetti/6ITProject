/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

// todo crz: more of this
#if __APPLE__ && __MACH__
#define _6IT_OSX
#endif

#ifdef _WIN32
#define _6IT_WIN32
#endif

#ifdef _6IT_WIN32
#define _6IT_WIN32_SAFE_CRT
#endif

#ifdef _6IT_WIN32_SAFE_CRT
#define _6IT_VSPRINTF_STATIC_BUFFER(DEST, FORMAT, ARGS) vsprintf_s(DEST, sizeof(DEST), FORMAT, ARGS)
#else
#define _6IT_VSPRINTF_STATIC_BUFFER(DEST, FORMAT, ARGS) vsprintf(DEST, FORMAT, ARGS)
#endif

#ifdef __ANDROID__
#define _6IT_ANDROID
#endif

#if defined(_6IT_WIN32)
#include <direct.h>
#define _6IT_GetCurrentDirectory _getcwd
#define _6IT_Fileno _fileno
#elif defined(_6IT_ANDROID)
#include <unistd.h>
#define _6IT_GetCurrentDirectory getcwd
#define _6IT_Fileno _fileno
#elif defined(_6IT_OSX)
#include <unistd.h>
#define _6IT_GetCurrentDirectory getcwd
#define _6IT_Fileno fileno
#else
#pragma message "we don't know what platform we are running on"
#endif
