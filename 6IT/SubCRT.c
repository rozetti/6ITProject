/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#include "6IT.h"
#include "SubCRT.h"

#include "SubCRT/SubCRT_stdio.c"
#include "SubCRT/SubCRT_conio.c"

_6IT_PUBLIC void _6IT_MACHINECALL(bind_SubCRT)
{
	add_library_conio(_Machine);
	add_library_stdio(_Machine);
}