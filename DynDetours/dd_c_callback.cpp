//========================================================================
// Includes
//========================================================================
#include "dd_c_callback.h"
#include "dd_detourman.h"

//========================================================================
// Creates and/or adds a detour to a function.
//========================================================================
C_Callback* C_HookFunction( void* pTarget, char* szParams, eCallingConv conv, 
					void* pCallBack )
{
	// Sanity checking
	if( !pTarget || !szParams )
		return NULL;

	// This either creates or returns already existing detour.
	CDetour* pDetour = g_pDetourManager->Add_Detour( pTarget, szParams, conv );

	// Checks to see if there is a callback for the C language
	C_Callback* c = (C_Callback *)pDetour->CallBack_Find( "C" );

	// If it exists, add our function to it.
	if( c )
	{
		c->Add(pCallBack);
		return c;
	}

	// If it doesn't exist add it
	c = new C_Callback();
	c->Add(pCallBack);
	
	if( pDetour->CallBack_Add((ICallBack *)c) )
		printf("Added C callback successfully.\n");
	
	return c;
}

#if 0
//========================================================================
// Removes a callback from a detour.
//========================================================================
bool C_RemoveHook( void* pTarget, void* pCallBack )
{
	if( !pTarget || !pCallBack )
		return false;

	// Find the detour for this target
	CDetour* pDet = NULL;
	Detour_t* d = g_pDetourManager->Find_Detour(pTarget);

	// If the result is valid
	if( d )
		pDet = d->pDetour;

	// If it doesn't exist don't bother
	if( !pDet )
		return false;

	// Find C language callback
	C_Callback* pLangCallBack = (C_Callback *)pDet->CallBack_Find("C");

	// If it's not valid, don't bother
	if( !pLangCallBack )
		return false;

	// Remove the callback
	pLangCallBack->Remove(pCallBack);
}
#endif

//========================================================================
// Processes callbacks for C functions.
//========================================================================
HookRes_t* C_Callback::ProcessCallBack( CDetour* pDet ) 
{
	HookRes_t* pHighest = NULL;
	HookRes_t* pCurRes  = NULL;

	// Loop through all the callbacks in the vector.
	for( unsigned int i = 0; i < m_vecCallBacks.size(); i++ )
	{
		// Each callback needs the following prototype:
		// HookRes_t* function( CDetour* pDet );
		C_CallBackFunc c = (C_CallBackFunc)m_vecCallBacks[i];

		// Sanity check
		if( !c )
			continue;

		// Call it
		pCurRes = c( pDet );

		// Something went wrong
		if( !pCurRes )
		{
			// Move on to the next function call.
			continue;
		}

		// Is pHighest initialized?
		if( !pHighest )
		{
			// If not, set the current result to it.
			pHighest = pCurRes;

			// And iterate to the next function call.
			continue;
		}

		// Check priority
		if( pCurRes->action >= pHighest->action )
		{
			// Get rid of the memory from the old result.
			delete pHighest;

			// Set it to the new result.
			pHighest = pCurRes;
		}

		else
		{
			delete pCurRes;
		}
	}

	return pHighest;
}