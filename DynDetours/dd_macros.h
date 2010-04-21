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

#ifndef _DD_MACROS_H
#define _DD_MACROS_H

//========================================================================
// Includes
//========================================================================
#include "dd_definitions.h"

//========================================================================
// Declares a handler function for a hook.
//========================================================================
#define DECLARE_HOOK( name ) \
	HookRes_t* ##name##_hook( CDetour* pDet )

//========================================================================
// Use this macro if you are in a void function, or don't want to
// override.
//========================================================================
#define DYN_RETN_VOID() \
	HookRes_t* res = new HookRes_t; \
	res->action = HOOK_ACTION_NONE; \
	res->retVal = NULL; \
	return res;

//========================================================================
// Use this macro if you want to block a function from being called.
// Be sure to provide a return value.
//========================================================================
#define DYN_RETN( returnval ) \
	HookRes_t* res = new HookRes_t; \
	res->action = HOOK_ACTION_OVERRIDE; \
	res->retVal = (void *)returnval; \
	return res;

#endif // _DD_MACROS_H