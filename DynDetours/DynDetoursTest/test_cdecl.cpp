//========================================================================
// Includes
//========================================================================
#include "test_cdecl.h" /* Need header for test functions. */
#include "dd_c.h"		/* C Interface for DynDetours.     */
#include "dd_macros.h"	/* Macros for DynDetours.		   */
#include <stdio.h>		/* Printf...					   */

//========================================================================
// No return value, no arguments.
//========================================================================
void cdecl_function1( void )
{
	int a = 20;
	int b = 20;
	int c = 30;
	double d = 20;

	a++;
	b++;
	c++;

	// Prologue
	printf("=================================\n");
	printf("Function: cdecl_function1\n");
	
	// Print out the values.
	printf("Locals: %d %d %d\n", a, b,c);
	printf("=================================\n\n");
}

//========================================================================
// No return value, three arguments.
//========================================================================
void cdecl_function2( int a, int b, int c )
{
	a++;
	b++;
	c++;

	// Prologue
	printf("=================================\n");
	printf("Function: \n cdecl_function2\n");

	// Print out the values.
	printf("Locals: %d %d %d\n", a, b,c);
	printf("=================================\n\n");
}

//========================================================================
// Integer return value, no arguments.
//========================================================================
int cdecl_function3( void )
{
	int a = 20;
	int b = 20;
	int c = 30;
	double d = 20;

	a++;
	b++;
	c++;

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
	a++;
	b++;
	c++;

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
DECLARE_HOOK( function1 )
{
	// Prologue
	printf("=================================\n");
	printf("Function: \n function1_hook\n");

	// Get stack pointer
	unsigned long r_esp = pDet->GetESP();

	// Get detour info
	CDetourInfo* pDetInfo = pDet->GetInfo();

	// Parse params and print
	int* paramOffsets = pDetInfo->GetOffsets();

	for( int i = 0; i < pDetInfo->GetNumParams(); i++ )
	{
		int paramVal = *((int *)(r_esp + paramOffsets[i]));
		printf("Parameter %d: %d\n", i, paramVal);
	}

	printf("=================================\n");

	// Void functions don't return.
	DYN_RETN_VOID();
}

//========================================================================
// Begins testing.
//========================================================================
void cdecl_begin()
{
	// Call the original functions.
	cdecl_function1();
	cdecl_function2(22, 32, 43);
	printf("Result of cdecl_function3: %d\n\n", cdecl_function3());
	printf("Result of cdecl_function4: %d\n\n", cdecl_function4(22, 32, 43));

	// Add the callback for the first function.
	AddCallBack(&cdecl_function1, "v)v", Convention_CDECL, &function1);

	// Now call the function
	cdecl_function1();
}