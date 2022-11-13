#!tcc -lpython311
#include <stdlib.h>
#include <E:/Interpreter/Python/include/Python.h>

int main(int argc, char **argv)
{
	putenv("PYTHONHOME=E:/Interpreter/Python/");
	Py_SetProgramName((const wchar_t *)argv[0]);
	Py_Initialize();
	PyRun_SimpleString("print('Hello Python!')");
	Py_Finalize();
	return 0;
}