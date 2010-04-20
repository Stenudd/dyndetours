//////////////////////////////////////////////////////////////////////////
// File: dd_definitions.h
// Author: Deniz "your-name-here" Sezen
// Created: 7:45 PM, February 25, 2010
// Contributors: -
// Purpose: This file contains definitions and structures
//	        for use almost anywhere.
//////////////////////////////////////////////////////////////////////////
#ifndef _DD_DEFINITIONS_H
#define _DD_DEFINITIONS_H

//========================================================================
// Includes
//========================================================================
// ..

//========================================================================
// Definitions
//========================================================================

/* The size of the jump instruction that will override the
first five bytes of the target function. */
#define JMP_GATE_SIZE 5

/* Max size of any string. */
#define MAX_STRING 255

//========================================================================
// Enumerations
//========================================================================

/* Different kinds of calling conventions. */
enum eCallingConv
{
	DYN_CALL_CONV_CDECL, /* __cdecl */
	DYN_CALL_CONV_THIS,  /* __thiscall */
	DYN_CALL_CONV_FAST,  /* __fastcall */
};

/* Actions that a callback can take. Actions have priorities.
A higher priority action will always override a lower
priority one. */
enum eHookAction
{
	HOOK_ACTION_ERROR,	  /* Something went wrong. */
	HOOK_ACTION_NONE,     /* Do nothing. Original function will be called
						     like normal. */
    HOOK_ACTION_OVERRIDE, /* Overrides the original function, and uses the
						     supplied return value. */
};

/* Different ways to detour a function. */
enum eDetourType
{
	DET_TYPE_JMP, /* Detoured using 5 byte jmp. */
	DET_TYPE_RET, /* Detoured using ret addr. */
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
						NONE. If not, this value will be put into
						the appropriate register depending on the
						return value type. */
};

#endif // _DD_DEFINITIONS_H