#include "Stdafx.h"
/**
 * The main function is the entry point of our program. It's a rather small section of code
 * which initialises the application and runs it.
 * @file: Main.cpp
 * @author: Ryan Thorn
 * @date: 16/03/2017
 * @copyright: Copyright Ryan Thorn (c) 2017. All rights reserved.
 */

int main(int argc, char* argv[])
{
	return (Application::Instance()->Run() ? 0 : -1);
}