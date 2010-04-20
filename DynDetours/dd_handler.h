#ifndef _DD_HANDLER_H
#define _DD_HANDLER_H

//========================================================================
// Foward declarations.
//========================================================================
class CDetour;

//========================================================================
// This handles all of the callbacks registered to this library.
//========================================================================
int DynHandler( CDetour* pDet );

#endif // _DD_HANDLER_H