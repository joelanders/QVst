#include "pluginterfaces/base/ftypes.h"
#include <windows.h>
#include <QVstApplication.h>

#if defined (_MSC_VER) && defined (DEVELOPMENT)
	#include <crtdbg.h>
#endif

#ifdef UNICODE
#define tstrrchr wcsrchr
#else
#define tstrrchr strrchr
#endif

//------------------------------------------------------------------------
HINSTANCE ghInst = 0;
void* moduleHandle = 0;
Steinberg::tchar gPath[MAX_PATH] = {0};

//------------------------------------------------------------------------
#define DllExport __declspec( dllexport )

//------------------------------------------------------------------------
extern bool InitModule ();		///< must be provided by Plug-in: called when the library is loaded
extern bool DeinitModule ();	///< must be provided by Plug-in: called when the library is unloaded

//------------------------------------------------------------------------
#ifdef __cplusplus 
extern "C" {
#endif

	bool DllExport InitDll () ///< must be called from host right after loading dll
	{ 		
		return InitModule (); 
	} 

	bool DllExport ExitDll ()  ///< must be called from host right before unloading dll
	{ 
		return DeinitModule (); 
	}
#ifdef __cplusplus 
} // extern "C"
#endif

//------------------------------------------------------------------------
BOOL WINAPI DllMain (HINSTANCE hInst, DWORD dwReason, LPVOID /*lpvReserved*/)
{
	if (dwReason == DLL_PROCESS_ATTACH)	{
	#if defined (_MSC_VER) && defined (DEVELOPMENT)
		_CrtSetReportMode ( _CRT_WARN, _CRTDBG_MODE_DEBUG );
		_CrtSetReportMode ( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
		_CrtSetReportMode ( _CRT_ASSERT, _CRTDBG_MODE_DEBUG );
		int flag = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG);
		_CrtSetDbgFlag (flag | _CRTDBG_LEAK_CHECK_DF);
	#endif

		moduleHandle = ghInst = hInst;

		// gets the path of the component
        if (GetModuleFileName (ghInst, gPath, MAX_PATH) > 0) {
			Steinberg::tchar* bkslash = tstrrchr (gPath, TEXT ('\\'));
            if (bkslash) {
				gPath[bkslash - gPath + 1] = 0;
            }
		}

        // Initialize QVst application instance
        QVstApplication::Flags flags = QVstApplication::Flag_None;
#ifdef QVST_CONSOLE
        flags |= QVstApplication::Flag_CreateConsole;
#endif
        QVstApplication::createInstance(hInst, flags);
    } else if (dwReason == DLL_PROCESS_DETACH) {
        // Delete QVst application instance
        delete qApp;
    }

	return TRUE;
}
