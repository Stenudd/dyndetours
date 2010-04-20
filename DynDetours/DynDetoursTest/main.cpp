#include <stdio.h>
#include "dd_macros.h"
#include "dd_detourman.h"
#include "dd_c_callback.h"

//========================================================================
// This is the function we are going to hook.
//========================================================================
void test_void( int a, int b, int c )
{
	printf("%d %d %d\n", a, b, c);
	int l = a;
	l *= b;
	c -= 20;
	l++;
	b -= c;
	printf("Inside test_void!\n");
}

//========================================================================
// This is the function we're going to redirect to.
//========================================================================
HookRes_t* test_callback( CDetour* pDet )
{
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

	printf("Inside test_callback.\n");

	DYN_RETN_VOID();
}

//========================================================================
// This is the function we are going to hook. It returns a value.
//========================================================================
int test_int( int a, int b, int c )
{
	printf("%d %d %d\n", a, b, c);
	int l = a;
	l *= b;
	c -= 20;
	l++;
	b -= c;
	printf("Inside test_int!\n");
	return 20;
}

//========================================================================
// This is our callback for test_int.
//========================================================================
HookRes_t* test_callback2( CDetour* pDet )
{
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

	printf("Inside test_callback2.\n");

	// Block the function call. Use -19 as the return value.
	DYN_RETN( -19 );
}



int main( void )
{
	// Call functions without hooking.
	test_void( 0, 0, 0 );

	// Initialize the detour manager
	g_pDetourManager = new CDetourManager();

	// Add hooks
	C_HookFunction( &test_void, "iii)v", DYN_CALL_CONV_CDECL, &test_callback );
	C_HookFunction( &test_int, "iii)v", DYN_CALL_CONV_CDECL, &test_callback2);

	test_void( 0, 0, 0 );
	printf("Result: %d\n", test_int( 1, 1, 1 ));

	delete g_pDetourManager;

	system("pause");
	return 0;
}