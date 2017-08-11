#pragma once

#ifndef __MOUSEPATCH_HDR_H__
#define __MOUSEPATCH_HDR_H__

namespace NcLButtonUpHook
{
	// ////////////////////////////////////////////////////////////////////
	// Mouse hook process
	
	void SetHook(HWND hWnd);

	void UnHook(HWND hWnd);
}


#endif // __MOUSEPATCH_HDR_H__