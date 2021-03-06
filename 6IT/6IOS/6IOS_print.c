/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "../6IOS.h"

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef _6IT_WIN32
#include <Windows.h>
#endif

_6IT_PRIVATE void bios_print_string(char const *s)
{
	fputs(s, stdout);

#ifdef _6IT_WIN32
	auto len = strlen(s);

	wchar_t *dest = malloc((len + 1) * sizeof(wchar_t));

	size_t t;
	mbstowcs_s(&t, dest, len + 1, s, len);

	OutputDebugString(dest);

	free(dest);
#endif

	// todo crz: think about this
#ifdef _6IT_ANDROID
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "6IT", __VA_ARGS__))
	LOGI(s);
#undef LOGI
#endif

}

_6IT_PRIVATE void bios_printf(char const *format, ...)
{
	static char b[2000]; // todo crz: sort this out

	va_list args;
	va_start(args, format);
	_6IT_VSPRINTF_STATIC_BUFFER(b, format, args);
	va_end(args);

	bios_print_string(b);
}
