// trackbar.h
#ifndef trackbar_h
#define trackbar_h

#include<commctrl.h>

extern HINSTANCE hInst;

class CTrackBar
{
	HWND tbHWnd;

public:

	CTrackBar();
	
	~CTrackBar();
	
	bool Create(HWND pHWnd,UINT iLeft,UINT iTop,     
		UINT iWidth,UINT iHeight);

	void SetRange(UINT iMin,UINT iMax);

	void SetTicFreq(UINT wfreq);

	void SetPageSize(UINT iPageSize);

	void SetSelection(UINT iSelMin,UINT iSelMax);

	void SetPos(UINT iPos);

	UINT GetPos();
};

#endif
