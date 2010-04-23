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
// Includes.
//========================================================================
#include "dd_callbackman.h"

//========================================================================
// Destructor.
//========================================================================
CCallBackManager::~CCallBackManager( void )
{
	// Loop through the vector and delete each
	// callback.
	for( unsigned int i = 0; i < m_vecCallBacks.size(); i++ )
	{
		// If the pointer is valid
		// free up the memory.
		if( m_vecCallBacks[i] )
		{
			delete m_vecCallBacks[i];
		}
	}
}

//========================================================================
// Adds a callback to our list.
//========================================================================
bool CCallBackManager::AddCallBack( ICallBack* pCallBack )
{
	// Make sure the callback instance
	// is valid.
	if( !pCallBack )
	{
		return false;
	}

	// Does a callback for this language already
	// exist?
	if( FindCallBack(pCallBack->GetLanguageName()) )
	{
		// If so, no further work needed.
		return true;
	}

	// Add it if it's not
	m_vecCallBacks.push_back(pCallBack);

	// We're done!
	return true;
}

//========================================================================
// Finds a callback by language name.
//========================================================================
ICallBack* CCallBackManager::FindCallBack( const char* szLanguage )
{
	// Make sure the language string is valid.
	if( !szLanguage )
	{
		return NULL;
	}

	// Loop through each callback
	for( unsigned int i = 0; i < m_vecCallBacks.size(); i++ )
	{
		// Grab ICallBack instance
		ICallBack* pCallBack = m_vecCallBacks[i];

		// Is it valid?
		if( !pCallBack )
		{
			continue;
		}

		// If it is, check the language
		if( strcmp(szLanguage, pCallBack->GetLanguageName()) == 0 )
		{
			return pCallBack;
		}
	}

	// If we're here, we couldn't find it
	return NULL;
}

//========================================================================
// Processes callbacks in our list.
//========================================================================
HookRes_t* CCallBackManager::ProcessCallBacks( CDetour* pDetour )
{
	// We will store the highest priority hook in this
	// variable.
	HookRes_t* pHighest = NULL;
	HookRes_t* pCurRes = NULL;

	// Loop through all the callbacks in the vector.
	for( unsigned int i = 0; i < m_vecCallBacks.size(); i++ )
	{
		// Get the ICallBack instance.
		ICallBack* pCallBack = m_vecCallBacks[i];

		// Sanity check
		if( !pCallBack )
		{
			continue;
		}

		// Tell it to process its callbacks.
		pCurRes = pCallBack->ProcessCallBack( pDetour );

		// Something went wrong
		if( !pCurRes )
		{
			// Move on to the next callback.
			continue;
		}

		// Is pHighest initialized?
		if( !pHighest )
		{
			// If not, set the current result to it.
			pHighest = pCurRes;

			// And iterate to the next callback.
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

		// If the priority is not higher than the
		// current highest, free the memory occupied
		// by pCurRes.
		else
		{
			delete pCurRes;
		}
	}

	// Return our highest priority result.
	return pHighest;
}