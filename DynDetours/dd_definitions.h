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

#ifndef _DD_DEFINITIONS_H
#define _DD_DEFINITIONS_H

//========================================================================
// Definitions
//========================================================================

/* Max size of any string. */
#define MAX_STRING 255

//========================================================================
// Enumerations
//========================================================================

/* Different kinds of calling conventions. */
enum eCallingConv
{
	Convention_CDECL, /* __cdecl    */
	Convention_THIS,  /* __thiscall */
	Convention_FAST,  /* __fastcall */
};

/* Actions that a callback can take. Actions have priorities.
A higher priority action will always override a lower
priority one. */
enum eHookAction
{
	HOOK_ACTION_ERROR,	  /* Something went wrong. */
	HOOK_ACTION_NONE,     /* Call original function like normal. */
	HOOK_ACTION_MODIFY,	  /* The parameters of the original function were modified. 
						   * Call like normal. 
						   */
    HOOK_ACTION_OVERRIDE, /* Don't call original function. Use own return value. */
};

/* Different ways to detour a function. */
enum eDetourType
{
	Detour_JMP, /* Detoured using 5 byte jmp. */
};

//========================================================================
// Structures
//========================================================================

/* An instance of this structure is allocated and returned by each
callback function. This tells the library how to handle the hook. */
struct HookRes_t
{
	eHookAction action; /* Returned action the hook wants to take. */
	void*		retVal; /* Return value. This is NULL if the action is 
						 * NONE. If not, this value will be put into
						 * the appropriate register depending on the
						 * return value type. 
						 */
};

#endif // _DD_DEFINITIONS_H
