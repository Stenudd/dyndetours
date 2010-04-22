//========================================================================
// Includes
//========================================================================
#include "test_cdecl.h"
#include "dd_detourman.h"

//========================================================================
// Main entry point.
//========================================================================
int main( void )
{
	// Initialize the detour manager
	g_pDetourManager = new CDetourManager();

	// Begin testing!

	// __cdecl
	cdecl_begin();

	// Free up memory.
	delete g_pDetourManager;

	// Pause the command prompt.
	system("pause");

	// Done!
	return 0;
}