#include"stdafx.h"
#include"progbar.h"



	

CPBar::	CPBar()
{
//	InitCommonControls(); 
}


CPBar::~CPBar()
{
	DestroyWindow(pbHWnd); 
}

	
bool CPBar::	CreateProgBar(HWND pHwnd,int Left,int Top,
							  int width,int height)
{
	pbHWnd = CreateWindowEx(0, PROGRESS_CLASS, (LPSTR) NULL, 
    WS_CHILD | WS_VISIBLE /*|PBS_SMOOTH */, Left, 
    Top,width,height, 
    pHwnd, (HMENU) 0, hInst , NULL);

	if (!pbHWnd)
    {
       return FALSE;
    }

	return  TRUE;
}
 
void CPBar::SetPos(int newPos)
{
	SendMessage(pbHWnd, PBM_SETPOS, (WPARAM) newPos,0); 
}

int CPBar::SetRange(int min,int max)
{
	 // Set the range and increment of the progress bar. 
	return SendMessage(pbHWnd, PBM_SETRANGE, 0, MAKELPARAM(min, max)); 
}

void CPBar::	SetStepSize(int stepInc)
{
	SendMessage(pbHWnd, PBM_SETSTEP, (WPARAM) stepInc, 0); 
}

void CPBar::SetStep(int nStepInc)
{
	SendMessage(pbHWnd, PBM_SETSTEP, (WPARAM) nStepInc , 0); 
}

void CPBar::Step()
{
	SendMessage(pbHWnd, PBM_STEPIT, 0, 0); 
}

void CPBar::SetColor(COLORREF clrBar,COLORREF clrBk)
{
	SendMessage(pbHWnd,PBM_SETBARCOLOR ,0,(LPARAM)(COLORREF)clrBar);
	SendMessage(pbHWnd,PBM_SETBKCOLOR,0,(LPARAM)(COLORREF)clrBk);
}
