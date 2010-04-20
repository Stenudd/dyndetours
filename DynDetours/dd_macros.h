//////////////////////////////////////////////////////////////////////////
// DynDetours ( C ) 2010 - Deniz Sezen
//  Useful macros for detouring functions.
//////////////////////////////////////////////////////////////////////////
#ifndef _DD_MACROS_H
#define _DD_MACROS_H

//========================================================================
// Includes
//========================================================================
#include "dd_definitions.h"

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