//lint -library
#include "stdafx.h"
#include "mousepatch.h"

namespace NcLButtonUpHook
{
	CMap<HWND,HWND,BOOL,BOOL> hookWndMap;
	
	HHOOK hMHook = NULL;			// handle to the mouse hook
	BOOL bNcLButtonDown = FALSE;	// status of non-client left button down
	UINT hitTest = HTNOWHERE;

	// ////////////////////////////////////////////////////////////////////
	// Mouse hook process

	LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) 
	{ 
		if (nCode == HC_ACTION) 
		{ 
			// get a pointer to the mouse hook struct. 
			PMOUSEHOOKSTRUCT mhs = (PMOUSEHOOKSTRUCT) lParam; 

			// intercept messages for left button down and up
			switch(wParam) 
			{ 
			case WM_NCLBUTTONDOWN: 
				{					
					CMap<HWND,HWND,BOOL,BOOL>::CPair *pCurVal = hookWndMap.PLookup(mhs->hwnd);
 
					if (pCurVal)
					{
						pCurVal->value = TRUE;

						bNcLButtonDown = TRUE;
						hitTest = mhs->wHitTestCode;
					}
				}
				break;

			case WM_NCLBUTTONUP: 
				// you will get this message if you double-click on the title bar 
				// reset the status
				bNcLButtonDown = FALSE; 
				break; 

			case WM_LBUTTONUP: 
				{
					CMap<HWND,HWND,BOOL,BOOL>::CPair *pCurVal = hookWndMap.PLookup(mhs->hwnd);
					
					if (pCurVal && pCurVal->value)
					{
						if (bNcLButtonDown == TRUE)
						{
							::PostMessage(mhs->hwnd,WM_NCLBUTTONUP,hitTest,MAKELONG(mhs->pt.x, mhs->pt.y)); 
							bNcLButtonDown = FALSE;
							pCurVal->value = FALSE;
						}
					}
				} 
				break; 

			default: 
				break; 
			} 
		} 
		// let the	messages through to the next hook 
		return CallNextHookEx(hMHook, nCode, wParam, lParam); 
	}

	void SetHook(HWND hWnd)
	{		
		if (hookWndMap.IsEmpty() && NULL != AfxGetThread())
		{
			hMHook = SetWindowsHookEx(WH_MOUSE,(HOOKPROC)MouseHookProc,AfxGetInstanceHandle(),AfxGetThread()->m_nThreadID);
		}

		hookWndMap[hWnd] = FALSE;
	}

	void UnHook(HWND hWnd)
	{
		hookWndMap.RemoveKey(hWnd);

		if (hookWndMap.IsEmpty())
		{
			UnhookWindowsHookEx(hMHook);
		}
	}
}
