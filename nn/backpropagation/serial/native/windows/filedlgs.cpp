#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "filedlgs.h"

#if FILEDLGS_FOLDERS
#include <shlobj.h>
#endif

#if FILEDLGS_OPENSAVE
static BOOL     DlgFileName(HWND hwndf, char *sz_title, char *sz_ext, char *sz_desc, char *sz_buf, int bufsize, BOOL bisopen)
{
    OPENFILENAME    ofn;
    char           *sz_defilt = "All Files (*.*)^*.*^";
    char            szFile[MAX_PATH];
    char            szFilter[147];
    char           *pch;
    char            szInitialDir[MAX_PATH];
    LPITEMIDLIST    ppidl;
	int				ii, ib, ie;
	BOOL			bMatched = TRUE;

    if ((sz_title == sz_desc) || (sz_ext == NULL))
        strcpy(szFilter, sz_defilt);
    else
    {
        if (strlen(sz_ext) > 10)
            return (FALSE);

        if (strlen(sz_desc) > 100)
            return (FALSE);

        strcpy(szFilter, sz_desc);
        strcat(szFilter, " ( *.");
        strcat(szFilter, sz_ext);
        strcat(szFilter, " )^*.");
        strcat(szFilter, sz_ext);
		strcat(szFilter, "^");
        strcat(szFilter, sz_defilt);
    }

    for (pch = szFilter; *pch != 0; pch++)
        if (*pch == '^')
            *pch = '\0';

    szFile[0] = '\0';
    // Set all structure members to zero. 
    memset(&ofn, 0, sizeof(OPENFILENAME));

    SHGetSpecialFolderLocation(hwndf, CSIDL_DESKTOPDIRECTORY, &ppidl);
    SHGetPathFromIDList(ppidl, szInitialDir);
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwndf;
    ofn.lpstrFilter = szFilter;
    ofn.nFilterIndex = 0;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrTitle = sz_title;
    ofn.lpstrInitialDir = szInitialDir;

        if (bisopen)
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER;
    else
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER;

    if (bisopen)
    {
        if (GetOpenFileName(&ofn) != 0)
        {
            strncpy(sz_buf, ofn.lpstrFile, bufsize);
            return (TRUE);
        }
		else
			return (FALSE);
    }
    else
	{
		if (GetSaveFileName(&ofn) != 0)
		{
			strncpy(sz_buf, ofn.lpstrFile, bufsize);

			ib = strlen(sz_buf)-1;
			ie = strlen(sz_ext)-1;

			if (ib >= ie)
			{
				for (ii = 0; ii < (int)ie+1; ii++)
					if ((toupper(sz_buf[ib-ii])) != (toupper(sz_ext[ie-ii])))
						bMatched = FALSE;

				if (bMatched)
					if (sz_buf[ib-ii] != '.')
						bMatched = FALSE;
			}
			else
				bMatched = FALSE;

			if ((ofn.nFilterIndex == 1) && (!bMatched))
				if ((strlen(sz_buf)+strlen(sz_ext)+1) < MAX_PATH)
				{
					strcat(sz_buf,".");
					strcat(sz_buf,sz_ext);
				}

			return (TRUE);
		}
		else
			return (FALSE);
	}
}
#endif

#if FILEDLGS_FOLDERS
static BOOL     DlgGetFolder(HWND hwndf, DWORD dwFlags, char *sz_title, char *sz_buf, int bufsize)
{
    BROWSEINFO      bi;
    LPITEMIDLIST    pidlRoot, pidlChosen;
    char            pszBuffer[MAX_PATH];
    char            szChosenFolder[MAX_PATH] = "\\\\";
    BOOL            bret;

    SHGetSpecialFolderLocation(hwndf, BIF_BROWSEFORCOMPUTER, &pidlRoot);

    memset(&bi, 0, sizeof(BROWSEINFO));
    bi.hwndOwner = hwndf;
    bi.pidlRoot = pidlRoot;
    bi.pszDisplayName = pszBuffer;
    bi.lpszTitle = sz_title;
    bi.ulFlags = dwFlags;

    bret = FALSE;
    if ((pidlChosen = SHBrowseForFolder(&bi)) != NULL)
    {
        if (dwFlags & BIF_BROWSEFORCOMPUTER)
        {
            strncat(szChosenFolder, pszBuffer, MAX_PATH - 2);
            bret = TRUE;
        }
        else if (SHGetPathFromIDList(pidlChosen, pszBuffer))
        {
            strncpy(szChosenFolder, pszBuffer, MAX_PATH);
            bret = TRUE;
        }
    }
    else
        szChosenFolder[0] = '\0';

    strncpy(sz_buf, szChosenFolder, MAX_PATH);
    return (bret);
}
#endif

#if FILEDLGS_OPENSAVE
BOOL            GetOpenFileNameDlg(HWND _hwndf, char *_sz_title, char *_sz_ext, char *_sz_desc, char *_sz_buf, int _bufsize)
{
    return (DlgFileName(_hwndf, _sz_title, _sz_ext, _sz_desc, _sz_buf, _bufsize, TRUE));
}

BOOL            GetSaveFileNameDlg(HWND _hwndf, char *_sz_title, char *_sz_ext, char *_sz_desc, char *_sz_buf, int _bufsize)
{
    return (DlgFileName(_hwndf, _sz_title, _sz_ext, _sz_desc, _sz_buf, _bufsize, FALSE));
}
#endif

#if FILEDLGS_FOLDERS
BOOL            GetFolderDlg(HWND _hwndf, char *_sz_title, char *_sz_buf, int _bufsize)
{
    return (DlgGetFolder(_hwndf, 0L, _sz_title, _sz_buf, _bufsize));
}

BOOL            GetComputerDlg(HWND _hwndf, char *_sz_title, char *_sz_buf, int _bufsize)
{
    return (DlgGetFolder(_hwndf, BIF_BROWSEFORCOMPUTER, _sz_title, _sz_buf, _bufsize));
}
#endif
