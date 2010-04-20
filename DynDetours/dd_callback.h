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
#ifndef _DD_CALLBACK_H
#define _DD_CALLBACK_H

//========================================================================
// Includes
//========================================================================
#include "dd_definitions.h"

//========================================================================
// Forward declarations.
//========================================================================
class CDetour;

//========================================================================
// Default callback interface which each language specific callback
// must inherit.
//========================================================================
class ICallBack
{
	public:
		/* @brief Processes every function callback inside this class's
		 *	internal list.
		 * @return The result of the highest priority callback.
		 */
		virtual HookRes_t* ProcessCallBack( CDetour* pDet ) = 0;

		/* @brief Accessor for the language name this callback represents.
		 * @return String containing the name of the language this callback
		 *	is meant for.
		 */
		virtual const char* GetLanguageName( void ) = 0;
};

#endif // _DD_CALLBACK_H