#include <stdio.h>
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

int main()
{
	// Create the callback.
  CPP_CreateCallback( (void *)&test_1, CONV_CDECL, "i)i", &callback_1, TYPE_PRE );

	printf("%d\n", test_1( 2 ));

	printf("OMG LAWL");

	system("pause");
	return 0;
}
