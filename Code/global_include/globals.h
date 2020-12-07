#ifndef GLOBALS_H
#define GLOBALS_H

#define SUDOKU_SIZE 9
#define SQUARE_SIZE 3

#ifdef _WIN32
	#define DLL_EXPORT __declspec(dllexport)
#else
	#define DLL_EXPORT
#endif 

#endif // GLOBALS_H
