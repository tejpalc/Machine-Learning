#include "stdafx.h"
#include "trackbar.h"


CTrackBar::	CTrackBar()
	{
		//InitCommonControls();
	}

CTrackBar::	~CTrackBar()
	{
		DestroyWindow(tbHWnd);
	}
	
bool CTrackBar:: Create(HWND pHWnd,UINT iLeft,UINT iTop,     
				UINT iWidth,UINT iHeight)
{
	tbHWnd = CreateWindowEx( 
	      0,                             // no extended styles 
        TRACKBAR_CLASS,                // class name 
        "Trackbar Control",            // title (caption) 
        WS_CHILD | WS_VISIBLE |		   // style 
        TBS_AUTOTICKS //| TBS_ENABLESELRANGE
        ,iLeft, iTop,                        // position 
        iWidth, iHeight,                       // size 
        pHWnd, (HMENU) 0, hInst , NULL);                       // parent window 
   
	if (!tbHWnd)
	{
		return FALSE;
	}

	return  TRUE;
}


void CTrackBar:: SetRange(UINT iMin,UINT iMax)
{
    SendMessage(tbHWnd, TBM_SETRANGE, 
	    (WPARAM) TRUE,               // redraw flag 
        (LPARAM) MAKELONG(iMin, iMax));  // min. & max. positions 
}

void CTrackBar:: SetTicFreq(UINT wFreq)
{
    SendMessage(tbHWnd, TBM_SETTICFREQ , 
	    (WPARAM) wFreq,               // frequency
        (LPARAM) 0);  
}

void CTrackBar::SetPageSize(UINT iPageSize)
{
	SendMessage(tbHWnd, TBM_SETPAGESIZE, 
	    0, (LPARAM) iPageSize);          // new page size 
}

void CTrackBar::SetSelection(UINT iSelMin,UINT iSelMax)
{
	SendMessage(tbHWnd, TBM_SETSEL, 
        (WPARAM) FALSE,                  // redraw flag 
        (LPARAM) MAKELONG(iSelMin, iSelMax)); 
}

void CTrackBar::SetPos(UINT iPos)
{
    SendMessage(tbHWnd, TBM_SETPOS, 
        (WPARAM) TRUE,                   // redraw flag 
        (LPARAM) iPos); 
}

UINT CTrackBar::GetPos()
{
	return SendMessage(tbHWnd,TBM_GETPOS,0,0);
}

