/**
* ======================================================
* DynDetours
* Copyright (C) 2009 Deniz Sezen
* All rights reserved.
* ======================================================
*
* This software is provided 'as-is', without any express or implied warranty.
* In no event will the authors be held liable for any damages arising from 
* the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose, 
* including commercial applications, and to alter it and redistribute it 
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not 
* claim that you wrote the original software. If you use this software in a 
* product, an acknowledgment in the product documentation would be 
* appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*
*/

//========================================================================
// Includes
//========================================================================
#include <stdio.h>		/* Printf...					   */
#include "test_cdecl.h" /* Need header for test functions. */
#include "dd_c.h"		/* C Interface for DynDetours.     */
#include "dd_macros.h"	/* Macros for DynDetours.		   */

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
	printf("\n\n=================================");
	printf("\nFunction: cdecl_function1");
	
	// Print out the values.
	printf("\n Locals: %d %d %d", a, b,c);
	printf("\n=================================");
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
	printf("\n\n=================================");
	printf("\nFunction: cdecl_function2");

	// Print out the values.
	printf("\n Locals: %d %d %d", a, b,c);
	printf("\n=================================");
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
	printf("\n\n=================================");
	printf("\nFunction: cdecl_function3");

	// Print out the values.
	printf("\n Locals: %d %d %d", a, b,c);
	printf("\n=================================");

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
	printf("\n\n=================================");
	printf("\nFunction: cdecl_function4");

	// Print out the values.
	printf("\n Locals: %d %d %d", a, b,c);
	printf("\n=================================");

	return 29;
}

//========================================================================
// Hook for function1.
//  NOTE: Calls the original function like normal.
//========================================================================
DECLARE_HOOK( function1_hook )
{
	// Prologue
	printf("\n\n=================================");
	printf("\nFunction: function1_hook");
	printf("\n=================================");

	// Void functions don't return.
	// Don't override. Call function like normal.
	DYN_RETN_VOID();
}

//========================================================================
// Hook for function1.
//  NOTE: Does not call the original function.
//========================================================================
DECLARE_HOOK( function1_override )
{
	printf("\n\n=================================");
	printf("\nFunction: function1_override");
	printf("\n=================================");

	// We don't want to call cdecl_function1.
	DYN_RETN(NULL);
}

//========================================================================
// Hook for function2.
//  NOTE: Calls the original function like normal.
//========================================================================
DECLARE_HOOK( function2_hook )
{
	// Prologue
	printf("\n\n=================================");
	printf("\nFunction: function2_hook\n");

	// Get info manager
	CDetourInfo* pInfo = pDet->GetInfo();

	// Get function state variables
	CFuncState* pState = pDet->GetState();

	// Make sure both are valid
	if( !pInfo || !pState )
	{
		// Return.
		DYN_RETN_VOID();
	}

	// Loop through the parameter format string.
	char* ch = pInfo->GetParams();
	for( int i = 0; i < pInfo->GetNumParams(); i++ )
	{
		switch( *ch )
		{
			case 'i':
			{
				int arg;
				arg = *(int *)(pState->GetESP() + pInfo->GetOffsets()[i]);
				printf(" Arg[%d]: int, %d\n", i, arg);
			}
		}
	}


	printf("=================================");

	// Void functions don't return.
	// Don't override. Call function like normal.
	DYN_RETN_VOID();
}

//========================================================================
// Hook for function2.
//  NOTE: Does not call the original function.
//========================================================================
DECLARE_HOOK( function2_override )
{
	printf("\n\n=================================");
	printf("\nFunction: function2_override");
	printf("\n=================================");

	// We don't want to call cdecl_function2.
	DYN_RETN(NULL);
}

//========================================================================
// Begins testing.
//========================================================================
void cdecl_begin()
{
	// -----------------------------
	// Call the original functions.
	// -----------------------------
	cdecl_function1();
	cdecl_function2(22, 32, 43);
	printf("\nResult of cdecl_function3: %d", cdecl_function3());
	printf("\nResult of cdecl_function4: %d", cdecl_function4(22, 32, 43));

	// -----------------------------
	// Hook cdecl_functions!
	// -----------------------------
	printf("\n\n********************************************************");
	printf("\n* Phase 1:											 *");
	printf("\n*  Single hook. Call functions like normal.            *");
	printf("\n********************************************************\n");

	AddCallBack(&cdecl_function1, "v)v", Convention_CDECL, &function1_hook);
	AddCallBack(&cdecl_function2, "iii)v", Convention_CDECL, &function2_hook);
	
	// -----------------------------
	// Now call the hooked functions.
	// The hooks should call them
	// after finishing.
	// -----------------------------
	cdecl_function1();
	cdecl_function2(1,2,3);

	// -----------------------------
	// Now add the override callback.
	// -----------------------------
	printf("\n\n********************************************************");
	printf("\n* Phase 2:											 *");
	printf("\n*  Two hooks. Do not call the original function.       *");
	printf("\n********************************************************");

	AddCallBack(&cdecl_function1, "v)v", Convention_CDECL, &function1_override);
	AddCallBack(&cdecl_function2, "iii)v", Convention_CDECL, &function2_override);

	// -----------------------------
	// Call cdecl_function1.
	// The override should prevent
	// the original function from 
	// being called.
	// -----------------------------
	cdecl_function1();

	// -----------------------------
	// Test to make sure we don't
	// crash.
	// -----------------------------
	printf("\n\nI made it!\n");
}