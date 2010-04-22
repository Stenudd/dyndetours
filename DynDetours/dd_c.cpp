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
#include "dd_c.h"
#include "dd_c_callback.h"
#include "dd_detourman.h"

//========================================================================
// Adds a C function callback to the detour list.
//========================================================================
bool AddCallBack(void* pTarget, char* szParams, eCallingConv conv, 
				 void* pCallBack)
{
	// Sanity checking
	if( !pTarget || !szParams )
	{
		return NULL;
	}

	// This either creates or returns already existing detour.
	CDetour* pDetour = g_pDetourManager->Add_Detour( pTarget, szParams, conv );

	// See if we can find a C_CallBack instance for the C++ language
	// registered to this detour instance.
	C_CallBack* c = (C_CallBack *)pDetour->CallBack_Find( "C" );

	// If it exists, add our function pointer to it.
	if( c )
	{
		// Return the result of C_CallBack::Add.
		return c->Add(pCallBack);
	}

	// If the C_CallBack instance does not exist, create it.
	c = new C_CallBack();

	// Now add the function pointer to it.
	c->Add(pCallBack);

	// Now store it inside the detour instance.
	return pDetour->CallBack_Add(c);
}

//========================================================================
// Removes a callback from a detour.
//========================================================================
bool RemoveCallBack( void* pTarget, void* pCallBack )
{
	// Need a target and a callback to remove from it.
	if( !pTarget || !pCallBack )
	{
		return false;
	}

	// Find the detour for this target
	CDetour* pDet = g_pDetourManager->Find_Detour(pTarget);

	// If it doesn't exist don't bother
	if( !pDet )
	{
		return false;
	}

	// Find C language callback
	C_CallBack* pLangCallBack = (C_CallBack *)pDet->CallBack_Find("C");

	// If it's not valid, don't bother
	if( !pLangCallBack )
	{
		return false;
	}

	// Remove the callback
	return pLangCallBack->Remove(pCallBack);
}