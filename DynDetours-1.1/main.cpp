#include <stdio.h>
#include "cpp_manager.h"
#include "detour_class.h"
#include "func_class.h"
#include "asmbridge_class.h"

unsigned long gVarOffset = 0;

class CMyEntity {
  
public:
  void Touch(int q) {
    gVarOffset = (unsigned long)&q;
    printf("Address of q %lu\n", gVarOffset);
  }
};

int test_1( int a )
{
  a++;
  int b;
  b = a - 25;
  a--;
  a *= 10;
  printf("Inside lol.\n");
  return 24;
}

HookRetBuf_t* callback_1( CDetour* pDet )
{
  printf("Lucas is going to break his XDK lol.\n");
  HookRetBuf_t* pBuf = new HookRetBuf_t;
  
  pBuf->eRes = HOOKRES_NONE;
  pBuf->pRetBuf = (void *)30;
  return pBuf;
}

HookRetBuf_t* callback_2( CDetour* pDet ) {
  
  // Get the ASM blob.
  CASMBridge* pBridge = pDet->GetAsmBridge();

  // Get at the calling convention.
  ICallConvention* pConv = pBridge->GetConv();
  
  // Get registers.
  CRegisterObj* pRegisters = pConv->GetRegisters();
  
  // Get at the stack pointer.
  unsigned long stk = pRegisters->r_esp;
  
  // Get at the this pointer.
  unsigned long pThisPtr = *(unsigned long *)(stk + 4);
  unsigned long iVal	 = *(unsigned long *)(stk + 8);

  //printf("The stack: %lu\n", stk);
  //printf("Offset of q: %lu\n", gVarOffset - stk);
  printf("%d -- %d\n", pThisPtr, iVal);

  HookRetBuf_t* pBuf = new HookRetBuf_t;

  pBuf->eRes = HOOKRES_NONE;
  pBuf->pRetBuf = 0;
  
  return pBuf;
}

int main()
{
  // Create the callback.
  // CPP_CreateCallback( (void *)&test_1, CONV_CDECL, "i)i", &callback_1, TYPE_PRE );

  // CPP_CreateCallback( (void *)&CMyEntity::Touch, CONV_THISCALL, "pi)v", &callback_2, TYPE_PRE );

  CMyEntity* pEntity = new CMyEntity();
  pEntity->Touch(30);
  
  printf("%d\n", pEntity);

  CPP_CreateCallback( (void *)&CMyEntity::Touch, CONV_THISCALL, "pi)v", &callback_2, TYPE_PRE );
  
  pEntity->Touch(30);
  
  return 0;
}
