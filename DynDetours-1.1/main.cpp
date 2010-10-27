#include <stdio.h>
#include <windows.h>
#include "cpp_manager.h"

int test_1( int a )
{
	a++;
	int b;
	b = a - 25;
	a--;
	a *= 10;
	printf("Inside lol.\n");
	return 24;
}

HookRetBuf_t* callback_1( CDetour* pDet )
{
	printf("Lucas is going to break his XDK lol.\n");
	HookRetBuf_t* pBuf = new HookRetBuf_t;
	
	pBuf->eRes = HOOKRES_OVERRIDE;
	pBuf->pRetBuf = (void *)30;
	return pBuf;
}

int __stdcall test_2( int a, int b, int c )
{
	a++;
	b = a - 25;
	a--;
	a *= 10;
	printf("Inside test_2.\n");
	return 24;
}

HookRetBuf_t* callback_2( CDetour* pDet )
{
	printf("Trololol.");
	HookRetBuf_t* pBuf = new HookRetBuf_t;

	pBuf->eRes = HOOKRES_OVERRIDE;
	pBuf->pRetBuf = (void *)30;
	return pBuf;
}

int main()
{
	// Create the callback.
	CPP_CreateCallback( &test_2, CONV_STDCALL, "iii)v", &callback_2, TYPE_PRE );

	printf("%d\n", test_2( 1, 2, 3 ));

	printf("OMG LAWL");

	system("pause");
	return 0;
}