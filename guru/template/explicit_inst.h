#pragma once
template <typename T>
void f(T)
{
}

template void f<int>(int);
template void f<>(float);
template void f(long);
template void f(char);