// guru.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "guru.h"


// This is an example of an exported variable
GURU_API int nguru=0;

// This is an example of an exported function.
GURU_API int fnguru(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see guru.h for the class definition
Cguru::Cguru()
{
    return;
}
