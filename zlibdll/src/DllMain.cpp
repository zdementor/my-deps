//-------------------------------------------------------------
#include <windows.h>
//-------------------------------------------------------------
// Dll Entry Point
BOOL APIENTRY DllMain( 
    HANDLE hModule,
    DWORD  ulReasonForCall,
    LPVOID lpReserved 
    ) 
{
   switch( ulReasonForCall )
   {
   case DLL_PROCESS_ATTACH:
      // Allocate memory here, if you need.
      break;
   case DLL_THREAD_ATTACH:
   case DLL_THREAD_DETACH:
      // Not used.
      break;
   case DLL_PROCESS_DETACH:
      // Clean-up allocated memory here.
      break;
   }

   return TRUE;
}
//-------------------------------------------------------------