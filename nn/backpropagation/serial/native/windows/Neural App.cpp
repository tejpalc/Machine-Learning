// Neural App.cpp : Defines the entry point for the application.
//

#define WIN32_LEAN_AND_MEAN
#include "stdafx.h"
#include "resource.h"
#include <process.h>
#include"backprop.h"
#include"brain.h"
#include "filedlgs.h"

#define MAX_LOADSTRING 100

// Global Variables:
CBrain *brain;			//  *Brain of appl*
HWND g_hWnd;							// main window
HINSTANCE hInst;						// current instance
TCHAR szTitle[MAX_LOADSTRING];			// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];	// The title bar text
char buff[255];
bool bRunThread;

// Foward declarations of functions included in this code module:
VOID				worker(PVOID param);	// Thread Procedure
void				DisplayErr(char *err);
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_NEURALAPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_NEURALAPP);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}




//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW ;//| CS_OWNDC;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_NEURALAPP);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_NEURALAPP;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_BORDER|WS_MINIMIZEBOX|WS_SYSMENU,
      CW_USEDEFAULT, 0, /*CW_USEDEFAULT*/700,/*0*/480, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd; // init global handle

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	HMENU hMenu;
	static int LearningMethod=0;// intially bp method
	static DWORD menuStateTop=MF_CHECKED;
	static HANDLE hWorkerThread;
	switch (message) 
	{

		case WM_CREATE:
			brain = new CBrain; // (((( DELETE ))))
			hMenu=GetMenu(hWnd);
			EnableMenuItem(hMenu,IDM_CONFIGURE,MF_BYCOMMAND|MF_GRAYED);//ON NEW
			EnableMenuItem(hMenu,IDM_TRAIN,MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(hMenu,IDM_TEST,MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(hMenu,IDM_PAUSE,MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(hMenu,IDM_RESUME,MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(hMenu,IDM_TERMINATE,MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(hMenu,IDM_SAVE,MF_BYCOMMAND|MF_GRAYED);//ON RETURN OF CONFIG
			EnableMenuItem(hMenu,IDM_SAVEAS,MF_BYCOMMAND|MF_GRAYED);//--,,--
			break;

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_NEW:
					hMenu=GetMenu(hWnd);
					if(LearningMethod==0){
						brain = (CBackProp *) new  CBackProp;// (((( delete ))))
						brain->create();
					}
					else if(LearningMethod==1){
						DisplayErr("Gentic Algorithm not implemented");
					}

					if(brain->configure()){
						hMenu=GetMenu(hWnd);
						EnableMenuItem(hMenu,IDM_TRAIN,MF_BYCOMMAND|MF_ENABLED);
						EnableMenuItem(hMenu,IDM_TEST,MF_BYCOMMAND|MF_ENABLED);
						EnableMenuItem(hMenu,IDM_TERMINATE,MF_BYCOMMAND|MF_ENABLED);
						EnableMenuItem(hMenu,IDM_SAVE,MF_BYCOMMAND|MF_ENABLED);//ON RETURN OF CONFIG
						EnableMenuItem(hMenu,IDM_SAVEAS,MF_BYCOMMAND|MF_ENABLED);//--,,--
						EnableMenuItem(hMenu,IDM_CONFIGURE,MF_BYCOMMAND|MF_ENABLED);
						brain->create();
						
					}

					break;

				case IDM_OPEN:
					GetOpenFileNameDlg(hWnd,"Specify Wieght File","bpwt","weight file",buff,255);
					brain->fileOpen();
					break;

				case IDM_SAVE:
					brain->fileSave();
					break;

				case IDM_SAVEAS:
					GetSaveFileNameDlg(hWnd,"Save Neural Net",".bpwt","weight file",buff,255);
					brain->fileSaveAs();
					break;

				case IDM_TRAIN:
					hMenu=GetMenu(hWnd);
					EnableMenuItem(hMenu,IDM_PAUSE,MF_BYCOMMAND|MF_ENABLED);
					EnableMenuItem(hMenu,IDM_TERMINATE,MF_BYCOMMAND|MF_ENABLED);
					GetOpenFileNameDlg(hWnd,"Specify Training Data Set","data set","dat",buff,255);
					//MessageBox(hWnd,buff," ",0);
					bRunThread=TRUE;
					hWorkerThread=(HANDLE)_beginthread(worker,0,buff);
					//brain->setTrainFpath(buff);
					//brain->train(buff);
					break;

				case IDM_TEST:
					hMenu=GetMenu(hWnd);
					EnableMenuItem(hMenu,IDM_PAUSE,MF_BYCOMMAND|MF_ENABLED);
					EnableMenuItem(hMenu,IDM_TERMINATE,MF_BYCOMMAND|MF_ENABLED);
					//GetOpenFileNameDlg(hWnd,"Specify Test Data Set","dat","data set",buff,255);
					
					break;

				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				
				case IDM_EXIT:
				   PostMessage(hWnd,WM_CLOSE,0,0);
					//DestroyWindow(hWnd);
				   break;

				case IDM_CONFIGURE:
					if(brain->configure()){
						hMenu=GetMenu(hWnd);
						EnableMenuItem(hMenu,IDM_TRAIN,MF_BYCOMMAND|MF_ENABLED);
						EnableMenuItem(hMenu,IDM_TEST,MF_BYCOMMAND|MF_ENABLED);
						EnableMenuItem(hMenu,IDM_TERMINATE,MF_BYCOMMAND|MF_ENABLED);
						EnableMenuItem(hMenu,IDM_SAVE,MF_BYCOMMAND|MF_ENABLED);//ON RETURN OF CONFIG
						EnableMenuItem(hMenu,IDM_SAVEAS,MF_BYCOMMAND|MF_ENABLED);//--,,--
						EnableMenuItem(hMenu,IDM_CONFIGURE,MF_BYCOMMAND|MF_ENABLED);

						brain->create();
					}

					break;

				case IDM_BACKPROP:
					hMenu=GetMenu(hWnd);
					CheckMenuRadioItem(hMenu,IDM_BACKPROP,IDM_GENETIC,IDM_BACKPROP,MF_BYCOMMAND);
					EnableMenuItem(hMenu,IDM_CONFIGURE,MF_BYCOMMAND|MF_ENABLED);
					LearningMethod=0;
					break;

				case IDM_GENETIC:
					hMenu=GetMenu(hWnd);
					CheckMenuRadioItem(hMenu,IDM_BACKPROP,IDM_GENETIC,IDM_GENETIC,MF_BYCOMMAND);
					EnableMenuItem(hMenu,IDM_CONFIGURE,MF_BYCOMMAND|MF_ENABLED);
					LearningMethod=1;
					break;

				
				case IDM_TOP://toggle always on top, check/uncheck menu
					if(menuStateTop==MF_CHECKED)
						SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
					else
						SetWindowPos(hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
					hMenu=GetMenu(hWnd);
					menuStateTop=CheckMenuItem(hMenu,IDM_TOP,menuStateTop);
					break;

				case IDM_PAUSE:
					hMenu=GetMenu(hWnd);
					EnableMenuItem(hMenu,IDM_PAUSE,MF_BYCOMMAND|MF_GRAYED);
					EnableMenuItem(hMenu,IDM_RESUME,MF_BYCOMMAND|MF_ENABLED);
					SuspendThread(hWorkerThread);
					break;

				case IDM_RESUME:
					hMenu=GetMenu(hWnd);
					EnableMenuItem(hMenu,IDM_PAUSE,MF_BYCOMMAND|MF_ENABLED);
					EnableMenuItem(hMenu,IDM_RESUME,MF_BYCOMMAND|MF_GRAYED);
					ResumeThread(hWorkerThread);
					break;

				case IDM_TERMINATE:
					hMenu=GetMenu(hWnd);
					EnableMenuItem(hMenu,IDM_TERMINATE,MF_BYCOMMAND|MF_GRAYED);
					bRunThread=FALSE;

					break;
				
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		
		case WM_SIZE:/*
 			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);*/// window isnt resizable
			break;

		
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			brain->paint(hdc);
			EndPaint(hWnd, &ps);				
			break;
		
		case WM_CLOSE:
			//confirm from user
			//if has to quit call destroywindow
			if(MessageBox(hWnd,"Are you sure?",
				"Neural App: Warning",
				MB_ICONQUESTION|MB_YESNO)==IDYES)
				
				DestroyWindow(hWnd);
				
			else
				return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}


// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			
			case IDOK:
			case IDCANCEL:
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

// shows error
void DisplayErr(char *err)
{
	MessageBox(g_hWnd,err,"NeuralApp: error",MB_ICONERROR);
}

// worker thread proc
VOID worker(PVOID param)
{
	char *fpath;
	fpath = (char*) param;
	brain->train(fpath);
}
	

int numLayers,numNeurons[25],numEpochs;
	double lrate,momentum,tol,noise;

LRESULT CALLBACK Config(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{/*
	int numLayers,numNeurons[25],numEpochs;
	double lrate,momentum,tolerence;
*/	char buff[400],*token,seps[]={" ,-;"};
	switch (message)
	{
	/*	case WM_INITDIALOG:
			//always on top
			//SetWindowPos(hDlg,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
			return TRUE;
	*/	
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			
			case IDOK:// get all parameters
				numLayers=GetDlgItemInt(hDlg,IDC_NUMLAYER,NULL,FALSE);

				numEpochs=GetDlgItemInt(hDlg,IDC_NUMEPOCH,NULL,FALSE);

				GetDlgItemText(hDlg,IDC_LRATE,buff,400);
				lrate=atof(buff);
				memset( buff,0,400);

				GetDlgItemText(hDlg,IDC_NOISE,buff,400);
				noise=atof(buff);
				memset( buff,0,400);

				GetDlgItemText(hDlg,IDC_MOMENTUM,buff,400);
				momentum=atof(buff);
				memset( buff,0,400);

				GetDlgItemText(hDlg,IDC_MSETOL,buff,400);
				tol=atof(buff);
				memset( buff,0,400);

				GetDlgItemText(hDlg,IDC_LSIZE,buff,400);
				int i;
				i=0;
				token = strtok( buff, seps );
				while( token != NULL ){
					numNeurons[i]=atoi(token);
					i++;
					token = strtok( NULL, seps );
				}

				if(i!=numLayers){
					DisplayErr("Invalid Layer Information");
					break;
				}

			/*	sprintf(buff,"numLayer: %d\nsize: %d %d %d %d\nnumEpochs: %d\nmomentum: %f\ntolerence: %f\nlrate: %f"
					,numLayers,numNeurons[0],numNeurons[1],numNeurons[2],numNeurons[3],numEpochs,momentum,tolerence,lrate);
				MessageBox(0,buff," ",0);

			*/	
				//brain->setConfig(numLayers,numNeurons,numEpochs,lrate,momentum,tolerence);

				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
				
			case IDCANCEL:
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
	return FALSE;
}