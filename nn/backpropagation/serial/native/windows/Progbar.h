// progbar.h

#ifndef _progbar_
#define _progbar_

#include <commctrl.h>

extern HINSTANCE hInst;

class CPBar
{
	HWND pbHWnd;

public:
	
	CPBar();
	
	~CPBar();

	void SetPos(int newPos);

	bool CreateProgBar(HWND pHwnd,int left,
		int top,int width,int height);

	int SetRange(int min,int max);
	
	void SetStepSize(int stepInc);

	void SetStep(int nStepInc);

	void Step();
	
	void SetColor(COLORREF clrBar=CLR_DEFAULT,
		COLORREF clrBk=CLR_DEFAULT);
};

#endif

