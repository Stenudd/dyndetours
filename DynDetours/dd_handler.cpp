//========================================================================
// Includes
//========================================================================
#include "dd_handler.h"
#include "dd_detour.h"
#include "dd_definitions.h"

//========================================================================
// Handler definition
//========================================================================
int DynHandler( CDetour* pDet )
{
	// Sanity check
	if( !pDet )
		return HOOK_ACTION_ERROR;

	// We'll store the highest priority result
	// in this variable.
	HookRes_t* pHighest = pDet->Process_CallBacks();

	if( !pHighest )
		return HOOK_ACTION_ERROR;

	switch(pHighest->action)
	{
		/* Call the function like normal. */
		case HOOK_ACTION_NONE:
		{
			// Free memory
			delete pHighest;

			// Call the original function
			return HOOK_ACTION_NONE;
		}

		/* Block function, use our return value. */
		case HOOK_ACTION_OVERRIDE:
		{
			// Set the retbuffer value
			pDet->SetRet( pHighest->retVal );

			// Free memory
			delete pHighest;

			// Done
			return HOOK_ACTION_OVERRIDE;
		}
	}

	return HOOK_ACTION_ERROR;
}