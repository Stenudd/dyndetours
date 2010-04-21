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

#ifndef _TEST_CDECL_H
#define _TEST_CDECL_H

//========================================================================
// Tests for hooking CDECL functions go here.
//========================================================================

//========================================================================
// Function prototypes that we're going to hook.
//========================================================================
void cdecl_function1( void );				 /* No return value. No arguments. */
void cdecl_function2( int a, int b, int c ); /* No return value. 3 arguments.  */

int cdecl_function3( void );				/* Return int, no arguments. */
int cdecl_function4( int a, int b, int c ); /* Return int, 3 arguments.  */

//========================================================================
// Begins the testing sequence.
//========================================================================
void cdecl_begin( void );

#endif // _TEST_CDECL_H