// filedlgs.h
#ifndef _filedlgs_h_
#define _filedlgs_h_

#define FILEDLGS_OPENSAVE   1
#define FILEDLGS_FOLDERS    1

#include <Commdlg.h>
#if FILEDLGS_OPENSAVE
BOOL            GetOpenFileNameDlg(HWND _hwndf, char *_sz_title, char *_sz_ext, char *_sz_desc, char *_sz_buf, int bufsize);
BOOL            GetSaveFileNameDlg(HWND _hwndf, char *_sz_title, char *_sz_ext, char *_sz_desc, char *_sz_buf, int bufsize);
#endif

#if FILEDLGS_FOLDERS
BOOL            GetFolderDlg(HWND _hwndf, char *_sz_title, char *_sz_buf, int _bufsize);
BOOL            GetComputerDlg(HWND _hwndf, char *_sz_title, char *_sz_buf, int _bufsize);
#endif

#endif