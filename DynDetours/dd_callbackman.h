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

#ifndef _DD_CALLBACKMAN_H
#define _DD_CALLBACKMAN_H

//========================================================================
// Includes
//========================================================================
#include <vector>
#include "dd_callback.h"

//========================================================================
// Forward Declarations.
//========================================================================
class CDetour;

//========================================================================
// Namespaces we'll need.
//========================================================================
using namespace std;

//========================================================================
// The callback manager class.
//========================================================================
class CCallBackManager
{
	private:
		vector<ICallBack *> m_vecCallBacks;

	public:

		/* @brief Destructor. */
		~CCallBackManager( void );

		/* @brief Adds a callback to this detour.
		 * @param pCallBack - Language specific callback instance to add to our
		 *	internal list.
		 * @return True if the callback was successfully added or if it is
		 *	already present in the list.
		 */
		bool AddCallBack( ICallBack* pCallBack );

		/* @brief Finds a callback by language name.
		 * @param szLanguage - Language the callback handles.
		 * @return ICallBack pointer which handles callbacks for the given
		 *	language.
		 */
		ICallBack* FindCallBack( const char* szLanguage );

		/* @brief Processes all the callbacks stored in this detour.
		 * @param pDetour - CDetour instance to pass into all callbacks.
		 * @return The highest priority hook structure.
		 */
		HookRes_t* ProcessCallBacks( CDetour* pDetour );
};

#endif // _DD_CALLBACKMAN_H