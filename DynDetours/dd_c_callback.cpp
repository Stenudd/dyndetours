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
#include "dd_c_callback.h"
#include "dd_detourman.h"

//========================================================================
// Adds a function to call back.
//========================================================================
bool C_CallBack::Add( void* pCallBack )
{
	// Make sure it's valid.
	if( !pCallBack )
	{
		return false;
	}

	// Add it to our internal callback list.
	m_vecCallBacks.push_back(pCallBack);

	// Done
	return true;
}

//========================================================================
// Removes the callback from our internal list.
//========================================================================
bool C_CallBack::Remove( void* pCallBack )
{
	// Can't remove NULL!
	if( !pCallBack )
	{
		return false;
	}

	// Loop through each element in the vector.
	vector<void *>::iterator i;
	for( i = m_vecCallBacks.begin(); i != m_vecCallBacks.end(); i++ )
	{
		// If they are equal, remove it
		if( (*i) == pCallBack )
		{
			// Get rid of the function pointer.
			m_vecCallBacks.erase(i);

			// Done!
			return true;
		}
	}

	// If we're here, we didn't find it!
	return false;
}

//========================================================================
// Processes callbacks for C functions.
//========================================================================
HookRes_t* C_CallBack::ProcessCallBack( CDetour* pDet ) 
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