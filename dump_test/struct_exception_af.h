#pragma once

#include "dump/struct_exception.h"

void foo3()
{
	throw guru::struct_exception("test struct_exception");
}

void foo2()
{
	foo3();
}

void foo1()
{
	foo2();
}