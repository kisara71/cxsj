#include <iostream>
#include <Windows.h>
#include "Starter.h"
#include "Database.h"
int main()
{
	system("chcp 65001");
	Starter* starter = Starter::create_starter();
	return 0;
}