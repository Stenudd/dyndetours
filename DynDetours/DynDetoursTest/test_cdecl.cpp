//========================================================================
// Includes
//========================================================================
#include "test_cdecl.h"
#include "dd_detour.h"

//========================================================================
// No return value, no arguments.
//========================================================================
void cdecl_function1( void )
{
	int a = 20;
	int b = 20;
	int c = 30;
	double b = 20;

	a <<= 30;
	b >>= 45;
	c *= 30;

	// Prologue
	printf("=================================\n");
	printf("Function: \n cdecl_function1\n");
	
	// Print out the values.
	printf("Locals: %d %d %d\n", a, b,c);
	printf("=================================\n");
}

//========================================================================
// No return value, three arguments.
//========================================================================
void cdecl_function2( int a, int b, int c )
{
	a <<= 30;
	b >>= 45;
	c *= 30;

	// Prologue
	printf("=================================\n");
	printf("Function: \n cdecl_function2\n");

	// Print out the values.
	printf("Locals: %d %d %d\n", a, b,c);
	printf("=================================\n");
}

//========================================================================
// Integer return value, no arguments.
//========================================================================
int cdecl_function3( void )
{
	int a = 20;
	int b = 20;
	int c = 30;
	double b = 20;

	a <<= 30;
	b >>= 45;
	c *= 30;

	// Prologue
	printf("=================================\n");
	printf("Function: \n cdecl_function3\n");

	// Print out the values.
	printf("Locals: %d %d %d\n", a, b,c);
	printf("=================================\n");

	return 30;
}

//========================================================================
// Integer return value, three arguments.
//========================================================================
int cdecl_function4( int a, int b, int c )
{
	a <<= 30;
	b >>= 45;
	c *= 30;

	// Prologue
	printf("=================================\n");
	printf("Function: \n cdecl_function4\n");

	// Print out the values.
	printf("Locals: %d %d %d\n", a, b,c);
	printf("=================================\n");

	return 29;
}

//========================================================================
// Hook for function1.
//========================================================================
DECLARE_HOOK( function1, )