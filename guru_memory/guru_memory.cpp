// guru_memory.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "guru_memory.h"


// This is an example of an exported variable
GURU_MEMORY_API int nguru_memory=0;

// This is an example of an exported function.
GURU_MEMORY_API int fnguru_memory(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see guru_memory.h for the class definition
Cguru_memory::Cguru_memory()
{
    return;
}
