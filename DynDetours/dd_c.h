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

#ifndef _DD_C_H
#define _DD_C_H

//========================================================================
// This is the C interface for dyndetours!
//========================================================================

//========================================================================
// Includes
//========================================================================
#include "dd_definitions.h"

//========================================================================
// Interface functions.
//========================================================================

/* @brief Native C functions will use this to hook functions.
 * @param pTarget   - Target function to hook.
 * @param szParams  - Parameter format string of the target.
 * @param conv      - Calling convention of target.
 * @param pCallBack - Function to redirect to. NULL if you want to use
 *					  the DynHandler function (RECOMMENDED)!
 * @return True if the callback was added successfully.
 */
extern bool AddCallBack(void* pTarget, char* szParams, eCallingConv conv, 
						void* pCallBack);

/* @brief  Native C functions will use this to remove a callback.
 * @param  pTarget   - The target function the callback is stored in.
 * @param  pCallBack - Function to remove from the callback.
 * @return True if removal was successful.
 */
extern bool RemoveCallBack(void* pTarget, void* pCallBack);


#endif // _DD_C_H