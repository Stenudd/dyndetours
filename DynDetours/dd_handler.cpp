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
	{
		return HOOK_ACTION_ERROR;
	}

	// We'll store function state information here.
	CFuncState* pState = pDet->GetState();

	// Make sure it's valid
	if( !pState )
	{
		return HOOK_ACTION_ERROR;
	}

	// We'll store the highest priority result
	// in this variable.
	HookRes_t* pHighest = pDet->Process_CallBacks();

	// If we got NULL back from the callbacks,
	// there was a problem.
	if( !pHighest )
	{
		return HOOK_ACTION_ERROR;
	}

	// Figure out what to do.
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
			pState->SetRetVal( pHighest->retVal );

			// Free memory
			delete pHighest;

			// Done
			return HOOK_ACTION_OVERRIDE;
		}
	}

	return HOOK_ACTION_ERROR;
}