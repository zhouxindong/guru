// template_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "template/friend.h"

using namespace std;

int main()
{
	Friend_Demo<int> fdi;
	cout << fdi;
	show_me(fdi);
	show_me2(fdi);

	system("pause");
    return 0;
}

